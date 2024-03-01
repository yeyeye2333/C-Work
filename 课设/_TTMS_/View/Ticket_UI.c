//
//  Ticket_UI.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/29.
//

#include <stdio.h>
#include "Common.h"
#include "Seat_UI.h"
#include "Ticket_UI.h"
#include "../Service/Studio.h"
#include "../Service/Schedule.h"
#include "../Service/Ticket.h"
#include "../Common/List.h"

static const int PAGE_SIZE = 5;

// 将票状态转换成字符
static char statusToChar(ticket_status_t status) {
    char res = '&';
    switch (status) {
        case TICKET_NIL:
            break;
        case TICKET_AVL:
            res = '#';
            break;
        case TICKET_SOLD:
            res = '$';
            break;
        case TICKET_RESV:
            res = '@';
            break;
    }
    return res;
}

// 将字符转换成状态
static ticket_status_t charToStatus(char status) {
    ticket_status_t res = TICKET_NIL;
    switch (status) {
        case 'n':
        case 'N':
            break;
        case 'a':
        case 'A':
            res = TICKET_AVL;
            break;
        case 's':
        case 'S':
            res = TICKET_SOLD;
            break;
        case 'r':
        case 'R':
            res = TICKET_RESV;
            break;
    }
    return res;
}

// 将状态转换成字符串
static char *statusToStr(ticket_status_t status) {
    char *res = "NONE";
    switch (status) {
        case TICKET_NIL:
            break;
        case TICKET_AVL:
            res = "available";
            break;
        case TICKET_SOLD:
            res = "sold";
            break;
        case TICKET_RESV:
            res = "reserve";
            break;
    }
    return res;
}

void Ticket_UI_MgtEntry(int scheduleID) {
    schedule_t *schedule = new(schedule_t);
    Schedule_Srv_FetchByID(scheduleID, schedule);
    
    studio_t *studio = new(studio_t);
    Studio_Srv_FetchByID(schedule->studioID, studio);
    
    seat_list_t seats = NULL;
    List_Init(seats, seat_node_t);
    Seat_Srv_FetchByRoomID(seats, schedule->studioID);
    
    char choice;
    do {
        printf("\n==================================================================\n");
        printf("*********************** Ticket Management ************************\n");
        printf(" '&' <-> null - '#' <-> available - '$' <-> sold - '@' <-> reserve\n");
        
        printf(" ");
        for (int i = 0; i < studio->colsCount; i++) {
            printf("%4d", (i + 1));
        }
        printf("\n");
        for (int i = 1; i <= studio->rowsCount; i++) {
            printf("%d", i);
            for (int j = 1; j <= studio->colsCount; j++) {
                int flag = 0;
                seat_node_t *cur;
                List_ForEach(seats, cur) {
                    if (cur->data.row == i && cur->data.column == j) {
                        flag = 1;
                        ticket_t *ticket = new(ticket_t);
                        if (!Ticket_Srv_FetchBySeatAndSchedule(cur->data.id, scheduleID, ticket)) {
                            ticket->status = TICKET_NIL;
                        }
                        printf("%4c", statusToChar(ticket->status));
                        break;
                    }
                }
                if (!flag) {
                    printf("    ");
                }
            }
            printf("\n");
        }
        
        printf("******************************************************************\n");
        printf(" [A]dd|Add [B]atch | Delete Bat[c]h|[D]elete | [U]pdate | [R]eturn\n");
        printf("==================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 'a':
            case 'A':
                Ticket_UI_Add(scheduleID, seats);
                break;
            case 'b':
            case 'B':
                Ticket_UI_AddBatch(scheduleID);
                break;
            case 'c':
            case 'C':
                Ticket_UI_DeleteBatch(scheduleID);
                break;
            case 'd':
            case 'D':
                Ticket_UI_Delete(scheduleID, seats);
                break;
            case 'u':
            case 'U':
                Ticket_UI_Modify(scheduleID, seats);
                break;
        }
    } while ('r' != choice && 'R' != choice);
}

void Ticket_UI_Add(int scheduleID, seat_list_t list) {
    int row, col;
    printf("Row of ticket:");
    readInt(&row);
    printf("Col of ticket:");
    readInt(&col);
    
    seat_node_t *seat = Seat_Srv_FindByRowCol(list, row, col);
    if (NULL == seat) {
        printf("Cannot find row %d, column %d seat!", row, col);
        getchar();
        return;
    }
    
    ticket_t *temp = new(ticket_t);
    if (Ticket_Srv_FetchBySeatAndSchedule(seat->data.id, scheduleID, temp)) {
        printf("Cannot add the same ticket!");
        getchar();
        return;
    }
    
    int price;
    printf("Price of ticket:");
    readInt(&price);
    if (price < 1) {
        printf("Price should bigger than 0!");
        getchar();
        return;
    }
    
    char status;
    printf("[N]ull                           [S]old\n[A]vailable                      [R]eserve\n");
    printf("Status of ticket:");
    readChar(&status);
    
    ticket_t *added = new(ticket_t);
    added->scheduleID = scheduleID;
    added->seatID = seat->data.id;
    added->price = price;
    added->status = charToStatus(status);
    
    printf("Add %s!", (Ticket_Srv_Add(added) ? "sucessfully" : "failed"));
    getchar();
}

