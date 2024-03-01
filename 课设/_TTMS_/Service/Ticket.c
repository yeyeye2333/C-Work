//
//  Ticket.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#include <stdio.h>
#include "Seat.h"
#include "Play.h"
#include "Ticket.h"
#include "Schedule.h"
#include "../Persistence/Ticket_Persist.h"
#include "../Common/List.h"

int Ticket_Srv_Add(ticket_t *data) {
    return Ticket_Perst_Insert(data);
}

int Ticket_Srv_AddBatch(int scheduleID) {
    schedule_t *schedule = new(schedule_t);
    Schedule_Srv_FetchByID(scheduleID, schedule);
    
    play_t *play = new(play_t);
    Play_Srv_FetchByID(schedule->playID, play);
    
    seat_list_t seatList = NULL;
    List_Init(seatList, seat_node_t);
    Seat_Srv_FetchByRoomID(seatList, schedule->studioID);
    
    int count = 0;
    seat_node_t *cur;
    List_ForEach(seatList, cur) {
        ticket_t *data = new(ticket_t);
        // 如果本次演出计划的这个座位已经有票了，那么就添加下一个座位
        if (Ticket_Srv_FetchBySeatAndSchedule(cur->data.id, scheduleID, data)) {
            continue;
        }
        
        data->price = play->price;
        data->scheduleID = scheduleID;
        data->seatID = cur->data.id;
        data->status = TICKET_AVL;
        
        
        if (!Ticket_Perst_Insert(data)) {
            return count;
        }
        
        count++;
    }
    
    return count;
}

int Ticket_Srv_Delete(int id) {
    return Ticket_Perst_DeleteByID(id);
}

int Ticket_Srv_DeleteBatch(int scheduleID) {
    ticket_list_t list;
    List_Init(list, ticket_node_t);
    if (!Ticket_Srv_FetchByScheduleID(scheduleID, list)) {
        return 0;
    }
    
    int count = 0;
    ticket_node_t *cur;
    List_ForEach(list, cur) {
        if (!Ticket_Perst_DeleteByID(cur->data.id)) {
            continue;
        }
        
        count++;
    }
    return count;
}

int Ticket_Srv_Modify(ticket_t *data) {
    return Ticket_Perst_Update(data);
}

int Ticket_Srv_FetchByID(int id, ticket_t *buf) {
    return Ticket_Perst_SelectByID(id, buf);
}

int Ticket_Srv_FetchAll(ticket_list_t list) {
    return Ticket_Perst_SelectAll(list);
}

//int Ticket_Srv_FetchBySeatID(int seatID, ticket_list_t list) {
//    ticket_list_t tickets = NULL;
//    List_Init(tickets, ticket_node_t);
//    Ticket_Srv_FetchAll(tickets);
//    
//    int count = 0;
//    ticket_node_t *cur;
//    List_ForEach(tickets, cur) {
//        if (seatID == cur->data.seatID) {
//            List_AddTail(list, cur);
//            count++;
//        }
//    }
//    return count;
//}

int Ticket_Srv_FetchByScheduleID(int scheduleID, ticket_list_t list) {
    ticket_list_t allTickets = NULL;
    List_Init(allTickets, ticket_node_t);
    if (!Ticket_Perst_SelectAll(allTickets)) {
        return 0;
    }
    
    int count = 0;
    ticket_node_t *cur;
    List_ForEach(allTickets, cur) {
        if (scheduleID == cur->data.scheduleID) {
            ticket_node_t *newNode = new(ticket_node_t);
            newNode->data = cur->data;
            List_AddTail(list, newNode);
            count++;
        }
    }
    return count;
}

int Ticket_Srv_FetchBySeatAndSchedule(int seatID, int scheduleID, ticket_t *buf) {
    ticket_list_t tickets = NULL;
    List_Init(tickets, ticket_node_t);
    if (!Ticket_Srv_FetchByScheduleID(scheduleID, tickets)) {
        return 0;
    }
    
    ticket_node_t *cur;
    List_ForEach(tickets, cur) {
        if (seatID == cur->data.seatID) {
            *buf = cur->data;
            return 1;
        }
    }
    
    return 0;
}

int Ticket_Srv_FetchAvailableByScheduleID(int scheduleID, ticket_t *buf) {
    ticket_list_t tickets = NULL;
    List_Init(tickets, ticket_node_t);
    if (!Ticket_Srv_FetchByScheduleID(scheduleID, tickets)) {
        return 0;
    }
    
    ticket_node_t *cur;
    List_ForEach(tickets, cur) {
        if (TICKET_AVL == cur->data.status) {
            *buf = cur->data;
            return 1;
        }
    }
    
    return 0;
}
