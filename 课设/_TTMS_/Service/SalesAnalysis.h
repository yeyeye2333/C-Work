//
//  SaleAnalysis.h
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#ifndef SaleAnalysis_h
#define SaleAnalysis_h

#include "Structs.h"

typedef struct {
    int playID;         // 剧目编号
    char *name;         // 剧目名称
    char *area;         // 剧目地区
    int duration;       // 剧目播放时长
    long totalTickets;  // 剧目上座数量
    long sales;         // 剧目票房
    int price;          // 剧目票价
    usr_date_t start;   // 剧目上映日期
    usr_date_t end;     // 剧目下映日期
} salesAnalysis_t; // 票房类型

typedef struct salesAnalysis_node {
    salesAnalysis_t data;            // 实体数据
    struct salesAnalysis_node *prev; // 前向指针
    struct salesAnalysis_node *next; // 后向指针
} salesAnalysis_node_t, *salesAnalysis_list_t; // 票房节点类型、票房链表类型

// 获取剧目票房信息
int SalesAnalysis_Srv_StaticSale(salesAnalysis_list_t list);

// 剧目票房排序
void SalesAnalysis_Srv_SortBySale(salesAnalysis_list_t *plist);

#endif /* SaleAnalysis_h */
