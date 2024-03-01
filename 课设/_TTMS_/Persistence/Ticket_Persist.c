//
//  Ticket_Persist.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "EntityKey_Persist.h"
#include "MacroFunc.h"
#include "../Common/List.h"
#include "Ticket_Persist.h"

static const char TICKET_DATA_FILE[] = "Ticket.dat";            // 票文件名常量
static const char TICKET_DATA_TEMP_FILE[] = "TicketTmp.dat";    // 票临时文件名常量
static const char TICKET_KEY_NAME[] = "Ticket";                 // 票名常量

int Ticket_Perst_Insert(ticket_t *data) {
    INSERT(data, TICKET_KEY_NAME, TICKET_DATA_FILE, ticket_t);
}

int Ticket_Perst_DeleteByID(int id) {
    DELETE_BY_ID(id, TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE, ticket_t);
}

int Ticket_Perst_Update(ticket_t *data) {
    UPDATE(data, TICKET_DATA_FILE, ticket_t);
}

int Ticket_Perst_SelectByID(int id, ticket_t *buf) {
    SELECT_BY_ID(id, buf, TICKET_DATA_FILE, ticket_t);
}

int Ticket_Perst_SelectAll(ticket_list_t list) {
    SELECT_ALL(list, TICKET_DATA_FILE, ticket_node_t, ticket_t);
}
