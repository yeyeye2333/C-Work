//
//  Ticket_UI.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/29.
//

#ifndef Ticket_UI_h
#define Ticket_UI_h

#include "../Service/Seat.h"

// 票管理函数
void Ticket_UI_MgtEntry(int scheduleID);

// 增加单个座位的票的函数
void Ticket_UI_Add(int scheduleID, seat_list_t list);

// 批量增加指定演出计划中所有座位的票，除已经添加过的票之外
void Ticket_UI_AddBatch(int scheduleID);

// 删除单个座位的票的函数
void Ticket_UI_Delete(int scheduleID, seat_list_t list);

// 批量删除指定演出计划中所有座位的票
void Ticket_UI_DeleteBatch(int scheduleID);

// 修改票函数
void Ticket_UI_Modify(int scheduleID, seat_list_t list);

// 查询票函数
void Ticket_UI_Query(void);

#endif /* Ticket_UI_h */
