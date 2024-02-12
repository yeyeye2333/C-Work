#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
typedef struct node{
    char cont;
    struct node*next;
}st;
int isempty(st**head)
{
    if(*head==NULL)return 1;
    return 0;
}
void init(st**head)
{
    *head=NULL;
}
int push(st**head,char ch)
{
    st*p;
    if((p=(st*)malloc(sizeof(st)))==NULL)return 0;
    p->cont=ch;
    p->next=(*head);
    (*head)=p;
    return 1;
}
int pop(st**head)
{
    if(isempty(head)==1)return 0;
    st*p=(*head);
    (*head)=(*head)->next;
    free(p);
    return 1;
}
char top(st**head)
{
    if(isempty(head)==1)return 0;
    return (*head)->cont;
}
void clear(st**head)
{
    while(isempty(head)==0)
    {
        pop(head);
    }
}
int longestValidParentheses(char* s) {
    st*stack;
    init(&stack);
    int result=0;
    int tmp=0;
    for(int c=0;c<=strlen(s);c++)
    {
        if(top(&stack)=='('&&s[c]==')')
        {
            pop(&stack);
            tmp+=2;
        }
        else if(top(&stack)==0&&s[c]=='(')
        {
            push(&stack,s[c]);
        }
        else if(top(&stack)==0&&s[c]==')')
        {
            push(&stack,s[c]);
        }
        else if(top(&stack)==')'&&s[c]=='(')
        {
            clear(&stack);
            result=tmp>result?tmp:result;
            tmp=0;
            push(&stack,s[c]);
        }
        else if(top(&stack)=='('&&s[c]=='(')
        {
            push(&stack,s[c]);
        }
    }
    if(tmp!=0)result=tmp>result?tmp:result;
    return result;
}

int main()
{
    char command[1000];
    printf("\e[32mhhh:\e[0m");
    while ((fgets(command,1000,fdopen(STDIN_FILENO,"r")))!=NULL)
    {
        if(command[0]!='\n')
        {
            printf("\n");
        }
        printf("\e[32mhhh:\033[2D\033[1D\e[0m\033[2D");
    }
    return 0;
}