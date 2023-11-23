#ifndef maxwd
#define maxwd 40
#define maxcu 10
typedef struct{
    char word[maxwd];
}item;
typedef struct Node{
    item content;
    int num;
    struct  Node *left;
    struct Node*right;
}node;
typedef struct{
    node*head;
    int curnum;
}tree;
//判断树是否满了
int isfull(tree *input);
//判断树是否为空
int isempty(tree *input);
//初始化一个树
void init(tree*input);
//增加item
int add(tree* input,item target);
//删除item
int del(tree *input,item target);
//遍历树
int traverse(tree*input,void(*func)(item*a,int*b));
//清空树
int tradel(tree*input);
#endif