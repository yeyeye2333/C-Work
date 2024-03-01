//
//  Play_Persist.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#include <stdio.h>
#include <assert.h>
#include "EntityKey_Persist.h"
#include "MacroFunc.h"
#include "Play_Persist.h"
#include "../Common/List.h"

static const char PLAY_DATA_FILE[] = "Play.dat";            // 剧目文件名常量
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";    // 剧目临时文件名常量
static const char PLAY_KEY_NAME[] = "Play";                 // 剧目名常量

int Play_Perst_Insert(play_t *data) {
    INSERT(data, PLAY_KEY_NAME, PLAY_DATA_FILE, play_t);
}

int Play_Perst_DeleteByID(int id) {
    DELETE_BY_ID(id, PLAY_DATA_FILE, PLAY_DATA_TEMP_FILE, play_t);
}

int Play_Perst_Update(play_t *data) {
    UPDATE(data, PLAY_DATA_FILE, play_t);
}

int Play_Perst_SelectByID(int id, play_t *buf) {
    SELECT_BY_ID(id, buf, PLAY_DATA_FILE, play_t);
}

int Play_Perst_SelectAll(play_list_t list) {
    SELECT_ALL(list, PLAY_DATA_FILE, play_node_t, play_t);
}
