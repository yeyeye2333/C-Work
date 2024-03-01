//
//  Sale.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/29.
//

#ifndef Sale_h
#define Sale_h

#include "Structs.h"

typedef enum {
    SALE_SELL = 1,      // 售票
    SALE_REFUND = -1   // 退票
} sale_type_t; // 交易类型枚举

typedef struct {
    long id;            // 主键ID
    int userID;         // 销售员ID
    int ticketID;       // 票ID
    usr_date_t date;    // 日期
    int price;          // 价格
    sale_type_t type;   // 交易类型
} sale_t; // 交易类型


typedef struct sale_node {
    sale_t data;                // 实体数据
    struct sale_node *next;     // 后向指针
    struct sale_node *prev;     // 前向指针
} sale_node_t, *sale_list_t;    // 交易节点类型、交易链表类型


// 把data添加到文件中。如果添加失败，则返回0
int Sale_Srv_Add(sale_t *data);

// 根据ID删除对象。如果找不到，则返回0
int Sale_Srv_Delete(long id);

// 清除所有记录。返回删除的记录数
int Sale_Srv_Clear(void);

// 根据id查找对象，并将查找到的对象储存到buf中。如果找不到对象，则返回0
int Sale_Srv_FetchByID(int id, sale_t *buf);

// 找到所有对象，并将查找到的对象储存到list中。返回找到的对象数
int Sale_Srv_FetchAll(sale_list_t list);

// 根据userID查找交易记录，并将查找到的交易记录储存到list中。返回找到的记录数
int Sale_Srv_FetchByUserID(int userID, sale_list_t list);

// 根据playID查找售卖的交易记录
int Sale_Srv_FetchSellByPlayID(int playID, sale_list_t list);

#endif /* Sale_h */
