//
//  Seat_UI.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#include <stdio.h>
#include <stdlib.h>
#include "Seat_UI.h"
#include "../Service/Studio.h"
#include "../Common/List.h"

inline char Seat_UI_Status2Char(seat_status_t status) {
    char res = ' '; // 默认返回值为空字符，代表空座位
    switch (status) {
        case SEAT_NONE:
            break;
        case SEAT_GOOD:
            res = '#';
            break;
        case SEAT_BROKEN:
            res = '$';
            break;
    }
    return res;
}

inline seat_status_t Seat_UI_Char2Status(char statusChar) {
    seat_status_t res = SEAT_NONE; // 默认为空座位
    switch (statusChar) {
        case 'n':
        case 'N':
            break;
        case 'g':
        case 'G':
            res = SEAT_GOOD;
            break;
        case 'b':
        case 'B':
            res = SEAT_BROKEN;
            break;
    }
    return res;
}

void Seat_UI_MgtEntry(int roomID) {
    char choice;
    do {
        // 获取演出厅的信息
        studio_t *pStudio = (studio_t *) malloc(sizeof(studio_t));
        if (!Studio_Srv_FetchByID(roomID, pStudio)) {
            printf("Can not find room %d.\n", roomID);
            getchar();
            return;
        }
        
        printf("\n==================================================================\n");
        printf("************************ Room %d Seat List ************************\n", roomID);
        printf("------------------------------------------------------------------\n");
        
        // 获取座位的信息
        seat_list_t seatList = NULL;
        List_Init(seatList, seat_node_t);
        Seat_Srv_FetchByRoomID(seatList, roomID);

        // 打印座位的信息
        // 打印列数
        printf(" ");
        for (int i = 0; i < pStudio->colsCount; i++) {
            printf("%4d", (i + 1));
        }
        printf("\n");
        // 打印行数和座位字符
        for (int i = 1; i <= pStudio->rowsCount; i++) {
            printf("%d", i);
            for (int j = 1; j <= pStudio->colsCount; j++) {
                seat_node_t *pSeat = NULL;
                int flag = 0;
                List_ForEach(seatList, pSeat) {
                    if (pSeat->data.row == i && pSeat->data.column == j) {
                        flag = 1;
                        printf("%4c", Seat_UI_Status2Char(pSeat->data.status));
                        break;
                    }
                }
                if (!flag) {
                    printf("    ");
                }
            }
            printf("\n");
        }
        
        printf("-----------------%2d row --%2d col --%3d totalSeat------------------\n", pStudio->rowsCount, pStudio->colsCount, pStudio->seatCount);
        printf("******************************************************************\n");
        printf("[A]dd    |      [D]elete      |      [U]pdate      |      [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        fflush(stdin);
        scanf("%c", &choice);
        fflush(stdin);

        // 如果选择是退出，则直接退出do-while循环
        // 由于增删改都需要row和column，而退出不需要，所以不在while的循环条件中判断是否退出循环
        if (choice == 'r' || choice == 'R') {
            break;
        }
        
        // 获取待操作对象的行数和列数
        printf("Row of Seats:");
        int row = 0;
        scanf("%d", &row);
        fflush(stdin);
        printf("Column of Seats:");
        int column = 0;
        scanf("%d", &column);
        fflush(stdin);
        
        // 检查row和column是否在演出厅内
        if (row > pStudio->rowsCount || column > pStudio->colsCount) {
            printf("Please enter sutiable row and column!");
            getchar();
            continue;
        }
        
        switch (choice) {
            case 'a':
            case 'A':
                if (!Seat_UI_Add(seatList, roomID, row, column)) {
                    printf("Add failed!");
                } else {
                    printf("Add sucessfully!");
                }
                getchar();
                break;
            case 'd':
            case 'D':
                if (!Seat_UI_Delete(seatList, row, column)) {
                    printf("Delete failed!");
                } else {
                    printf("Delete sucessfully!");
                }
                getchar();
                break;
            case 'u':
            case 'U':
                if (!Seat_UI_Modify(seatList, row, column)) {
                    printf("Update failed!");
                } else {
                    printf("Updata sucessfully!");
                }
                getchar();
                break;
        }
        List_Destroy(seatList, seat_node_t);
    } while (1);
}

int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {
    // 1. 根据行列找到座位
    seat_node_t *pSeat = Seat_Srv_FindByRowCol(list, row, column);
    
    // 2. 判断是否是空座位
    if (pSeat != NULL) {
        return 0;
    }
    
    // 3. 对座位的信息进行赋值
    pSeat = (seat_node_t *) malloc(sizeof(seat_node_t));
    pSeat->data.column = column;
    pSeat->data.row = row;
    pSeat->data.status = SEAT_GOOD;
    pSeat->data.roomID = roomID;
 
    // 4. 将座位添加到文件中
    if (!Seat_Srv_Add(&(pSeat->data))) {
        return 0;
    }

    // 5. 成功
    return 1;
}

int Seat_UI_Modify(seat_list_t list, int row, int column) {
    // 1. 根据行列号确定要修改的座位
    seat_node_t *pSeat = Seat_Srv_FindByRowCol(list, row, column);
    
    // 2. 检查pSeat是否为空
    if (pSeat == NULL) {
        printf("No seat in %d row, %d column!", row, column);
        getchar();
        return 0;
    }
    
    // 3. 确定修改的数据
    char status;
    printf("\n=======================================================\n");
    printf("[G]ood | [B]reak | [N]one\n\n");
    printf("The seat's situation is %c.\n", Seat_UI_Status2Char(pSeat->data.status));
    printf("-------------------------------------------------------\n");
    printf("Please choose your choice:");
    scanf("%c", &status);
    getchar();
    
    // 4. 进行修改
    pSeat->data.status = Seat_UI_Char2Status(status);
    return Seat_Srv_Modify(&(pSeat->data));
}

int Seat_UI_Delete(seat_list_t list, int row, int column) {
    // 1. 根据行列号确定要删除的座位
    seat_node_t *pSeat = Seat_Srv_FindByRowCol(list, row, column);
    
    // 2. 检查pSeat是否为空
    if (pSeat == NULL) {
        printf("No seat in %d row, %d column!", row, column);
        getchar();
        return 0;
    }
    
    // 3. 进行删除
    return Seat_Srv_DeleteByID(pSeat->data.id);
}
