//
//  Play_UI.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#include <stdio.h>
#include <stdlib.h>
#include "Common.h"
#include "Play_UI.h"
#include "../Service/Play.h"
#include "../Common/List.h"

static const int PLAY_PAGE_SIZE = 5; // 每页显示剧目的个数

// 将字符转化为电影类型
static play_type_t charToType(char type) {
    play_type_t res = PLAY_TYPE_FILM;
    switch (type) {
        case 'f':
        case 'F':
            break;
        case 'o':
        case 'O':
            res = PLAY_TYPE_OPERA;
            break;
        case 'c':
        case 'C':
            res = PLAY_TYPE_CONCERT;
            break;
    }
    return res;
}

// 将字符转化为电影年龄分级
static play_rating_t charToRating(char rating) {
    play_rating_t res = PLAY_RATE_CHILD;
    switch (rating) {
        case 'c':
        case 'C':
            break;
        case 't':
        case 'T':
            res = PLAY_RATE_TEENAGE;
            break;
        case 'a':
        case 'A':
            res = PLAY_RATE_ADULT;
            break;
    }
    return res;
}

// 将电影年龄分级转化为字符串
static char* ratingToStr(play_rating_t rate) {
    char *res = (char *) malloc(8 * sizeof(char));
    switch (rate) {
        case PLAY_RATE_CHILD:
            res = "child";
            break;
        case PLAY_RATE_TEENAGE:
            res = "teenage";
            break;
        case PLAY_RATE_ADULT:
            res = "adult";
            break;
        default:
            res = "NULL";
    }
    return res;
}

// 将电影类型转化为字符串
static char* typeToStr(play_type_t type) {
    char *res = (char *) malloc(8 * sizeof(char));
    switch (type) {
        case PLAY_TYPE_FILM:
            res = "film";
            break;
        case PLAY_TYPE_OPERA:
            res = "opera";
            break;
        case PLAY_TYPE_CONCERT:
            res = "concert";
            break;
        default:
            res = "NULL";
    }
    return res;
}

// 打印play_t的属性
static void Print_Field(void) {
    printf("%10s %20s %10s %10s %10s %10s %6s %20s\n", "ID", "Name", "Rating", "Type", "Duration",
           "Area","Price","Start  --  End");
}

// 打印cur的信息
static void Print_Info(play_t *cur) {
    printf("%10d %20s %10s %10s %10d %10s %6d %7d.%02d.%02d——%4d.%02d.%02d\n", cur->id, cur->name, 
           ratingToStr(cur->rating), typeToStr(cur->type), cur->duration, cur->area, cur->price,
           cur->start_date.year, cur->start_date.month, cur->start_date.day,
           cur->end_date.year, cur->end_date.month, cur->end_date.day);
}

