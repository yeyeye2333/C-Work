//
//  Schedule.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#include "Schedule.h"
#include "../Persistence/Schedule_Persist.h"

int Schedule_Srv_Add(schedule_t *data) {
    return Schedule_Perst_Insert(data);
}

int Schedule_Srv_DeleteByID(int id) {
    return Schedule_Perst_DeleteByID(id);
}

int Schedule_Srv_Modify(schedule_t *data) {
    return Schedule_Perst_Update(data);
}

int Schedule_Srv_FetchByID(int id, schedule_t *buf) {
    return Schedule_Perst_SelectByID(id, buf);
}

int Schedule_Srv_FetchByPlayID(schedule_list_t list, int playID) {
    return Schedule_Perst_SelectByPlayID(list, playID);
}

int Schedule_Srv_FetchAll(schedule_list_t list) {
    return Schedule_Perst_SelectAll(list);
}
