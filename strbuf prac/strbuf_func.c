#include<stdlib.h>
#include "strbuf.h"
#include <stdio.h>
void strbuf_init(struct strbuf*sb,size_t alloc)
{
    sb->alloc=alloc;
    sb->len=0;
    sb->buf=(char*)calloc(alloc,1);
}
void strbuf_attach(struct strbuf*sb,void*str,size_t len,size_t alloc)
{
    sb->buf=(char*)str;
    sb->len=len;
    sb->alloc=alloc;
}
void strbuf_release(struct strbuf *sb)
{
    if(sb->buf!=NULL)free(sb->buf);
    sb->buf=NULL;
    sb->alloc=0;
    sb->len=0;
}
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    struct strbuf temp = *a;
    *a=*b;
    *b=temp;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    *sz=sb->alloc;
    return sb->buf;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    return strcmp(first->buf,second->buf);
}
void strbuf_reset(struct strbuf *sb)
{
    free(sb->buf);
    sb->buf=NULL;
    sb->len=0;
}

void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if(sb->len+extra>=sb->alloc)
    {
        sb->buf=(char*)realloc(sb->buf,sb->len+extra+1);
        sb->alloc=sb->len+extra+1;
    }
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    if(sb->len+len>=sb->alloc)
    {
        strbuf_grow(sb,len);
    }
    //strncat(sb->buf,(char*)data,len);
    memcpy(sb->buf+sb->len,data,len);
    sb->len+=len;
    sb->buf[sb->len]='\0';
}
void strbuf_addch(struct strbuf *sb, int c)
{
    strbuf_add(sb,&c,1);
}
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    strbuf_add(sb,s,strlen(s));
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_add(sb,sb2->buf,sb2->len);
}
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    sb->len=len;
    sb->buf[len]='\0';
}
size_t strbuf_avail(const struct strbuf *sb)
{
    return sb->alloc-sb->len-1;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    sb->buf=(char*)realloc(sb->buf,pos+sb->alloc);
    sb->alloc=pos+sb->alloc;
    int tm;
    char tmp[tm=(sb->len-pos+1)];
    memcpy(tmp,sb->buf+pos,tm);
    sb->len+=len;
    memcpy(sb->buf+pos,data,len);
    memcpy(sb->buf+pos+len,tmp,tm);
}

void strbuf_rtrim(struct strbuf *sb)
{
    int tmp=sb->len/2;
    int tmp2=sb->len;
    for(int c=tmp2/2;c<=tmp2;c++)
    {
        if(sb->buf[tmp]==' '||sb->buf[tmp]=='\t')
        {
            memmove(sb->buf+tmp,sb->buf+tmp+1,sb->len-tmp);
            sb->len--;
        }
        else tmp++;
    }
}
void strbuf_ltrim(struct strbuf *sb)
{
    int tmp=0;
    int tmp2=sb->len;
    for(int c=0;c<tmp2/2;c++)
    {
        if(sb->buf[tmp]==' '||sb->buf[tmp]=='\t')
        {
            memmove(sb->buf+tmp,sb->buf+tmp+1,sb->len-tmp);
            sb->len--;
        }
        else tmp++;
    }
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len+1);
    sb->len-=len;
}

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    strbuf_grow(sb,hint?hint:8192);
    FILE*ff=fdopen(fd,"r");
    char ch;
    while((ch=getc(ff))!=EOF)
    {
        sb->buf[sb->len]=ch;
        sb->len++;
    }
    sb->buf[sb->len]='\0';
    return 0;
}
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    char ch;
    while((ch=getc(fp))!='\n'&&ch!=EOF)
    {
        strbuf_addch(sb,ch);
    }
}

char* _strchr(char*str,int n,int len)
{
    for(int c=0;c<=len;c++)
    {
        if(str[c]==n)return str+c;
    }
    return NULL;
}
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    struct strbuf**a=(struct strbuf**)malloc((max+1)*sizeof(struct strbuf*));
    char cpy[len+1];
    memcpy(cpy,str,len+1);
    char*tmp=NULL;
    char*tmpp=cpy;
    int tmp2=len;
    int c;
    for(c=0;c<max;c++)
    {
        do{
        if(tmp!=NULL)tmpp=tmp+1;
        tmp=_strchr(tmpp,terminator,cpy+len-tmpp);
        if(tmp!=NULL)tmp2=tmp-tmpp;
        else  tmp2=cpy+len-tmpp;
        }while(tmp==tmpp);
        if(tmp==NULL&&tmpp[0]=='\0')break;
        a[c]=(struct strbuf*)malloc(sizeof(struct strbuf));
        strbuf_init(a[c],0);
        strbuf_add(a[c],tmpp,tmp2);
        if(tmp==NULL)
        {
            c++;
            break;
        }
    }
    a[c]=NULL;
    return a;
}
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)
{
    if(strnlen==0&&str==NULL)return true;
    char*tmp;
    if((tmp=strstr(target_str,str))==NULL)return false;
    else if(tmp==target_str)return true;
    else return false;
}
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
{
    if(begin<0||end>len||end<begin)return NULL;
    char*back=(char*)malloc(sizeof(char)*(end-begin+1));
    memcpy(back,target_buf+begin,end-begin);
    back[end-begin]='\0';
    return back;
}