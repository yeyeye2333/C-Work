//
//  Schedule_Persist.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#ifndef Schedule_Persist_h
#define Schedule_Persist_h

#include "../Service/Schedule.h"

// 将data添加到文件中。如果添加失败，则返回0
int Schedule_Perst_Insert(schedule_t *data);

// 根据id将指定演出计划从文件中删除。如果文件中不包含该演出计划，则返回0
int Schedule_Perst_DeleteByID(int id);

// 根据studioID将指定演出厅的所有演出计划从文件中删除。返回删除的演出计划的数量
int Schedule_Perst_DeleteByStduioID(int studioID);

// 根据playID将指定剧目的所有演出计划从文件中删除。返回删除的演出计划的数量
int Schedule_Perst_DeleteByPlayID(int playID);

// 修改data在文件中的记录。如果找不到data的记录，则返回0
int Schedule_Perst_Update(schedule_t *data);

// 根据id将指定演出计划储存到申请空间后的buf中。如果找不到指定演出计划，则返回0
int Schedule_Perst_SelectByID(int id, schedule_t *buf);

// 将所有演出计划储存到初始化后的list中。返回值是list中节点的数量
int Schedule_Perst_SelectAll(schedule_list_t list);

// 根据playID将指定剧目的所有演出计划从文件中加入到list里。返回找到的演出计划的数量
int Schedule_Perst_SelectByPlayID(schedule_list_t list, int playId);

// 根据ticketID将指定票对应的演出计划从文件中缓存到buf里。如果找不到，返回0
int Schedule_Perst_SelectByTicketID(int ticketID, schedule_t *buf);

#endif /* Schedule_Persist_h */
