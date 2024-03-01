//
//  Studio_Persist.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#ifndef Studio_Persist_h
#define Studio_Persist_h

#include "../Service/Studio.h"

//标识符：TTMS_SCU_Studio_Perst_Insert
//功能：向文件中存储新演出厅
int Studio_Perst_Insert(studio_t *data);

//标识符：TTMS_SCU_Studio_ Perst_Update
//功能：在文件中更新演出厅
int Studio_Perst_Update(const studio_t *data);

//标识符：TTMS_SCU_Studio_Perst_DeleteByID
//功能：在文件中删除演出厅
int Studio_Perst_DeleteByID(int ID);

//标识符：TTMS_SCU_Studio_Perst_SelectByID
//功能：根据ID从文件中载入演出厅
int Studio_Perst_SelectByID(int ID, studio_t *buf);

//标识符：TTMS_SCU_Studio_Perst_SelAll
//功能：从文件中载入全部演出厅
int Studio_Perst_SelectAll(studio_list_t list);

#endif /* Studio_Persist_h */
