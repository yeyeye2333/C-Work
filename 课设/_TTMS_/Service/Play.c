//
//  Play.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#include <string.h>
#include "Play.h"
#include "../Persistence/Play_Persist.h"
#include "../Common/List.h"

// 判断两个剧目p1和p2是否相同
static int juageEquals(play_t *p1, play_t *p2) {
    // 如果p1和p2的名称都一样的话，那么它们两个相同
    return (!strcmp(p1->name, p2->name));
}

// 判断added是否与剧目链表中的一个相同
static int juageAll(play_t *added) {
    play_list_t playList;
    List_Init(playList, play_node_t);
    if (!Play_Srv_FetchAll(playList)) {
        return 0;
    }
    
    play_node_t *cur;
    List_ForEach(playList, cur) {
        if (juageEquals(added, &(cur->data))) {
            return 1;
        }
    }
    return 0;
}

int Play_Srv_Add(play_t *data) {
    if (juageAll(data)) {
        return 0;
    }
    return Play_Perst_Insert(data);
}

int Play_Src_DeleteByID(int id) {
    return Play_Perst_DeleteByID(id);
}

int Play_Srv_Modify(play_t *data) {
    return Play_Perst_Update(data);
}

int Play_Srv_FetchByID(int id, play_t *buf) {
    return Play_Perst_SelectByID(id, buf);
}

int Play_Srv_FetchAll(play_list_t list) {
    return Play_Perst_SelectAll(list);
}

int Play_Srv_FetchByName(char *name, play_t *buf) {
    play_list_t plays;
    List_Init(plays, play_node_t);
    if (!Play_Srv_FetchAll(plays)) {
        return 0;
    }
    
    play_node_t *cur;
    List_ForEach(plays, cur) {
        if (!strcmp(name, cur->data.name)) {
            *buf = cur->data;
            return 1;
        }
    }
    
    return 0;
}
