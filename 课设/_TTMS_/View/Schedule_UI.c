//
//  Schedule_UI.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#include <stdio.h>
#include "Common.h"
#include "Schedule_UI.h"
#include "Ticket_UI.h"
#include "../Service/Studio.h"
#include "../Service/Play.h"
#include "../Service/Schedule.h"
#include "../Common/List.h"

static const int SCHEDULE_PAGE_SIZE = 5;

void Schedule_UI_MgtEntry(void) {
    Pagination_t paging;
    paging.offset = 0;
    paging.pageSize = SCHEDULE_PAGE_SIZE;
    
    char choice;
    do {
        schedule_list_t list;
        List_Init(list, schedule_node_t);
        paging.totalRecords = Schedule_Srv_FetchAll(list);
        List_Paging(list, paging, schedule_node_t);
        
        printf("\n==================================================================\n");
        printf("********************** Schedule Management ***********************\n");
        printf("%5s %10s %11s %24s %12s\n", "ID", "Play ID", "Studio ID", "Show Time", "Seat Count");
        printf("------------------------------------------------------------------\n");
        
        int i;
        schedule_node_t *cur;
        Paging_ViewPage_ForEach(list, paging, schedule_node_t, cur, i) {
            printf("%5d %10d %11d %9d-%02d-%02d-%02d:%02d:%02d %12d\n", cur->data.id,
                   cur->data.playID, cur->data.studioID, cur->data.date.year,
                   cur->data.date.month, cur->data.date.day, cur->data.time.hour,
                   cur->data.time.minute, cur->data.time.second, cur->data.seatCount);
        }
        
        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ------\n",
                paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage|[N]extPage| [A]dd|[D]elete|[U]pdate| [T]icket |[R]eturn\n");
        printf("==================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 'a':
            case 'A':
                Schedule_UI_Add();
                break;
            case 'd':
            case 'D':
                Schedule_UI_Delete();
                break;
            case 'u':
            case 'U':
                Schedule_UI_Modify();
                break;
            case 't':
            case 'T':
            {
                int scheduleID;
                printf("ID of schedule:");
                readInt(&scheduleID);
                Ticket_UI_MgtEntry(scheduleID);
            }
                break;
            case 'p':
            case 'P':
                if (!Pageing_IsFirstPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
                }
                break;
            case 'n':
            case 'N':
                if (!Pageing_IsLastPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
                }
                break;
        }
        List_Destroy(list, schedule_node_t);
    } while ('r' != choice && 'R' != choice);
}


void Schedule_UI_Add(void) {
    // 1. 获取剧目ID
    int playID;
    printf("Id of play:");
    readInt(&playID);
    
    // 2. 根据playID寻找剧目
    play_t *play = new(play_t);
    if (!Play_Srv_FetchByID(playID, play)) {
        printf("Cannot find play %d!", playID);
        getchar();
        return;
    }
    
    // 3. 获取演出厅ID
    int studioID;
    printf("Id of studio:");
    readInt(&studioID);
    
    // 4. 根据studioID寻找演出厅
    studio_t *studio = new(studio_t);
    if (!Studio_Srv_FetchByID(studioID, studio)) {
        printf("Cannot find studio %d!", studioID);
        getchar();
        return;
    }
    
    // 5. 获取放映日期和放映时间
    int year, month, day, hour, minute, second;
    printf("Year of perform:");
    readInt(&year);
    printf("Month of perform:");
    readInt(&month);
    printf("Day of perform:");
    readInt(&day);
    printf("Hour of perform:");
    readInt(&hour);
    printf("Minute of perform:");
    readInt(&minute);
    printf("Second of perform:");
    readInt(&second);
    
    // 6. 创建一个变量用来存储新的演出计划
    schedule_t *added = new(schedule_t);
    added->playID = playID;
    added->studioID = studioID;
    added->date.year = year;
    added->date.month = month;
    added->date.day = day;
    added->time.hour = hour;
    added->time.minute = minute;
    added->time.second = second;
    added->seatCount = studio->seatCount;
    
    // 7. 将新的演出计划加入到文件中
    printf("Add %s!", (Schedule_Srv_Add(added) ? "sucessfully" : "failed"));
    getchar();
}

void Schedule_UI_Delete(void) {
    // 1. 获取待删除演出计划的ID
    int id;
    printf("Id of schedule:");
    readInt(&id);
    
    // 2. 根据ID在文件中删除演出计划
    printf("Delete %s!", (Schedule_Srv_DeleteByID(id) ? "sucessfully" : "failed"));
    getchar();
}

