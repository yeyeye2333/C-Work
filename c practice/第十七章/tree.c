#include"treefunc.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define maxfile 40
int s_gets(char*a,int num,FILE*stream);
void printfile(item*a,int *b);
static void operate(node*p,void(*func)(item*a,int*b));
static void deltree(node*p);
static int cmp(item a,item b);
static int delnode(node**chi);
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
        add(&one,tem);
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
    if(choice[0]='q')printf("quit");
    fclose(ff);
    tradel(&one);
    return 0;
}
int s_gets(char*a,int num,FILE*stream)
{
    char* tt;
    tt=fgets(a,num,stream);
    if(tt==NULL)return 0;
    if(a[0]='\n')tt=fgets(a,num,stream);
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
//判断树是否满了
int isfull(tree *input)
{
    return input->curnum<maxcu?0:1;
}
//判断树是否为空
int isempty(tree *input)
{
    return input->curnum==0?1:0;
}
//初始化一个树
void init(tree*input)
{
    input->head=NULL;
    input->curnum=0;
}
//比较函数
static int cmp(item a,item b)
{
    return strncmp(a.word,b.word,maxwd);
}
//增加item
int add(tree* input,item target)
{
    node *p1,**p2;
    if(input->head==NULL)
    {
        input->head=(node*)malloc(sizeof(node));
        input->head->content=target;
        input->head->left=NULL;
        input->head->right=NULL;
        input->head->num=1;
    }
    else
    {
        p1=(node*)malloc(sizeof(node));
        p1->content=target;
        p1->left=NULL;
        p1->right=NULL;
        p1->num=1;
        p2=&(input->head);
        int tem;
        while((*p2)!=NULL)
        {
            if((tem=cmp((*p2)->content,p1->content))>0)
            {
                p2=&((*p2)->left);
            }
            if(tem<0)
            {
                p2=&((*p2)->right);
            }
            else
            {
                free(p1);
                (*p2)->num++;
                return 1;
            }
        }
        (*p2)=p1;
    }
    input->curnum++;
    return 1;
}
//删除node
static int delnode(node**chi)
{
    node*p1;
    node**p2;
    node*p;
    if((*chi)->left==NULL)
    {
        p=*chi;
        (*chi)=(*chi)->right;
        free(p);
    }
    else
    {
        p1=(*chi)->right;
        p2=&((*chi)->left);
        while((*p2)!=NULL)
        {
            p2=&((*p2)->right);
        }
        (*p2)->right=p1;
        p=*chi;
        (*chi)=(*chi)->left;
        free(p);
    }
    
}
//删除item
int del(tree *input,item target)
{
    node*p2;
    if(isempty(input))return 0;
    p2=input->head;
    int tem;
    while(p2!=NULL)
    {
        if((tem=cmp(p2->content,target))>0)
        {
            p2=p2->left;
        }
        if(tem<0)
        {
            p2=p2->right;
        }
        else
        {
            delnode(&p2);
            input->curnum--;
            return 1;
        }
    }
    return 0;
}
//遍历树
int traverse(tree*input,void(*func)(item*a,int*b))
{
    if(input!=NULL)
    operate(input->head,func);
    return 1;
}
static void operate(node*p,void(*func)(item*a,int*b))
{
    if(p!=NULL)
    {
        operate(p->left,func);
        func(&(p->content),&(p->num));
        operate(p->right,func);
    }
}
//清空树
int tradel(tree*input)
{
    if(input!=NULL)
    {
        deltree(input->head);
        input->curnum=0;
        input->head=NULL;
    }
    return 1;
}
static void deltree(node*p)
{
    node*p1;
    if(p!=NULL)
    {
        deltree(p->left);
        p1=p->right;
        free(p);
        deltree(p1);
    }
}
