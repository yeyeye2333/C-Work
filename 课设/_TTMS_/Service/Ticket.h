//
//  Ticket.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#ifndef Ticket_h
#define Ticket_h

typedef enum {
    TICKET_NIL = 0,     // 无票 null
    TICKET_AVL = 1,     // 待售 available
    TICKET_SOLD = 2,    // 已售 sold
    TICKET_RESV = 9     // 预留 reserve
} ticket_status_t; // 票状态枚举

typedef struct{
    int id;                 // 主键ID
    int scheduleID;         // 演出计划ID
    int seatID;             // 座位ID
    int price;              // 价格
    ticket_status_t status; // 票状态
} ticket_t; // 票类型

typedef struct ticket_node {
    ticket_t data;              // 实体数据
    struct ticket_node *next;   // 后向指针
    struct ticket_node *prev;   // 前向指针
} ticket_node_t, *ticket_list_t; // 票节点类型、票链表类型

// 把data添加到文件中。如果添加失败，则返回0
int Ticket_Srv_Add(ticket_t *data);

// 根据scheduleID批量生成票。返回生成的票数
int Ticket_Srv_AddBatch(int scheduleID);

// 根据ID删除票。如果找不到，则返回0
int Ticket_Srv_Delete(int id);

// 根据scheduleID批量删除票。返回删除的票数
int Ticket_Srv_DeleteBatch(int scheduleID);

// 修改data在文件中的记录。如果找不到data的记录，则返回0
int Ticket_Srv_Modify(ticket_t *data);

// 根据id查找票，并将查找到的票储存到buf中。如果找不到票，则返回0
int Ticket_Srv_FetchByID(int id, ticket_t *buf);

// 找到所有票，并将查找到的票储存到list中。返回找到的票数
int Ticket_Srv_FetchAll(ticket_list_t list);

// 根据seatID查找票，并将查找到的票都储存到list中。返回找到的票数
//int Ticket_Srv_FetchBySeatID(int seatID, ticket_list_t list);

// 根据scheduleID查找票，并将查找到的票储存到list中。返回list中节点的个数
int Ticket_Srv_FetchByScheduleID(int scheduleID, ticket_list_t list);

// 根据seatID和scheduleID查找票，并将查找到的票储存到buf中。如果找不到，则返回0
int Ticket_Srv_FetchBySeatAndSchedule(int seatID, int scheduleID, ticket_t *buf);

// 根据scheduleID查找待售的票，并将查找到的票储存到buf中。如果找不到，则返回0
int Ticket_Srv_FetchAvailableByScheduleID(int scheduleID, ticket_t *buf);

#endif /* Ticket_h */
