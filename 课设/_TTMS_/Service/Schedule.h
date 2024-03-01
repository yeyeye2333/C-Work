//
//  Schedule.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#ifndef Schedule_h
#define Schedule_h

#include "Structs.h"

typedef struct {
    int id;             // 演出计划ID
    int playID;         // 上映剧目ID
    int studioID;       // 演出厅ID
    usr_date_t date;    // 放映日期
    usr_time_t time;    // 放映时间
    int seatCount;      // 座位数
} schedule_t; // 演出计划类型

typedef struct schedule_node {
    schedule_t data;                // 实体数据
    struct schedule_node *next;     // 后向指针
    struct schedule_node *prev;     // 前向指针
} schedule_node_t, *schedule_list_t; // 演出计划节点类型、演出计划链表类型

// 将data添加到文件中。如果添加失败，则返回0
int Schedule_Srv_Add(schedule_t *data);

// 根据id将指定演出计划从文件中删除。如果文件中不包含该演出计划，则返回0
int Schedule_Srv_DeleteByID(int id);

// 修改data在文件中的记录。如果找不到data的记录，则返回0
int Schedule_Srv_Modify(schedule_t *data);

// 根据id将指定剧目储存到申请空间后的buf中。如果找不到指定演出计划，则返回0
int Schedule_Srv_FetchByID(int id, schedule_t *buf);

// 根据playID将指定剧目的所有演出计划从文件中加入到list里。返回找到的演出计划的数量
int Schedule_Srv_FetchByPlayID(schedule_list_t list, int playID);

// 将所有演出计划储存到初始化后的list中。返回值是list中节点的数量
int Schedule_Srv_FetchAll(schedule_list_t list);

#endif /* Schedule_h */
