//
//  Sale_UI.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/29.
//

#include <stdio.h>
#include <time.h>
#include "Common.h"
#include "Sale_UI.h"
#include "../Service/Sale.h"
#include "../Common/List.h"

static const int PAGE_SIZE = 5;

// 将交易状态转换成字符串
static char *typeToStr(sale_type_t type) {
    char *res = NULL;
    switch (type) {
        case SALE_SELL:
            res = "sell";
            break;
        case SALE_REFUND:
            res = "refund";
            break;
    }
    return res;
}

void Sale_UI_MgtEntry(void) {
    char choice;
    do {
        printf("\n==================================================================\n");
        printf("************************** Sell Tickets **************************\n");
        printf("[P]erchase     |     Refun[d]     |     Record[s]    |    [R]eturn\n");
        printf("******************************************************************\n");
        printf("==================================================================\n");
        printf("Your choice:");
        readChar(&choice);
        switch (choice) {
            case 'p':
            case 'P':
                Sale_UI_Perchase();
                break;
            case 'd':
            case 'D':
                Sale_UI_Refound();
                break;
            case 's':
            case 'S':
                Sale_UI_Records();
                break;
        }
    } while ('r' != choice && 'R' != choice);
}

void Sale_UI_Perchase(void) {
    int scheduleID;
    printf("ID of schedule:");
    readInt(&scheduleID);
    
    ticket_t *ticket = new(ticket_t);
    if (!Ticket_Srv_FetchAvailableByScheduleID(scheduleID, ticket)) {
        printf("No ticket!");
        getchar();
        return;
    }
    
    printf("This ticket's price is %d¥.", ticket->price);
    printf(" Are you sure to buy it?\n");
    printf("[Y]es | [N]o\n");
    printf("Your choice:");
    char choice;
    readChar(&choice);
    if ('y' != choice && 'Y' != choice) {
        printf("Bye.");
        getchar();
        return;
    }
    
    int userID;
    printf("ID of user:");
    readInt(&userID);
    if (userID < 1) {
        printf("UserID must bigger than 0!");
        getchar();
        return;
    }
    
    ticket->status = TICKET_SOLD;
    if (!Ticket_Srv_Modify(ticket)) {
        printf("Perchase failed! Because of file operation.");
        getchar();
        return;
    }
    
    sale_t *perchased = new(sale_t);
    perchased->type = SALE_SELL;
    perchased->userID = userID;
    perchased->price = ticket->price;
    perchased->ticketID = ticket->id;
    GET_DATE(perchased->date.year, perchased->date.month, perchased->date.day);
    
    if (!Sale_Srv_Add(perchased)) {
        printf("Perchase failed!");
        getchar();
        return;
    }
    
    printf("Your sale id is <%ld>, please remember it.", perchased->id);
    getchar();
}

void Sale_UI_Refound(void) {
    int userID;
    printf("ID of refunded user:");
    readInt(&userID);
    if (userID < 1) {
        printf("UserID must bigger than 0!");
        getchar();
        return;
    }
    
    int saleID;
    printf("ID of sale:");
    readInt(&saleID);
    
    sale_t *sale = new(sale_t);
    if (!Sale_Srv_FetchByID(saleID, sale)) {
        printf("Cannot find sale %d!", saleID);
        getchar();
        return;
    }
    
    if (sale->userID != userID) {
        printf("Please enter correct uesr id!");
        getchar();
        return;
    }
    
    ticket_t *ticket = new(ticket_t);
    if (!Ticket_Srv_FetchByID(sale->ticketID, ticket)) {
        printf("No ticket!");
        getchar();
        return;
    }
    
    ticket->status = TICKET_AVL;
    if (!Ticket_Srv_Modify(ticket)) {
        printf("Refund failed! Because of file operation.");
        getchar();
        return;
    }
    
    sale_t *refunded = new(sale_t);
    refunded->type = SALE_REFUND;
    refunded->userID = userID;
    refunded->price = ticket->price;
    refunded->ticketID = ticket->id;
    GET_DATE(refunded->date.year, refunded->date.month, refunded->date.day);
    
    printf("Refund %s!", (Sale_Srv_Add(refunded) ? "sucessfully" : "failed"));
    getchar();
}

void Sale_UI_Records(void) {
    Pagination_t paging;
    paging.offset = 0;
    paging.pageSize = PAGE_SIZE;
    
    char choice;
    do {
        sale_list_t sales;
        List_Init(sales, sale_node_t);
        paging.totalRecords = Sale_Srv_FetchAll(sales);
        List_Paging(sales, paging, sale_node_t);
        
        printf("\n==================================================================\n");
        printf("************************** Sale Records **************************\n");
        printf("%8s %10s %12s %13s %9s %9s\n", "Sale ID", "User ID", "Ticket ID", "Date",
               "Price", "Type");
        printf("------------------------------------------------------------------\n");
        
        int i;
        sale_node_t *cur;
        Paging_ViewPage_ForEach(sales, paging, sale_node_t, cur, i) {
            printf("%8ld %10d %12d %7d.%02d.%02d %9d %9s\n", cur->data.id, cur->data.userID,
                   cur->data.ticketID, cur->data.date.year, cur->data.date.month,
                   cur->data.date.day, cur->data.price, typeToStr(cur->data.type));
        }
        
        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ------\n",
                paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage | [N]extPage   |   [D]elete  |  [C]lear   |    [R]eturn\n");
        printf("==================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 'p':
            case 'P':
                if (!Pageing_IsFirstPage(paging)) {
                    Paging_Locate_OffsetPage(sales, paging, -1, sale_node_t);
                }
                break;
            case 'n':
            case 'N':
                if (!Pageing_IsLastPage(paging)) {
                    Paging_Locate_OffsetPage(sales, paging, 1, sale_node_t);
                }
                break;
            case 'd':
            case 'D':
                Sale_UI_Delete();
                break;
            case 'c':
            case 'C':
                Sale_UI_Clear();
                break;
        }
        List_Destroy(sales, sale_node_t);
    } while ('r' != choice && 'R' != choice);
}

void Sale_UI_Delete(void) {
    int userID;
    printf("User id of deleted sale:");
    readInt(&userID);
    if (userID < 1) {
        printf("UserID must bigger than 0!");
        getchar();
        return;
    }
    
    int saleID;
    printf("ID of deleted sale:");
    readInt(&saleID);
    
    sale_t *sale = new(sale_t);
    if (!Sale_Srv_FetchByID(saleID, sale)) {
        printf("Cannot find sale %d!", saleID);
        getchar();
        return;
    }
    
    if (sale->userID != userID) {
        printf("Please enter correct uesr id!");
        getchar();
        return;
    }
    
    printf("Delete record %s!", (Sale_Srv_Delete(saleID) ? "sucessfully" : "failed"));
    getchar();
}

void Sale_UI_Clear(void) {
    printf("This operation will clear all records. Are you sure to clear?\n");
    printf("[Y]es | [N]o\n");
    printf("Your choice:");
    char choice;
    readChar(&choice);
    if ('y' != choice && 'Y' != choice) {
        printf("Bye.");
        getchar();
        return;
    }
    
    printf("This operation affected %d records.", Sale_Srv_Clear());
    getchar();
}
