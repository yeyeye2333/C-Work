#ifndef PLAY_UI_H_
#define PLAY_UI_H_

#include"../Service/Play.h"
int Play_UI_Add(void);//添加新剧目界面 

int Play_UI_Modify(int id);//修改剧目界面
 
int Play_UI_Delete(int id);//删除剧目界面 

int Play_UI_Query(int id);//查询剧目界面 

int Schedule_UI_MgtEntry(int play_id);//安排演出界面 

void Play_UI_MgtEntry(void);//管理剧目界面层 

#endif