// 打印主菜单
static void MgtEntry_Print(play_list_t head, Pagination_t paging) {
    printf("\n============================================================================================================\n");
    printf("******************************************* Projection Play List *******************************************\n");
    Print_Field();
    printf("------------------------------------------------------------------------------------------------------------\n");
    
    int i;
    play_node_t *cur;
    Paging_ViewPage_ForEach(head, paging, play_node_t, cur, i) {
        Print_Info(&(cur->data));
    }

    printf("------------------- Total Records:%2d ------------------------------------------- Page %2d/%2d ----------------\n",
            paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
    printf("************************************************************************************************************\n");
    printf("     [P]revPage    |    [N]extPage    |    [A]dd    |    [D]elete    |    [U]pdate    |    [R]eturn");
    printf("\n============================================================================================================\n");
    printf("Your Choice:");
    fflush(stdin);
}

// 打印选择类型的界面
static void Print_Type(void) {
    printf("\n=======================================================\n");
    printf("\t\t[F]ilm \t  |\t  [O]pera \t|\t [C]oncert\n");
    printf("-------------------------------------------------------\n");
    printf("Type of play:");
}

// 打印选择年龄分级的界面
static void Print_Rate(void) {
    printf("\n=======================================================\n");
    printf("\t  [C]hild \t|\t  [T]eenage \t|\t [A]dult\n");
    printf("-------------------------------------------------------\n");
    printf("Type of play:");
}

void Play_UI_MgtEntry(void) {
    Pagination_t paging;
    paging.offset = 0;
    paging.pageSize = PLAY_PAGE_SIZE;

    char choice;
    do {
        play_list_t playList;
        List_Init(playList, play_node_t);
        paging.totalRecords = Play_Srv_FetchAll(playList); // 获取剧目的总数
        List_Paging(playList, paging, play_node_t);
        
        MgtEntry_Print(playList, paging);
        
        readChar(&choice);
        
        switch (choice) {
            case 'a':
            case 'A':
                Play_UI_Add();
                break;
            case 'd':
            case 'D':
                Play_UI_Delete();
                break;
            case 'u':
            case 'U':
                Play_UI_Modify();
                break;
            case 'p':
            case 'P':
                if (!Pageing_IsFirstPage(paging)) {
                    Paging_Locate_OffsetPage(playList, paging, -1, play_node_t);
                }
                break;
            case 'n':
            case 'N':
                if (!Pageing_IsLastPage(paging)) {
                    Paging_Locate_OffsetPage(playList, paging, 1, play_node_t);
                }
                break;
        }
        List_Destroy(playList, play_node_t);
    } while (choice != 'r' && choice != 'R');
}

void Play_UI_Add(void) {
    play_t *added = (play_t *) malloc(sizeof(play_t));
    char type;              // 类型
    char rating;            // 年龄分级
    
    printf("Name of play:");
    readStr(added->name, 31);
    
    Print_Type();
    readChar(&type);
    added->type = charToType(type);
    
    printf("Area of play:");
    readStr(added->area, 9);
    
    Print_Rate();
    readChar(&rating);
    added->rating = charToRating(rating);
    
    printf("Duration of play:");
    readInt(&(added->duration));
    
    printf("Year of start_date:");
    readInt(&(added->start_date.year));
    printf("Month of start_date:");
    readInt(&(added->start_date.month));
    printf("Day of start_date:");
    readInt(&(added->start_date.day));
    printf("Year of end_date:");
    readInt(&(added->end_date.year));
    printf("Month of end_date:");
    readInt(&(added->end_date.month));
    printf("Day of end_date:");
    readInt(&(added->end_date.day));
    
    printf("Price of play:");
    readInt(&(added->price));
    
    printf("Add %s!", (Play_Srv_Add(added) ? "sucessfully" : "failed"));
    getchar();
}

void Play_UI_Delete(void) {
    int id;
    printf("Id of deleted play:");
    readInt(&id);
    
    printf("Delete %s!", (Play_Src_DeleteByID(id) ? "sucessfully" : "failed"));
    getchar();
}

void Play_UI_Modify(void) {
    // 1. 获取ID
    int id;
    printf("Id of modified play:");
    readInt(&id);
    
    // 2. 获取待修改的play
    play_t *modified = (play_t *) malloc(sizeof(play_t));
    if (!Play_Srv_FetchByID(id, modified)) {
        printf("Cannot find play %d!", id);
        getchar();
        return;
    }
    
    // 3. 进行修改
    printf("Name of play[%s]:", modified->name);
    readStr(modified->name, 31);
    
    char type;
    Print_Type();
    readChar(&type);
    modified->type = charToType(type);
    
    printf("Area of play[%s]:", modified->area);
    readStr(modified->area, 9);
    
    char rating;
    Print_Rate();
    readChar(&rating);
    modified->rating = charToRating(rating);
    
    printf("Duration of play[%d]:", modified->duration);
    readInt(&(modified->duration));
    
    printf("Year of start_date[%d]:", modified->start_date.year);
    readInt(&(modified->start_date.year));
    printf("Month of start_date[%d]:", modified->start_date.month);
    readInt(&(modified->start_date.month));
    printf("Day of start_date[%d]:", modified->start_date.day);
    readInt(&(modified->start_date.day));
    printf("Year of end_date[%d]:", modified->end_date.year);
    readInt(&(modified->end_date.year));
    printf("Month of end_date[%d]:", modified->end_date.month);
    readInt(&(modified->end_date.month));
    printf("Day of end_date[%d]:", modified->end_date.day);
    readInt(&(modified->end_date.day));
    
    printf("Price of play[%d¥]:", modified->price);
    readInt(&(modified->price));
    
    // 4. 调用服务层的函数进行文件中数据的修改
    printf("Update %s!", (Play_Srv_Modify(modified) ? "sucessfully" : "failed"));
    getchar();
}
