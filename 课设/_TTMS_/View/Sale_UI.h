//
//  Sale_UI.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/29.
//

#ifndef Sale_UI_h
#define Sale_UI_h

#include"../Service/Seat.h"
#include"../Service/Ticket.h"

// 交易管理函数
void Sale_UI_MgtEntry(void);

// 购票函数
void Sale_UI_Perchase(void);

// 退票函数
void Sale_UI_Refound(void);

// 查看所有记录函数
void Sale_UI_Records(void);

// 删除交易的函数
void Sale_UI_Delete(void);

// 清除所有交易的记录
void Sale_UI_Clear(void);

#endif /* Sale_UI_h */
