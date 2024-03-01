//
//  Seat_UI.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#ifndef Seat_UI_h
#define Seat_UI_h

#include "../Service/Seat.h"

/*
 标识符：SCU_Seat_UI_MgtEnt
 功能：管理座位
 */
void Seat_UI_MgtEntry(int roomID);

/*
 标识符：SCU_Seat_UI_S2C
 功能：把座位状态转换成字符
 */
char Seat_UI_Status2Char(seat_status_t status);

/*
 标识符：TTMS_SCU_Seat_UI_C2S
 功能：把字符转换成座位状态
 */
seat_status_t Seat_UI_Char2Status(char statusChar);

/*
 标识符：TTMS_SCU_Seat_UI_Add
 功能：添加座位
 */
int Seat_UI_Add(seat_list_t list, int roomID, int rowsCount, int colsCount);

/*
 标识符：TTMS_SCU_Seat_UI_Mod
 功能：修改座位
 */
int Seat_UI_Modify(seat_list_t list, int rowsCount, int colsCount);

/*
 标识符：TTMS_SCU_Seat_UI_Del
 功能：删除座位
 */
int Seat_UI_Delete(seat_list_t list, int rowsCount, int colsCount);

#endif /* Seat_UI_h */
