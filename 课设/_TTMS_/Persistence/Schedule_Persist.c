//
//  Schedule_Persist.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#include <stdio.h>
#include <assert.h>
#include "EntityKey_Persist.h"
#include "MacroFunc.h"
#include "Schedule_Persist.h"
#include "../Common/List.h"

static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";            // 演出计划文件名常量
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";    // 演出计划临时文件名常量
static const char SCHEDULE_KEY_NAME[] = "Schedule";                 // 演出计划名常量

int Schedule_Perst_Insert(schedule_t *data) {
    INSERT(data, SCHEDULE_KEY_NAME, SCHEDULE_DATA_FILE, schedule_t);
}

int Schedule_Perst_DeleteByID(int id) {
    DELETE_BY_ID(id, SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE, schedule_t);
}

int Schedule_Perst_DeleteByStduioID(int studioID) {
    schedule_list_t scheduleList;
    List_Init(scheduleList, schedule_node_t);
    if (!Schedule_Perst_SelectAll(scheduleList)) {
        return 0;
    }
    
    int count = 0;
    schedule_node_t *cur;
    List_ForEach(scheduleList, cur) {
        if (studioID == cur->data.studioID) {
            Schedule_Perst_DeleteByID(cur->data.id);
            count++;
        }
    }
    return count;
}

int Schedule_Perst_DeleteByPlayID(int playID) {
    schedule_list_t scheduleList;
    List_Init(scheduleList, schedule_node_t);
    if (!Schedule_Perst_SelectAll(scheduleList)) {
        return 0;
    }
    
    int count = 0;
    schedule_node_t *cur;
    List_ForEach(scheduleList, cur) {
        if (playID == cur->data.playID) {
            Schedule_Perst_DeleteByID(cur->data.id);
            count++;
        }
    }
    return count;
}

int Schedule_Perst_Update(schedule_t *data) {
    assert(NULL != data);
    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");
    if (NULL == fp) {
        printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
        return 0;
    }
    schedule_t buf;                                                 \
    int found = 0;                                              \
    while (!feof(fp)) {                                         \
        if (fread(&buf, sizeof(schedule_t), 1, fp)) {               \
            if (buf.id == data->id) {                           \
                fseek(fp, -((int)sizeof(schedule_t)), SEEK_CUR);    \
                fwrite(data, sizeof(schedule_t), 1, fp);            \
                found = 1;                                      \
                break;                                          \
            }                                                   \
        }                                                       \
    }                                                           \
    fclose(fp);                                                 \
    return found;                                               \
}

int Schedule_Perst_SelectByID(int id, schedule_t *buf) {
    SELECT_BY_ID(id, buf, SCHEDULE_DATA_FILE, schedule_t);
}

int Schedule_Perst_SelectAll(schedule_list_t list) {
    SELECT_ALL(list, SCHEDULE_DATA_FILE, schedule_node_t, schedule_t);
}

int Schedule_Perst_SelectByPlayID(schedule_list_t list, int playID) {
    schedule_list_t scheduleList;
    List_Init(scheduleList, schedule_node_t);
    if (!Schedule_Perst_SelectAll(scheduleList)) {
        return 0;
    }
    
    int count = 0;
    schedule_node_t *cur;
    List_ForEach(scheduleList, cur) {
        if (playID == cur->data.playID) {
            schedule_node_t *newNode = new(schedule_node_t);
            *newNode = *cur;
            List_AddTail(list, newNode);
            count++;
        }
    }
    return count;
}

int Schedule_Perst_SelectByTicketID(int ticketID, schedule_t *buf) {
    // 无法实现，因为找不到ticketID与schedule的关系
    return 0;
}
