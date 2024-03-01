//
//  SaleAnalysis_UI.c
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#include <stdio.h>
#include "Common.h"
#include "SalesAnalysis_UI.h"
#include "../Service/SalesAnalysis.h"
#include "../Common/List.h"

static const int PAGE_SIZE = 5;

void SalesAnalysis_UI_MgtEntry(void) {
    Pagination_t paging;
    paging.offset = 0;
    paging.pageSize = PAGE_SIZE;
    
    salesAnalysis_list_t list;
    List_Init(list, salesAnalysis_node_t);
    paging.totalRecords = SalesAnalysis_Srv_StaticSale(list);
    
    char choice;
    do {
        printf("\n=========================================================================================================================\n");
        printf("******************************************************* Sale ************************************************************\n");
        printf("%30s %10s %10s %16s %8s %16s %12s %12s\n", "Play Name", "Area", "Duration", "Total Tickets",
               "Price", "Sale", "Start", "End");
        printf("-------------------------------------------------------------------------------------------------------------------------\n");
        
        List_Paging(list, paging, salesAnalysis_node_t);
        int i;
        salesAnalysis_node_t *cur;
        Paging_ViewPage_ForEach(list, paging, salesAnalysis_node_t, cur, i) {
            printf("%30s %10s %10d %16ld %8d %16ld %6d.%02d.%02d %6d.%02d.%02d\n",
                   cur->data.name, cur->data.area, cur->data.duration, cur->data.totalTickets,
                   cur->data.price, cur->data.sales, cur->data.start.year, cur->data.start.month,
                   cur->data.start.day, cur->data.end.year, cur->data.end.month, cur->data.end.day);
        }
        
        printf("---------------------- Total Records:%2d --------------------------------------------- Page %2d/%2d ------------------------\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("*************************************************************************************************************************\n");
        printf("           [P]revPage         |        [N]extPage           |          [S]ort       |           [R]eturn\n");
        printf("=========================================================================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 'p':
            case 'P':
                PREV_PAGE(list, paging, salesAnalysis_node_t);
                break;
            case 'n':
            case 'N':
                NEXT_PAGE(list, paging, salesAnalysis_node_t);
                break;
            case 's':
            case 'S':
                SalesAnalysis_Srv_SortBySale(&list);
                break;
        }
    } while ('r' != choice && 'R' != choice);
    List_Destroy(list, salesAnalysis_node_t);
}
