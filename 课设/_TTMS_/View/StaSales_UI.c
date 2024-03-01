//
//  StaSales_UI.c
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#include <stdio.h>
#include <time.h>
#include "Common.h"
#include "StaSales_UI.h"
#include "../Service/Structs.h"
#include "../Service/StaSales.h"
#include "../Common/List.h"

// 打印一行日期限制的销售额
void PrintSaleByDate(int userID) {
    usr_date_t start;
    usr_date_t end;
    
    printf("Year of start:");
    readInt(&(start.year));
    printf("Month of start:");
    readInt(&(start.month));
    printf("Day of start:");
    readInt(&(start.day));
    
    printf("Year of end:");
    readInt(&(end.year));
    printf("Month of end:");
    readInt(&(end.month));
    printf("Day of end:");
    readInt(&(end.day));
    
    printf("%8d %12d.%02d.%02d %12d.%02d.%02d %19ld\n",
           userID, start.year, start.month, start.day, end.year, end.month, end.day,
           StaSales_Srv_FecthStaByUserIDAndDate(userID, start, end));
}

// 打印一行本销售员的所有销售额
void PrintAllSale(int userID) {
    usr_date_t end;
    GET_DATE(end.year, end.month, end.day);
    printf("%8d %12d.%02d.%02d %12d.%02d.%02d %19ld\n",
           userID, 1900, 1, 1, end.year, end.month, end.day,
           StaSales_Srv_FecthStaByUserID(userID));
}

//void StaSales_UI_MgtEntry(void) {
//    char choice;
//    do {
//        printf("\n==================================================================\n");
//        printf("************************** Sale Statistics ***********************\n");
//        printf("      [S]elf       |       [C]lerk       |       [R]eturn\n");
//        printf("******************************************************************\n");
//        printf("==================================================================\n");
//        printf("Your choice:");
//        
//        readChar(&choice);
//        switch (choice) {
//            case 's':
//            case 'S':
//                StaSales_UI_Self();
//                break;
//            case 'c':
//            case 'C':
//                StaSales_UI_Clerk();
//                break;
//        }
//    } while ('r' != choice && 'R' != choice);
//}

void StaSales_UI_Self(void) {
    int userID;
    printf("Your user id:");
    readInt(&userID);
    if (userID < 1) {
        printf("Please enter correct uesr id!");
        getchar();
        return;
    }
    
    // 在账户中进行查找，如果找不到，直接返回
    
    char choice = 'a';
    do {
        printf("\n==================================================================\n");
        printf("****************************** Clerk %2d **************************\n", userID);
        printf("%8s %18s %18s %19s\n", "User ID", "Start", "End", "Sale");
        printf("------------------------------------------------------------------\n");
        
        switch (choice) {
            case 'a':
            case 'A':
                PrintAllSale(userID);
                break;
            case 'd':
            case 'D':
                PrintSaleByDate(userID);
                break;
        }
        
        printf("******************************************************************\n");
        printf("    Query By [D]ate     |     Query [A]ll     |     [R]eturn\n");
        printf("==================================================================\n");
        printf("Your choice:");
        readChar(&choice);
    } while ('r' != choice && 'R' != choice);
}

void StaSales_UI_Clerk(void) {
    // 在账户中进行查找经理的id和密码，如果找不到，直接返回
    
    // 找到所有的销售员，储存到一个链表中
//    _list_t sellers;
    
    char choice = 'a';
    do {
        printf("\n==================================================================\n");
//        printf("****************************** Clerk %2d **************************\n", userID);
        printf("%8s %18s %18s %19s\n", "User ID", "Start", "End", "Sale");
        printf("------------------------------------------------------------------\n");
        
        // 遍历所有销售员的链表，循环输出
        // 还需要分页器
//        _node_t *cur;
        switch (choice) {
            case 'a':
            case 'A':
//                List_Init(sellers, cur) {
//                    PrintAllSale(userID);
//                }
                break;
            case 'd':
            case 'D':
//                List_Init(, ) {
//                    PrintSaleByDate(userID);
//                }
                break;
        }
        
        printf("******************************************************************\n");
        printf("    Query By [D]ate     |     Query [A]ll     |     [R]eturn\n");
        printf("==================================================================\n");
        printf("Your choice:");
        readChar(&choice);
    } while ('r' != choice && 'R' != choice);
}
