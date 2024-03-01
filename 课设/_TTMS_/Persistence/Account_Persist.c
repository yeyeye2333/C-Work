//
//  Account_Persist.c
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#include <stdio.h>
#include <assert.h>
#include "EntityKey_Persist.h"
#include "MacroFunc.h"
#include "Account_Persist.h"
#include "../Common/List.h"

static const char DATA_FILE[] = "Account.dat";            // 文件名常量
static const char DATA_TEMP_FILE[] = "AccountTmp.dat";    // 临时文件名常量
static const char KEY_NAME[] = "Account";                 // 名称常量

int Account_Perst_Insert(account_t *data) {
    INSERT(data, KEY_NAME, DATA_FILE, account_t);
}

int Account_Perst_DeleteByID(int id) {
    DELETE_BY_ID(id, DATA_FILE, DATA_TEMP_FILE, account_t);
}

int Account_Perst_Update(account_t *data) {
    UPDATE(data, DATA_FILE, account_t);
}

int Account_Perst_SelectByID(int id, account_t *buf) {
    SELECT_BY_ID(id, buf, DATA_FILE, account_t);
}

int Account_Perst_SelectAll(account_list_t list) {
    SELECT_ALL(list, DATA_FILE, account_node_t, account_t);
}
