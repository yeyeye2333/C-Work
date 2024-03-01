//
//  Sale_Persist.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/29.
//

#ifndef Sale_Persist_h
#define Sale_Persist_h

#include "../Service/Sale.h"

// 将data添加到文件中。如果添加失败，则返回0
int Sale_Perst_Insert(sale_t *data);

// 根据id将指定对象从文件中删除。如果文件中不包含该对象，则返回0
int Sale_Perst_DeleteByID(long id);

// 根据id将指定对象储存到申请空间的到buf中。如果找不到指定对象，则返回0
int Sale_Perst_SelectByID(int id, sale_t *buf);

// 将所有对象储存到初始化后的list中。返回值是list中节点的数量
int Sale_Perst_SelectAll(sale_list_t list);

#endif /* Sale_Persist_h */