void Ticket_UI_AddBatch(int scheduleID) {
    int addCount = Ticket_Srv_AddBatch(scheduleID);
    printf("This operation affected %d tickets!", addCount);
    getchar();
}

void Ticket_UI_Delete(int scheduleID, seat_list_t list) {
    int row, col;
    printf("Row of deleted ticket:");
    readInt(&row);
    printf("Col of deleted ticket:");
    readInt(&col);
    
    seat_node_t *seat = Seat_Srv_FindByRowCol(list, row, col);
    if (NULL == seat) {
        printf("Cannot find row %d, column %d seat!", row, col);
        getchar();
        return;
    }
    
    ticket_t *deleted = new(ticket_t);
    if (!Ticket_Srv_FetchBySeatAndSchedule(seat->data.id, scheduleID, deleted)) {
        printf("Cannot find this ticket!");
        getchar();
        return;
    }
    
    printf("Delete %s!", (Ticket_Srv_Delete(deleted->id) ? "sucessfully" : "failed"));
    getchar();
}

void Ticket_UI_DeleteBatch(int scheduleID) {
    int deleteCount = Ticket_Srv_DeleteBatch(scheduleID);
    printf("This operation affected %d tickets!", deleteCount);
    getchar();
}

void Ticket_UI_Modify(int scheduleID, seat_list_t list) {
    int row, col;
    printf("Row of ticket:");
    readInt(&row);
    printf("Col of ticket:");
    readInt(&col);
    
    seat_node_t *seat = Seat_Srv_FindByRowCol(list, row, col);
    if (NULL == seat) {
        printf("Cannot find row %d, column %d seat!", row, col);
        getchar();
        return;
    }
    
    ticket_t *modified = new(ticket_t);
    if (!Ticket_Srv_FetchBySeatAndSchedule(seat->data.id, scheduleID, modified)) {
        printf("Cannot find this ticket!");
        getchar();
        return;
    }
    
    int price;
    printf("Price of ticket[%d]:", modified->price);
    readInt(&price);
    if (price < 1) {
        printf("Price should bigger than 0!");
        getchar();
        return;
    }
    
    char status;
    printf("[N]ull                           [S]old\n[A]vailable                      [R]eserve\n");
    printf("Status of ticket:");
    readChar(&status);
    
    modified->price = price;
    modified->status = charToStatus(status);
    
    printf("Update %s!", (Ticket_Srv_Modify(modified) ? "sucessfully" : "failed"));
    getchar();
}

void Ticket_UI_Query(void) {
    int scheduleID;
    printf("Id of schedule:");
    readInt(&scheduleID);

    Pagination_t paging;
    paging.offset = 0;
    paging.pageSize = PAGE_SIZE;
    
    char choice;
    do {
        ticket_list_t tickets;
        List_Init(tickets, ticket_node_t);
        paging.totalRecords = Ticket_Srv_FetchByScheduleID(scheduleID, tickets);
        List_Paging(tickets, paging, ticket_node_t);
        
        printf("\n==================================================================\n");
        printf("********************** Schedule %02d's Tickets *********************\n", scheduleID);
        printf("%9s %16s %12s %13s %12s\n", "Ticket ID", "Schedule ID", "Seat ID", "Price", "Status");
        printf("------------------------------------------------------------------\n");
        
        int i;
        ticket_node_t *cur;
        Paging_ViewPage_ForEach(tickets, paging, ticket_node_t, cur, i) {
            printf("%9d %16d %12d %13d %12s\n", cur->data.id, cur->data.scheduleID,
                   cur->data.seatID, cur->data.price, statusToStr(cur->data.status));
        }
        
        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ------\n",
                paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("      [P]revPage     |      [N]extPage      |     [R]eturn\n");
        printf("==================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 'p':
            case 'P':
                PREV_PAGE(tickets, paging, ticket_node_t);
                break;
            case 'n':
            case 'N':
                NEXT_PAGE(tickets, paging, ticket_node_t);
                break;
        }
        List_Destroy(tickets, ticket_node_t);
    } while ('r' != choice && 'R' != choice);
}
