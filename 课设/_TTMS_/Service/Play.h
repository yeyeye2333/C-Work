//
//  Play.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#ifndef Play_h
#define Play_h

#include "Structs.h"

typedef enum {
    PLAY_TYPE_FILM = 1,     // 电影
    PLAY_TYPE_OPERA = 2,    // 戏剧
    PLAY_TYPE_CONCERT = 3   // 演唱会
} play_type_t; // 剧目类型枚举

typedef enum {
    PLAY_RATE_CHILD = 1,    // 儿童
    PLAY_RATE_TEENAGE = 2,  // 青年
    PLAY_RATE_ADULT = 3     // 成人
} play_rating_t; // 剧目年龄分级美剧

typedef struct{
    int id;                 // ID
    char name[31];          // 名称
    play_type_t type;       // 类型
    char area[9];           // 地区
    play_rating_t rating;   // 年龄分级
    int duration;           // 时长，单位: min
    usr_date_t start_date;  // 起始放映日期
    usr_date_t end_date;    // 终止放映日期
    int price;              // 价格
} play_t; // 剧目类型

typedef struct play_node {
    play_t data;                // 数据
    struct play_node * next;    // 后向指针
    struct play_node * prev;    // 前向指针
} play_node_t, *play_list_t; // 剧目节点类型、剧目链表类型

// 将data添加到文件中。如果添加失败，则返回0
int Play_Srv_Add(play_t *data);

// 根据id将指定剧目从文件中删除。如果文件中不包含该剧目，则返回0
int Play_Src_DeleteByID(int id);

// 修改data在文件中的记录。如果修改失败，则返回0
int Play_Srv_Modify(play_t *data);

// 根据id将指定剧目储存到申请空间后的buf中。如果找不到指定剧目，则返回0
int Play_Srv_FetchByID(int id, play_t *buf);

// 将所有剧目储存到初始化后的list中。返回值是list中节点的数量
int Play_Srv_FetchAll(play_list_t list);

// 根据剧目名称查找剧目，并将剧目放在buf中。如果没有找到，则返回0
int Play_Srv_FetchByName(char *name, play_t *buf);

#endif /* Play_h */
