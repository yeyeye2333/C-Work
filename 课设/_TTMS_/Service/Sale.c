//
//  Sale.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/29.
//

#include "Schedule.h"
#include "Ticket.h"
#include "Sale.h"
#include "../Persistence/Sale_Persist.h"
#include "../Common/List.h"

int Sale_Srv_Add(sale_t *data) {
    return Sale_Perst_Insert(data);
}

int Sale_Srv_Delete(long id) {
    return Sale_Perst_DeleteByID(id);
}

int Sale_Srv_Clear(void) {
    sale_list_t sales;
    List_Init(sales, sale_node_t);
    Sale_Srv_FetchAll(sales);
    
    int count = 0;
    sale_node_t *cur;
    List_ForEach(sales, cur) {
        if (Sale_Srv_Delete(cur->data.id)) {
            count++;
        }
    }
    return count;
}

int Sale_Srv_FetchByID(int id, sale_t *buf) {
    return Sale_Perst_SelectByID(id, buf);
}

int Sale_Srv_FetchAll(sale_list_t list) {
    return Sale_Perst_SelectAll(list);
}

int Sale_Srv_FetchByUserID(int userID, sale_list_t list) {
    sale_list_t sales = NULL;
    List_Init(sales, sale_node_t);
    if (!Sale_Srv_FetchAll(sales)) {
        return 0;
    }
    
    int count = 0;
    sale_node_t *cur;
    List_ForEach(sales, cur) {
        if (userID == cur->data.userID) {
            sale_node_t *newNode = new(sale_node_t);
            *newNode = *cur;
            List_AddTail(list, newNode);
            count++;
        }
    }
    return count;
}

int Sale_Srv_FetchSellByPlayID(int playID, sale_list_t list) {
    sale_list_t sales = NULL;
    List_Init(sales, sale_node_t);
    if (!Sale_Srv_FetchAll(sales)) {
        return 0;
    }
    
    int count = 0;
    sale_node_t *cur;
    List_ForEach(sales, cur) {
        if (cur->data.type == SALE_REFUND) {
            continue;
        }
        
        ticket_t *ticket = new(ticket_t);
        if (!Ticket_Srv_FetchByID(cur->data.ticketID, ticket)) {
            continue;
        }
        
        schedule_t *schedule = new(schedule_t);
        if (!Schedule_Srv_FetchByID(ticket->scheduleID, schedule)) {
            continue;
        }
        
        if (playID == schedule->playID) {
            sale_node_t *newNode = new(sale_node_t);
            *newNode = *cur;
            List_AddTail(list, newNode);
            count++;
        }
    }
    return count;
}
