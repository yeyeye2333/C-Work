//
//  Seat.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#include <stdio.h>
#include <stdlib.h>
#include "Studio.h"
#include "Seat.h"
#include "../Persistence/Studio_Persist.h"
#include "../Persistence/Seat_Persist.h"
#include "../Common/List.h"

int Seat_Srv_Add(seat_t *data) {
    // 1. 获取行和列的值
    int row, column;
    row = data->row;
    column = data->column;
    
    // 2. 检查演出厅是否存在
    studio_t *studio = (studio_t*)malloc(sizeof(studio_t));
    if (!Studio_Perst_SelectByID(data->roomID, studio)) {
        printf("Can not find room %d!", data->roomID);
        getchar();
        return 0;
    }
    
    // 3. 获取本演出厅的座位链表
    seat_list_t list;
    List_Init(list, seat_node_t);
    
    // 4. 如果获取到链表，检查座位是否已存在
    if (Seat_Perst_SelectByRoomID(list, data->roomID)) {
        // 5. 检查待添加座位是否已存在
        if (Seat_Srv_FindByRowCol(list, row, column)) {
            printf("This seat was existed!");
            getchar();
            return 0;
        }
    }
    
    // 6. 创建座位节点并赋值
    seat_node_t *newSeat = (seat_node_t*) malloc(sizeof(seat_node_t));
    newSeat->data = *data;
    newSeat->prev = newSeat->next = NULL;
    
    // 7. 将节点添加到链表中
//    Seat_Srv_AddToSoftedList(list, newSeat);
    List_AddTail(list, newSeat);
    
    
    // 8. 调用 SCU_Seat_Perst_Insert
    if (!Seat_Perst_Insert(data)) {
        printf("Add failed!");
        getchar();
        return 0;
    }
    
    // 9. 让演出厅的座位数增加，并更新演出厅的信息
    studio->seatCount++;
    if (!Studio_Perst_Update(studio)) {
        return 0;
    }
    
    // 10. 终于成功了！
    return 1;
}

int Seat_Srv_AddBatch(seat_list_t list) {
    seat_node_t *p;
    List_ForEach(list, p) {
        Seat_Srv_Add(&(p->data));
    }
    return 0;
}


int Seat_Srv_Modify(const seat_t *data) {
    return Seat_Perst_Update(data);
}

int Seat_Srv_DeleteByID(int ID) {
    // 1. 找到座位
    seat_t *pSeat = (seat_t *) malloc(sizeof(seat_t));
    if (!Seat_Srv_FetchByID(ID, pSeat)) {
        return 0;
    }
    
    // 2. 找到演出厅
    int roomID = pSeat->roomID;
    studio_t *pStudio = (studio_t *) malloc(sizeof(studio_t));
    if (!Studio_Srv_FetchByID(roomID, pStudio)) {
        return 0;
    }
    
    // 3. 减少演出厅的座位数
    pStudio->seatCount--;
    if (!Studio_Srv_Modify(pStudio)) {
        return 0;
    }
    
    // 4. 将文件中的座位数据抹去
    return Seat_Perst_DeleteByID(ID);
}


int Seat_Srv_FetchByID(int ID, seat_t *buf) {
    return Seat_Perst_SelectByID(ID, buf);
}


inline int Seat_Srv_DeleteAllByRoomID(int roomID) {
    /*
     本函数可能导致studio的座位数没有减少
     但是本函数只会在删除studio时调用
     所以可以无视风险
     */
    return Seat_Perst_DeleteAllByRoomID(roomID);
}


int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID) {
    return Seat_Perst_SelectByRoomID(list, roomID);
}


int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID) {
    seat_list_t *plist = (seat_list_t *) malloc(sizeof(seat_list_t));
    if (!Seat_Perst_SelectByRoomID(*plist, roomID)) {
        return 0;
    }
    
    seat_node_t * p;
    int count = 0;
    List_ForEach(*plist, p){
        if (p->data.status == SEAT_GOOD) {
            List_AddTail(list, p);
            count++;
        }
    }
    return count;
}


int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount, int colsCount) {
    return Seat_Perst_SelectByRoomID(list, roomID);
}


void Seat_Srv_SortSeatList(seat_list_t *plist) {
    // 1. 先获取链表的长度
    int size = 0, minRow = 100, maxRow = 0;
    seat_node_t *cur;
    List_ForEach(*plist, cur) {
        if (minRow > cur->data.row) {
            minRow = cur->data.row;
        }
        if (maxRow < cur->data.row) {
            maxRow = cur->data.row;
        }
        size++;
    }
    
    // 2. 如果size=0，那么没有元素，不用排序
    if (!size) {
        return;
    }
    
    // 3. 初始化计数链表
    int rowSize = maxRow - minRow + 1;
    seat_list_t countRow[rowSize];
    for (int i = 0; i < rowSize; i++) {
        List_Init(countRow[i], seat_node_t);
    }
    
    // 4. 统计数据
    List_ForEach(*plist, cur) {
        int index = cur->data.row - minRow;
        seat_node_t *innerCur, *prev = NULL;
        List_ForEach(countRow[index], innerCur) {
            if (innerCur->data.column > cur->data.column) {
                if (prev == NULL){
                    List_AddTail(*plist, cur);
                    return;
                }
                
                List_InsertAfter(prev, cur);
                return;
            }
            prev = innerCur;
        }
    }
    
    // 5. 将统计后的数据放入排序后的链表
    seat_list_t sorted = NULL;
    
    List_Init(sorted, seat_node_t);
    for (int i = 0; i < rowSize; i++) {
        List_ForEach(countRow[i], cur) {
            List_AddTail(sorted, cur);
        }
    }
    
    // 6. 让传入的list指向新的sorted
    plist = &sorted;
}


void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t *node) {
    // 有问题，不要使用
    int row, column;
    row = node->data.row;
    column = node->data.column;
    seat_node_t *p, *prev = NULL;
    List_ForEach(list, p){
        if (p->data.row > row) {
            List_ForEach(list, p) {
                if (p->data.column > column) {
                    if(prev == NULL){
                        List_AddTail(list, node);
                        return;
                    }
                    
                    List_InsertAfter(prev, node);
                    return;
                }
                prev = p;
            }
        }
        prev = p;
    }
    
    // 如果链表为空，则prev指向NULL
    if(prev == NULL){
        List_AddTail(list, node);
    }
}


seat_node_t * Seat_Srv_FindByRowCol(seat_list_t list, int row, int column) {
    seat_node_t * p;
    List_ForEach(list, p) {
        if (p->data.row == row && p->data.column == column) {
            return p;
        }
    }
    
    return NULL;
}


seat_node_t * Seat_Srv_FindByID(seat_list_t list, int seatID) {
    seat_node_t * p;
    List_ForEach(list, p){
        if (p->data.id == seatID) {
            return p;
        }
    }
    
    return NULL;
}

