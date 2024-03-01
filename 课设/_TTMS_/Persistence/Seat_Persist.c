//
//  Seat_Persist.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#include <stdio.h>
#include <assert.h>
#include "Seat_Persist.h"
#include "EntityKey_Persist.h"
#include "../Common/List.h"


static const char SEAT_DATA_FILE[] = "Seat.dat";            // 座位文件名常量
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";    // 座位临时文件名常量
static const char SEAT_KEY_NAME[] = "Seat";                 // 座位名常量

/*
函数功能：用于向文件中添加一个新座位数据。
参数说明：data为seat_t类型指针，表示需要添加的座位数据结点。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_Perst_Insert(seat_t *data) {
    assert(NULL!=data);

    long key = EntKey_Perst_GetNewKeys(SEAT_KEY_NAME, 1); //为新座位分配获取
    if (key <= 0)            //主键分配失败，直接返回
        return 0;
    data->id = (int) key;        //赋给新对象带回到UI层

    FILE *fp = fopen(SEAT_DATA_FILE, "ab");
    int rtn = 0;
    if (NULL == fp) {
        printf("Cannot open file %s!\n", SEAT_DATA_FILE);
        return 0;
    }

    rtn = (int) fwrite(data, sizeof(seat_t), 1, fp);

    fclose(fp);
    return rtn;
}

/*
标识符：TTMS_SCU_Seat_Perst_InsertBatch
函数功能：用于向文件中添加一批座位数据。
参数说明：list为seat_list_t类型，表示需要添加的一批座位的链表头指针。
返 回 值：整型，表示成功添加一批座位的个数。
*/
int Seat_Perst_InsertBatch(seat_list_t list) {
    assert(NULL!=list);
    
    seat_node_t *p;
    int count = 0;
    List_ForEach(list, p) {
        Seat_Perst_Insert(&(p->data));
        count++;
    }
    
    return count;
}

/*
标识符：TTMS_SCU_Seat_Perst_Update
函数功能：用于在文件中更新一个座位数据。
参数说明：data为seat_t类型指针，表示需要更新的座位数据结点。
返 回 值：整型，表示是否成功更新了座位的标志。
*/
int Seat_Perst_Update(const seat_t *data) {
    assert(NULL!=data);

    FILE *fp = fopen(SEAT_DATA_FILE, "rb+");
    if (NULL == fp) {
        printf("Cannot open file %s!\n", SEAT_DATA_FILE);
        return 0;
    }

    seat_t buf;
    int found = 0;

    while (!feof(fp)) {
        if (fread(&buf, sizeof(seat_t), 1, fp)) {
            if (buf.id == data->id) {
                fseek(fp, -((int)sizeof(seat_t)), SEEK_CUR);
                fwrite(data, sizeof(seat_t), 1, fp);
                found = 1;
                break;
            }

        }
    }
    fclose(fp);

    return found;
}

