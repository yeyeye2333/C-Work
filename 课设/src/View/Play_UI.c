#include"Play_UI.h"
#include"../Common/List.h"
#include<stdio.h>
#include"../Service/Play.h"
#include<string.h>
static char*rating(int a)
{
    char*tmp=(char*)malloc(sizeof(char)*8);
    if(a=1)memcpy(tmp,"child",6);
    if(a=2)memcpy(tmp,"teenage",8);
    if(a=3)memcpy(tmp,"adult",6);
    return tmp;
}
static char*type(int a)
{
    char*tmp=(char*)malloc(sizeof(char)*8);
    if(a=1)memcpy(tmp,"film",5);
    if(a=2)memcpy(tmp,"opear",6);
    if(a=3)memcpy(tmp,"concert",8);
    return tmp;
}
static void MgtEntry_Print(play_node_t*pos)
{
    printf(
				"\n===========================================================================\n");
		printf(
				"********************** Projection Play List *******************************\n");
		printf("->%10s  %25s  %10s  %10s  %10s\n", "ID", "Name", "Rating","Type", "Duration");
		printf(
				"---------------------------------------------------------------------------\n");
        char*tmp1,*tmp2;
        printf("  %10d  %25s  %10s  %10s  %10d\n", pos->data.id,pos->data.name,
            tmp1=rating(pos->data.rating),tmp2=type(pos->data.type),pos->data.duration);
        free(tmp1);free(tmp2);
        printf(
			    "---------------------------------------------------------------------------\n");
        printf("->%10s  %25s  %32s","Area","Price","Start——End");
        printf(
				"\n---------------------------------------------------------------------------\n");
 		printf("  %10s  %25d  %17d.%2d.%2d——%4d.%2d.%2d",pos->data.area,pos->data.price,
            pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
            pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
	
        printf(
				"\n***************************************************************************\n");
		printf(
				"   [P]revPage| [N]extPage | [A]dd | [D]elete | [U]pdate | [R]eturn");
		printf(
				"\n===========================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
}
int main()
{
    play_node_t list;
    list.data.area[0]='a';//6
    list.data.duration=1;//5
    list.data.end_date.day=0;
    list.data.end_date.month=0;//9
    list.data.end_date.year=0;
    list.data.id=0;//1
    list.data.name[0]='b';//2
    list.data.price=1;//7
    list.data.rating=PLAY_RATE_TEENAGE;//3 zh
    list.data.type=PLAY_TYPE_CONCERT;//4 zh
    list.data.start_date.day=1;
    list.data.start_date.month=1;//8
    list.data.start_date.year=1;
    MgtEntry_Print(&list);
}
