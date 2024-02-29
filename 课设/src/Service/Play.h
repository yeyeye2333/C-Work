#ifndef PLAY_H_
#define PLAY_H_
typedef enum{
	PLAY_TYPE_FILM=1,
	PLAY_TYPE_OPEAR=2,
	PLAY_TYPE_CONCERT=3
}play_type_t; 
typedef enum{
	PLAY_RETE_CHILD=1,
	PLAY_RATE_TEENAGE=2,
	PALY_RATE_ADULT=3
}play_rating_t;
typedef struct{
	int year;
	int month;
	int day;
}ttms_date_t;
typedef struct{
	int  id;     //剧目ID
	char  name[31] ;   //剧目名称
	play_type_t  type;   //剧目类型
	char  area[9];    //剧目出品地区
	play_rating_t  rating;    //剧目等级
	int  duration;   //时长，以分钟为单位 
	ttms_date_t  start_date;  //开始放映日期
	ttms_date_t  end_date;  //放映结束日期
	int  price;  //建议票价 
	}play_t;
typedef struct play_node{
	play_t data;  //实体数据
	struct play_node*next;  //后向指针 
	struct play_node*prev;  //前项指针 
}play_node_t,*play_list_t;
int Play_Srv_FetchAll(play_list_t list);  //获取全部剧目
int Play_Srv_Add(play_t*data);  //储存新剧目 
int Play_Srv_FetchByID(int id,play_t*buf);  //根据ID查询剧目 
int Play_Srv_Modify(const play_t*data);  //修改剧目
int Play_Src_DeleteByID(int id);  //删除剧目 
#endif
