//
//  Ticket_Persist.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#ifndef Ticket_Persist_h
#define Ticket_Persist_h

#include "../Service/Ticket.h"

// 将data添加到文件中。如果添加失败，则返回0
int Ticket_Perst_Insert(ticket_t *data);

// 根据id将指定对象从文件中删除。如果文件中不包含该对象，则返回0
int Ticket_Perst_DeleteByID(int id);

// 修改data在文件中的记录。如果修改失败，则返回0
int Ticket_Perst_Update(ticket_t *data);

// 根据id将指定对象储存到申请空间的到buf中。如果找不到指定对象，则返回0
int Ticket_Perst_SelectByID(int id, ticket_t *buf);

// 将所有对象储存到初始化后的list中。返回值是list中节点的数量
int Ticket_Perst_SelectAll(ticket_list_t list);

#endif /* Ticket_Persist_h */
