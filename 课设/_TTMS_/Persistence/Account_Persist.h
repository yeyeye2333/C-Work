//
//  Account_Persist.h
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#ifndef Account_Persist_h
#define Account_Persist_h

#include "../Service/Account.h"

int Account_Perst_Insert(account_t *data);

int Account_Perst_DeleteByID(int id);

int Account_Perst_Update(account_t *data);

int Account_Perst_SelectByID(int id, account_t *buf);

int Account_Perst_SelectAll(account_list_t list);

#endif /* Account_Persist_h */
