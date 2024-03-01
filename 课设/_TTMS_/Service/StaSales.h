//
//  StaSales.h
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#ifndef StaSales_h
#define StaSales_h

#include "Structs.h"

// 根据userID获取销售额统计
long StaSales_Srv_FecthStaByUserID(int userID);

// 根据userID和始末日期获取销售额统计
long StaSales_Srv_FecthStaByUserIDAndDate(int userID, usr_date_t start, usr_date_t end);

#endif /* StaSales_h */
