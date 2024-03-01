//
//  Main_Menu.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/26.
//

#include <stdio.h>
#include "Main_Menu.h"
#include "Common.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Ticket_UI.h"
#include "Sale_UI.h"
#include "StaSales_UI.h"
#include "SalesAnalysis_UI.h"
#include "Account_UI.h"
#include "../Service/Schedule.h"
#include "../Service/Ticket.h"
#include "../Common/List.h"

static const int SCHEDULE_PAGE_SIZE = 5;

void SM_Menu(void) {
    char choice;
    do {
        printf("\n==================================================================\n");
        printf("************************* System Manager *************************\n");
        printf("[S]tudio Management.\n");
        printf("[A]ccount Management.\n");
        printf("[C]hange Password.\n");
        printf("[R]eturn.\n");
        printf("==================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 's':
            case 'S':
                Studio_UI_MgtEntry();
                break;
            case 'a':
            case 'A':
                Account_UI_MgtEntry();
                break;
            case 'c':
            case 'C':
                Account_UI_ModifyForSelf();
                break;
        }
    } while ('r' != choice && 'R' != choice);
}

void PM_Menu(void) {
    char choice;
    do {
        printf("\n==================================================================\n");
        printf("************************** Play Manager **************************\n");
        printf("Play [M]anagement.\n");
        printf("Arrange [P]erform.\n");
        printf("Query [T]icket.\n");
        printf("[S]ale Statistics.\n");
        printf("[B]usiness Statistics.\n");
        printf("[C]hange Password.\n");
        printf("[R]eturn.\n");
        printf("==================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 'm':
            case 'M':
                Play_UI_MgtEntry();
                break;
            case 'p':
            case 'P':
                Schedule_UI_MgtEntry();
                break;
            case 't':
            case 'T':
                Ticket_UI_Query();
                break;
            case 's':
            case 'S':
                StaSales_UI_Clerk();
                break;
            case 'b':
            case 'B':
                SalesAnalysis_UI_MgtEntry();
                break;
            case 'c':
            case 'C':
                Account_UI_ModifyForSelf();
                break;
        }
    } while ('r' != choice && 'R' != choice);
}

void TS_Menu(void) {
    Pagination_t paging;
    paging.offset = 0;
    paging.pageSize = SCHEDULE_PAGE_SIZE;
    
    char choice;
    do {
        schedule_list_t list;
        List_Init(list, schedule_node_t);
        paging.totalRecords = Schedule_Srv_FetchAll(list);
        List_Paging(list, paging, schedule_node_t);
        
        printf("\n=================================================================================\n");
        printf("********************************** Ticket Seller ********************************\n");
        printf("%11s %9s %10s %20s %12s %14s\n", "Schedule ID", "Play ID", "Studio ID", "Show Time", "Seat Count", "Rest Tickets");
        printf("---------------------------------------------------------------------------------\n");
        
        int i;
        schedule_node_t *cur;
        Paging_ViewPage_ForEach(list, paging, schedule_node_t, cur, i) {
            printf("%8d %10d %9d %8d-%02d-%02d-%02d:%02d:%02d %12d", cur->data.id,
                   cur->data.playID, cur->data.studioID, cur->data.date.year,
                   cur->data.date.month, cur->data.date.day, cur->data.time.hour,
                   cur->data.time.minute, cur->data.time.second, cur->data.seatCount);
            ticket_list_t tickets;
            List_Init(tickets, ticket_node_t);
            int ticketCount = 0;
            Ticket_Srv_FetchByScheduleID(cur->data.id, tickets);
            ticket_node_t *curTicket;
            List_ForEach(tickets, curTicket) {
                if (curTicket->data.status == TICKET_AVL) {
                    ticketCount++;
                }
            }
            printf("%15d\n", ticketCount);
        }
        
        printf("*********************************************************************************\n");
        printf("------------- Total Records:%2d --------------------------- Page %2d/%2d -----------\n",
                paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("Q[u]ery Perform.  |   S[a]le Statistics.  |  [Q]uery Ticket.    |  [S]ell Ticket.\n");
        printf("[P]revPage        |   [N]extPage          |  [C]hange Password. |  [R]eturn.\n");
        printf(".\n");
        printf("=================================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 'p':
            case 'P':
                if (!Pageing_IsFirstPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
                }
                break;
            case 'n':
            case 'N':
                if (!Pageing_IsLastPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
                }
                break;
            case 'u':
            case 'U':
                Schedule_UI_Query();
                break;
            case 'a':
            case 'A':
                StaSales_UI_Self();
                break;
            case 'q':
            case 'Q':
                Ticket_UI_Query();
                break;
            case 's':
            case 'S':
                Sale_UI_MgtEntry();
                break;
            case 'c':
            case 'C':
                Account_UI_ModifyForSelf();
                break;
        }
    } while ('r' != choice && 'R' != choice);
}

void Main_Menu(void) {
    char choice;
    do {
        printf("\n==================================================================\n");
        printf("**************** Theater Ticket Management System ****************\n");
        printf("[S]ystem Manager.\n");
        printf("[P]lay Manager.\n");
        printf("[T]icket Seller.\n");
        printf("[R]eturn.\n");
        printf("==================================================================\n");
        printf("Your choice:");

        readChar(&choice);
        switch (choice) {
            case 'S':
            case 's':
                SM_Menu();
                break;
            case 'P':
            case 'p':
                PM_Menu();
                break;
            case 'T':
            case 't':
                TS_Menu();
                break;
        }
    } while ('r' != choice && 'R' != choice);
}
