//
//  StaSales.c
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#include <stdio.h>
#include "StaSales.h"
#include "Sale.h"
#include "../Common/List.h"

long StaSales_Srv_FecthStaByUserID(int userID) {
    sale_list_t sales = NULL;
    List_Init(sales, sale_node_t);
    if (!Sale_Srv_FetchByUserID(userID, sales)) {
        return 0L;
    }
    
    long res = 0L;
    sale_node_t *cur;
    List_ForEach(sales, cur) {
        if (cur->data.type == SALE_SELL) {
            res += cur->data.price;
        }
    }
    return res;
}

long StaSales_Srv_FecthStaByUserIDAndDate(int userID, usr_date_t start, usr_date_t end) {
    sale_list_t sales = NULL;
    List_Init(sales, sale_node_t);
    if (!Sale_Srv_FetchByUserID(userID, sales)) {
        return 0L;
    }
    
    long res = 0L;
    sale_node_t *cur;
    List_ForEach(sales, cur) {
        if (cur->data.type == SALE_SELL) {
            if (IsBewteenDate(cur->data.date, start, end)) {
                res += cur->data.price;
            }
        }
    }
    return res;
}
