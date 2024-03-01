//
//  Sale_Persist.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/29.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "EntityKey_Persist.h"
#include "MacroFunc.h"
#include "../Common/List.h"
#include "Sale_Persist.h"

static const char DATA_FILE[] = "Sale.dat";            // 文件名常量
static const char DATA_TEMP_FILE[] = "SaleTmp.dat";    // 临时文件名常量
static const char KEY_NAME[] = "Sale";                 // 名称常量

int Sale_Perst_Insert(sale_t *data) {
    INSERT(data, KEY_NAME, DATA_FILE, sale_t);
}

int Sale_Perst_DeleteByID(long id) {
    DELETE_BY_ID(id, DATA_FILE, DATA_TEMP_FILE, sale_t);
}

int Sale_Perst_SelectByID(int id, sale_t *buf) {
    SELECT_BY_ID(id, buf, DATA_FILE, sale_t);
}

int Sale_Perst_SelectAll(sale_list_t list) {
    SELECT_ALL(list, DATA_FILE, sale_node_t, sale_t);
}
