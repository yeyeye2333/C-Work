//
//  Account.c
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#include "Account.h"
#include "../Persistence/Account_Persist.h"

int Account_Srv_Add(account_t *data) {
    return Account_Perst_Insert(data);
}

int Account_Src_DeleteByID(int id) {
    return Account_Perst_DeleteByID(id);
}

int Account_Srv_Modify(account_t *data) {
    return Account_Perst_Update(data);
}

int Account_Srv_FetchByID(int id, account_t *buf) {
    return Account_Perst_SelectByID(id, buf);
}

int Account_Srv_FetchAll(account_list_t list) {
    return Account_Perst_SelectAll(list);
}
