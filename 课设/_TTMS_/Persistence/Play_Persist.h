//
//  Play_Persist.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#ifndef Play_Persist_h
#define Play_Persist_h

#include "../Service/Play.h"

// 将data添加到文件中。如果添加失败，则返回0
int Play_Perst_Insert(play_t *data);

// 根据id将指定剧目从文件中删除。如果文件中不包含该剧目，则返回0
int Play_Perst_DeleteByID(int id);

// 修改data在文件中的记录。如果修改失败，则返回0
int Play_Perst_Update(play_t *data);

// 根据id将指定剧目储存到申请空间的到buf中。如果找不到指定剧目，则返回0
int Play_Perst_SelectByID(int id, play_t *buf);

// 将所有剧目储存到初始化后的list中。返回值是list中节点的数量
int Play_Perst_SelectAll(play_list_t list);

#endif /* Play_Persist_h */