void Schedule_UI_Modify(void) {
    // 1. 获取待修改的演出计划的ID
    int id;
    printf("Id of modified schedule:");
    readInt(&id);
    
    // 2. 找到文件中的演出计划
    schedule_t *original = new(schedule_t);
    if (!Schedule_Srv_FetchByID(id, original)) {
        printf("Cannot find schedule %d!", id);
        getchar();
        return;
    }
    
    // 3. 获取剧目ID
    int playID;
    printf("Id of play[%d]:", original->playID);
    readInt(&playID);
    
    // 4. 根据playID寻找剧目
    play_t *play = new(play_t);
    if (!Play_Srv_FetchByID(playID, play)) {
        printf("Cannot find play %d!", playID);
        getchar();
        return;
    }
    
    // 5. 获取演出厅ID
    int studioID;
    printf("Id of studio[%d]:", original->studioID);
    readInt(&studioID);
    
    // 6. 根据studioID寻找演出厅
    studio_t *studio = new(studio_t);
    if (!Studio_Srv_FetchByID(studioID, studio)) {
        printf("Cannot find studio %d!", studioID);
        getchar();
        return;
    }
    
    // 7. 获取放映日期和放映时间
    int year, month, day, hour, minute, second;
    printf("Year of perform[%d]:", original->date.year);
    readInt(&year);
    printf("Month of perform[%d]:", original->date.month);
    readInt(&month);
    printf("Day of perform[%d]:", original->date.day);
    readInt(&day);
    printf("Hour of perform[%d]:", original->time.hour);
    readInt(&hour);
    printf("Minute of perform[%d]:", original->time.minute);
    readInt(&minute);
    printf("Second of perform[%d]:", original->time.second);
    readInt(&second);
    
    // 6. 创建一个变量用来存储修改后的演出计划
    schedule_t *modified = new(schedule_t);
    modified->id = original->id;
    modified->playID = playID;
    modified->studioID = studioID;
    modified->date.year = year;
    modified->date.month = month;
    modified->date.day = day;
    modified->time.hour = hour;
    modified->time.minute = minute;
    modified->time.second = second;
    modified->seatCount = studio->seatCount;
    
    // 7. 修改文件中的演出计划
    printf("Update %s!", (Schedule_Srv_Modify(modified) ? "sucessfully" : "failed"));
    getchar();
}

void Schedule_UI_Query(void) {
    char *playName = (char *) malloc(20 * sizeof(char));
    printf("Name of play:");
    readStr(playName, 20);
    
    play_t *play = new(play_t);
    if (!Play_Srv_FetchByName(playName, play)) {
        printf("Cannot find %s!", playName);
        getchar();
        return;
    }
    
    Pagination_t paging;
    paging.offset = 0;
    paging.pageSize = SCHEDULE_PAGE_SIZE;
    
    char choice;
    do {
        schedule_list_t schedules = NULL;
        List_Init(schedules, schedule_node_t);
        paging.totalRecords = Schedule_Srv_FetchByPlayID(schedules, play->id);
        List_Paging(schedules, paging, schedule_node_t);
        
        printf("\n=================================================================================\n");
        printf("******************** %20s's Schedules ***************************\n", playName);
        printf("%11s %30s %23s %14s\n", "Schedule ID", "Studio Name", "Show Time", "Seat Count");
        printf("---------------------------------------------------------------------------------\n");
        
        int i;
        schedule_node_t *cur;
        Paging_ViewPage_ForEach(schedules, paging, schedule_node_t, cur, i) {
            studio_t *studio = new(studio_t);
            Studio_Srv_FetchByID(cur->data.studioID, studio);
            printf("%11d %30s %8d-%02d-%02d-%02d:%02d:%02d %14d\n", cur->data.id,
                   studio->name, cur->data.date.year, cur->data.date.month,
                   cur->data.date.day, cur->data.time.hour, cur->data.time.minute,
                   cur->data.time.second, cur->data.seatCount);
        }
        
        printf("------------ Total Records:%2d ---------------------------- Page %2d/%2d -----------\n",
                paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("*********************************************************************************\n");
        printf("     [P]revPage         |         [N]extPage         |         [R]eturn\n");
        printf("=================================================================================\n");
        printf("Your choice:");
        
        readChar(&choice);
        switch (choice) {
            case 'p':
            case 'P':
                PREV_PAGE(schedules, paging, schedule_node_t);
                break;
            case 'n':
            case 'N':
                NEXT_PAGE(schedules, paging, schedule_node_t);
                break;
        }
    } while ('r' != choice && 'R' != choice);
}