/*
识符：TTMS_SCU_Seat_Perst_DelByID
函数功能：用于从文件中删除一个座位的数据。
参数说明：参数ID为整型，表示需要删除的座位ID。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Perst_DeleteByID(int ID) {
    if (rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE) < 0) {
        printf("Cannot open file %s!\n", SEAT_DATA_FILE);
        return 0;
    }

    FILE *fpSour, *fpTarg;
    fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
    if (NULL == fpSour) {
        printf("Cannot open file %s!\n", SEAT_DATA_FILE);
        return 0;
    }

    fpTarg = fopen(SEAT_DATA_FILE, "wb");
    if ( NULL == fpTarg) {
        printf("Cannot open file %s!\n", SEAT_DATA_TEMP_FILE);
        return 0;
    }

    seat_t buf;

    int found = 0;
    while (!feof(fpSour)) {
        if (fread(&buf, sizeof(seat_t), 1, fpSour)) {
            if (ID == buf.id) {
                found = 1;
                continue;
            }
            fwrite(&buf, sizeof(seat_t), 1, fpTarg);
        }
    }

    fclose(fpTarg);
    fclose(fpSour);

    //删除临时文件
    remove(SEAT_DATA_TEMP_FILE);
    return found;
}

/*
标识符：TTMS_SCU_Seat_Perst_DelAllByID
函数功能：根据编号用于从文件中删除座位数据。
参数说明：参数roomID为整型，表示演出厅ID。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Perst_DeleteAllByRoomID(int roomID) {
    seat_list_t list = NULL;
    List_Init(list, seat_node_t);
    if (!Seat_Perst_SelectByRoomID(list, roomID)) {
        printf("Cannot find room %d!\n", roomID);
        return 0;
    }
    
    int count = 0;
    seat_node_t *p;
    List_ForEach(list, p) {
        
        Seat_Perst_DeleteByID(p->data.id);
        count++;
    }
    return count;
}

/*
标识符：TTMS_SCU_Studio_Perst_SelByID
函数功能：用于从文件中载入一个座位的数据。
参数说明：第一个参数ID为整型，表示需要载入数据的座位ID；第二个参数buf为seat_t指针，指向载入座位数据的指针。
返 回 值：整型，表示是否成功载入了座位的标志。
*/
int Seat_Perst_SelectByID(int ID, seat_t *buf) {
    assert(NULL!=buf);

    FILE *fp = fopen(SEAT_DATA_FILE, "rb");
    if (NULL == fp) {
        printf("Can not find file %s!\n", SEAT_DATA_FILE);
        return 0;
    }

    seat_t data;
    int found = 0;

    while (!feof(fp)) {
        if (fread(&data, sizeof(seat_t), 1, fp)) {
            if (ID == data.id) {
                *buf = data;
                found = 1;
                break;
            };

        }
    }
    fclose(fp);

    return found;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelAll
函数功能：用于从文件中载入所有座位数据。
参数说明：list为seat_list_t类型，表示将要载入的座位链表头指针。
返 回 值：整型，成功载入座位的个数。
*/
int Seat_Perst_SelectAll(seat_list_t list) {
    seat_node_t *newNode;
    seat_t data;
    int recCount = 0;

    assert(NULL!=list);

    List_Free(list, seat_node_t);

    FILE *fp = fopen(SEAT_DATA_FILE, "rb");
    if (NULL == fp) {
        printf("Can not find file %s!\n", SEAT_DATA_FILE);
        return 0;
    }

    while (!feof(fp)) {
        if (fread(&data, sizeof(seat_t), 1, fp)) {
            newNode = (seat_node_t*) malloc(sizeof(seat_node_t));
            if (!newNode) {
                printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                break;
            }
            newNode->data = data;
            List_AddTail(list, newNode);
            recCount++;
        }
    }
    fclose(fp);
    
    return recCount;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelByRoomID
函数功能：用于在文件中根据演出厅ID载入所有座位数据。
参数说明：第一个参数list为seat_list_t类型，表示将要载入的座位链表头指针，第二个参数roomID为整型，表示演出厅ID。
返 回 值：整型，表示成功载入了演出厅座位的个数。
*/
int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {
    FILE *fp = fopen(SEAT_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }
    
    seat_list_t seatList;
    List_Init(seatList, seat_node_t);
    if (!Seat_Perst_SelectAll(seatList)) {
        printf("Can not find room %d!\n", roomID);
        return 0;
    }
    
    int count = 0;
    while (!feof(fp)) {
        seat_t data;
        if (fread(&data, sizeof(seat_t), 1, fp)) {
            seat_node_t *newNode = (seat_node_t*) malloc(sizeof(seat_node_t));
            if (!newNode) {
                printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                break;
            }
            newNode->data = data;
            if(roomID == data.roomID){
                List_AddTail(list, newNode);
            }
            count++;
        }
    }
    fclose(fp);
    
    return count;
}
