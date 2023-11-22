//the operate for the file of “book”
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#define maxti 40
#define maxau 40
#define maxbk 10
struct book{
    int nodel;
    char title[maxti];
    char author[maxau];
    float value;
    int num;
};
//读取函数，并清理末尾换行符
void s_gets(char*a,int num,FILE*stream);
//查询可添加的位置
long seek(FILE*stream);
int main()
{
    printf("print read|add|delete (q to exit)");
    char operate[2];
    fgets(operate,2,stdin);
    while(getc(stdin)!='\n');
    if(operate[0]=='a'||operate[0]=='A')
    {
        FILE*stream1;
        if((stream1=fopen("book","r+b"))==NULL)exit(EXIT_FAILURE);
        struct book a;
        long sss;
        int before;
        if((sss=seek(stream1))==0)before=0;
        else
        {
            fseek(stream1,sss-sizeof(a),SEEK_SET);
            fread(&a,sizeof(a),1,stream1);
            before=a.num;
        }
        if(before==maxbk)
        {
            printf("book is full");
            return 0;
        }
        fseek(stream1,sss,SEEK_SET);
        a.nodel=1;
        printf("title:");
        s_gets(a.title,maxti,stdin);
        printf("author:");
        s_gets(a.author,maxau,stdin);
        printf("value:");
        scanf("%f",&a.value);
        a.num=before+1;
        fwrite(&a,sizeof(a),1,stream1);
        fclose(stream1);
        printf("complete");
        return 0;
    }
    else if(operate[0]=='r'||operate[0]=='R')
    {
        FILE *stream2;
        if((stream2=fopen("book","r+b"))==NULL)exit(EXIT_FAILURE);
        struct book b;
        rewind(stream2);
        while(fread(&b,sizeof(b),1,stream2)==1)
        {
            if(b.nodel==1)
            printf("%s by %s is %f",b.title,b.author,b.value);
        }
        return 0;
    }
    else if(operate[0]=='d'||operate[0]=='D')
    {
        printf("print title:");
        char rc[maxti];
        s_gets(rc,maxti,stdin);
        FILE*stream3;
        if((stream3=fopen("book","r+b"))==NULL)exit(EXIT_FAILURE);
        rewind(stream3);
        struct book c;
        while(fread(&c,sizeof(c),1,stream3)==1)
        {
            if(strcmp(c.title,rc)==0)
            {
                c.nodel=0;
                fseek(stream3,-sizeof(c),SEEK_CUR);
                fwrite(&c,sizeof(c),1,stream3);
                return 0;
            }
        }
        printf("no this book");
        return 0;
    }
    else printf("exit");
    return 0;
}
//读取函数，并清理末尾换行符
void s_gets(char*a,int num,FILE*stream)
{
    fgets(a,num,stream);
    char *find;
    if((find=strchr(a,'\n'))!=NULL)
    {
        *find='\0';
    }
    else while(getc(stdin)!='\n');
}
//查询可添加的位置
long seek(FILE*stream)
{
    struct book a;
    long rc=0;
    rewind(stream);
    do{
        if(fread(&a,sizeof(struct book),1,stream)!=1)return rc;
        rc=ftell(stream);
    }while(a.nodel==1);
    return ftell(stream)-sizeof(struct book);
}