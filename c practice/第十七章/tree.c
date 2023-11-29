#include"treefunc.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define maxfile 40
int s_gets(char*a,int num,FILE*stream);
void printfile(item*a,int *b);
int main()
{
    char file[maxfile];
    printf("print the file:");
    scanf("%s",file);
    FILE*ff;
    if((ff=fopen(file,"r+"))==NULL)exit(EXIT_FAILURE);
    rewind(ff);
    item tem;
    tree one;
    init(&one);
    while(fscanf(ff,"%s",tem.word)==1)
    {
        if(add(&one,tem)==0)break;
    }
    char choice[2];
    do{
        printf("print ur choice(read or quit)");
        s_gets(choice,2,stdin);
        if(choice[0]=='r')
        {
            traverse(&one,printfile);
        }
    }while(choice[0]=='r');
    if(choice[0]='q')printf("quit\n");
    fclose(ff);
    tradel(&one);
    return 0;
}
int s_gets(char*a,int num,FILE*stream)
{
    char* tt;
    tt=fgets(a,num,stream);
    if(tt==NULL)return 0;
    if(a[0]=='\n')tt=fgets(a,num,stream);
    if(tt==NULL)return 0;
    char *find;
    if((find=strchr(a,'\n'))!=NULL)
    {
        *find='\0';
    }
    else while(getc(stdin)!='\n');
    return 1;
}
void printfile(item*a,int *b)
{
    printf("%s ",a->word);
    printf("%d\n",*b);
}