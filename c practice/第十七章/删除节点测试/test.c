#include"treefunc.h"
#include<stdio.h>
void printfile(item*a,int *b);
int main()
{
    item a,b;
    int c;
    for(c=0;c<7;c++)
    {
        a.word[c]='a';
    }
    a.word[c]='\0';
    for(c=0;c<7;c++)
    {
        b.word[c]='b';
    }
    b.word[c]='\0';
    tree two;
    init(&two);
    add(&two,a);
    add(&two,b);
    traverse(&two,printfile);
    del(&two,a);
    traverse(&two,printfile);
    tradel(&two);
    return 0;
}
void printfile(item*a,int *b)
{
    printf("%s ",a->word);
    printf("%d\n",*b);
}
