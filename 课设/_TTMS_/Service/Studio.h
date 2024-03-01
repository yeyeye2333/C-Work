//
//  Studio.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#ifndef Studio_h
#define Studio_h

typedef struct {
    int id;         // id
    char name[30];  // 名称
    int rowsCount;  // 座位行数
    int colsCount;  // 座位列数
    int seatCount; // 座位数量
} studio_t; // 演出厅类型

typedef struct studio_node {
    studio_t data;              // 储存的数据
    struct studio_node *prev;   // 向前的指针
    struct studio_node *next;   // 向后的指针
} studio_node_t, *studio_list_t; // 演出厅节点类型、演出厅链表类型

/*
 标识符：SCU_Studio_Src_Add
 功能：添加演出厅
 */
int Studio_Srv_Add(studio_t *data);

/*
 标识符：SCU_Studio_Src_Mod
 功能：修改演出厅
 */
int Studio_Srv_Modify(const studio_t *data);

/*
 标识符：SCU_Studio_Srv_DelByID
 功能：根据ID删除演出厅
 */
int Studio_Srv_DeleteByID(int ID);

/*
 标识符：SCU_Studio_Srv_FetchByID
 功能：根据ID获取演出厅
 */
int Studio_Srv_FetchByID(int ID, studio_t *buf);

/*
 标识符：SCU_Studio_Srv_FetchAll
 功能：获取所有演出厅
 */
int Studio_Srv_FetchAll(studio_list_t list);

/*
 标识符：SCU_Studio_Srv_FindByID
 功能：根据ID寻找演出厅
 */
studio_node_t *Studio_Srv_FindByID(studio_list_t list, int ID);

#endif /* Studio_h */
