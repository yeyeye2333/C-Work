//
//  SaleAnalysis.c
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#include <assert.h>
#include "Play.h"
#include "Sale.h"
#include "SalesAnalysis.h"
#include "../Common/List.h"

int SalesAnalysis_Srv_StaticSale(salesAnalysis_list_t list) {
    assert(NULL != list);
    
    play_list_t plays = NULL;
    List_Init(plays, play_node_t);
    if (!Play_Srv_FetchAll(plays)) {
        return 0;
    }
    
    int count = 0;
    play_node_t *cur;
    List_ForEach(plays, cur) {
        sale_list_t sales;
        List_Init(sales, sale_node_t);
        int totalTickets = Sale_Srv_FetchSellByPlayID(cur->data.id, sales);
        
        salesAnalysis_node_t *newNode = new(salesAnalysis_node_t);
        newNode->data.name = cur->data.name;
        newNode->data.duration = cur->data.duration;
        newNode->data.start = cur->data.start_date;
        newNode->data.end = cur->data.end_date;
        newNode->data.area = cur->data.area;
        newNode->data.playID = cur->data.id;
        newNode->data.price = cur->data.price;
        newNode->data.totalTickets = totalTickets;
        newNode->data.sales = totalTickets * (newNode->data.price);
        
        List_AddTail(list, newNode);
        
        count++;
    }
    
    return count;
}

void SalesAnalysis_Srv_SortBySale(salesAnalysis_list_t *plist) {
    assert(NULL != (*plist));
    // 1. 先获取链表的长度和最值
    int len = 0;
    salesAnalysis_node_t *cur;
    List_ForEach(*plist, cur) {
        len++;
    }
    
    // 2. 如果size=0，那么没有元素，不用排序
    if (!len) {
        return;
    }
    
    // 3. 初始化数组
    salesAnalysis_node_t *sorting[len];
    int i = 0;
    List_ForEach(*plist, cur) {
        salesAnalysis_node_t *newNode = new(salesAnalysis_node_t);
        newNode->data = cur->data;
        sorting[i++] = newNode;
    }
    
    // 4. 使用插入排序将数组中的值变为有序
    for (int low = 1; low < len; low++) {
        salesAnalysis_node_t *temp = sorting[low];
        int j = low - 1;
        while (j >= 0 && temp->data.sales < sorting[j]->data.sales) {
            sorting[i + 1] = sorting[i];
            i--;
        }
        sorting[i + 1] = temp;
    }
    
    // 5. 将原先的list重新初始化
    List_Init(*plist, salesAnalysis_node_t);
    
    // 6. 再将数组中已经有序的数据(倒序)加入到原来的链表
    for (int j = 0; j < len; j++) {
        List_AddHead(*plist, sorting[j]);
    }
}
