//
//  Common.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#ifndef Structs_h
#define Structs_h

typedef struct {
    int year;   // 年
    int month;  // 月
    int day;    // 日
} usr_date_t; // 日期类型

typedef struct{
    int hour;   // 时
    int minute; // 分
    int second; // 秒
} usr_time_t; // 时间类型

// 判断date是否在start和end之间
#define IsBewteenDate(date, start, end)                             \
    ((date).year >= start.year && (date).year <= end.year) &&       \
    ((date).month >= start.month && (date).month <= end.month) &&   \
    ((date).day >= start.day && (date).day <= end.day)

// 获取当前日期
#define GET_DATE(year, month, day) {                    \
            time_t timer;                               \
            timer = time(NULL);                         \
            struct tm* plocaltime = gmtime(&timer);     \
            (year) = plocaltime->tm_year + 1900;        \
            (month) = plocaltime->tm_mon + 1;           \
            (day) = plocaltime->tm_mday;                \
        }

#endif /* Common_h */
