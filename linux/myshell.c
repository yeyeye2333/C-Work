//预设不支持如">a >b"一类的连续重定向操作
//支持的重定向操作有"< > 1> >> 1>> 2> 2>> &> &>>"，且只能在管道前后端使用输入和输出的重点向
//重定向与命令、参数间应有空格分开
#define _POSIX_C_SOURCE 199309L
#include<stdio.h>
#include<signal.h>
#include<limits.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
struct strbuf {
    int len;
    int alloc;
    char *buf;
};
int built_in(char*lastdir,char**theargv);
void onecommand(char*onecommand,char*lastdir,int in,int out,int err);
void redir(char*onecommand,int*in,int*out,int*err);
void mysystem(char*command,char*lastdir);
void strbuf_grow(struct strbuf *sb, size_t extra);
void strbuf_init(struct strbuf*sb,size_t alloc);
void strbuf_release(struct strbuf *sb);
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
char* _strchr(char*str,int n,int len);
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max);
sigset_t set,oldset;
static pid_t bg=0;
int main()
{
    sigemptyset(&set);
    sigaddset(&set,SIGTSTP);
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);
    if(sigprocmask(SIG_BLOCK,&set,&oldset)==-1)fprintf(stderr,"errno:sigprocmask\n");
    char lastdir[PATH_MAX];
    char cwdir[PATH_MAX];
    getcwd(lastdir,PATH_MAX);
    char command[1000];
    printf("\e[32m%s:\e[0m",getcwd(cwdir,PATH_MAX));
    while ((fgets(command,1000,fdopen(STDIN_FILENO,"r")))!=NULL)
    {
        if(command[0]!='\n')
        {
            mysystem(command,lastdir);
            printf("\n");
        }
        printf("\e[32m%s:\e[0m",getcwd(cwdir,PATH_MAX));
    }
    return 0;
}
int built_in(char*lastdir,char**theargv)
{
    if(strlen(theargv[0])==2&&theargv[0][0]=='c'&&theargv[0][1]=='d')
    {
        char cwdir[PATH_MAX];
        getcwd(cwdir,PATH_MAX);
        if(theargv[1]!=NULL)
        {
            if(strlen(theargv[1])==1&&theargv[1][0]=='-')
            {
                if(chdir(lastdir)!=-1)memcpy(lastdir,cwdir,PATH_MAX);
            }
            else
            {
                if(chdir(theargv[1])!=-1)memcpy(lastdir,cwdir,PATH_MAX);
            }
        }
    }
    else return 1;
    return 0;
}
void onecommand(char*onecommand,char*lastdir,int in,int out,int err)
{
    struct strbuf**strs=strbuf_split_buf(onecommand,strlen(onecommand),' ',strlen(onecommand));
    int count=0;
    pid_t childpid;
    int n=0;
    for(;strs[count]!=NULL;count++)
    {
        n++;
        for(int c=0;strs[count]->buf[c]!='\0';c++)
        {
            if(strs[count]->buf[c]=='>'||strs[count]->buf[c]=='<')
            {
                n--;
                if(strs[count+1]!=NULL)n--;
                break;
            }
        }
    }
    char**theargv=(char**)malloc(sizeof(char*)*(n+1));
    n=0;
    for(int c=0;c<count;c++)
    {
        for(int cc=0;strs[c]->buf[cc]!='\0';cc++)
        {
            if(strs[c]->buf[cc]=='>'||strs[c]->buf[cc]=='<')
            {
                if(strs[c+1]!=NULL)c++;
                goto here;
            }
        }
        theargv[n++]=strs[c]->buf;
        here:;
    }
    theargv[n]=NULL;
    int ini_in=dup(STDIN_FILENO);
    int ini_out=dup(STDOUT_FILENO);
    int ini_err=dup(STDERR_FILENO);
    if(in>0)dup2(in,STDIN_FILENO);
    if(out>0)dup2(out,STDOUT_FILENO);
    if(err>0)dup2(err,STDERR_FILENO);
    if(built_in(lastdir,theargv))
    {
        switch((childpid=fork()))
        {
            case -1:
            fprintf(fdopen(STDERR_FILENO,"w"),"errno:fork\n");
                return ;
            case 0:
                if(sigprocmask(SIG_SETMASK,&oldset,NULL)==-1)
                    fprintf(fdopen(STDERR_FILENO,"w"),"errno:sigprocmask\n");
                if(bg=1)
                {
                    bg=getpid();
                    setpgid(0,bg);
                }
                else if(bg>1)setpgid(0,bg);
                execvp(theargv[0],theargv);
                fprintf(fdopen(STDERR_FILENO,"w"),"errno:execvp\n");
                break;
            default:
                break;
        }
    }
    
    //还原
    if(in>0)dup2(ini_in,STDIN_FILENO);
    if(out>0)dup2(ini_out,STDOUT_FILENO);
    if(err>0)dup2(ini_err,STDERR_FILENO);
    close(ini_in);
    close(ini_out);
    close(ini_err);
    //释放内存
    for(int c=0;c<count;c++)
    {
        strbuf_release(strs[c]);
        free(strs[c]);
    }
    free(strs);
    free(theargv);
}
void redir(char*onecommand,int*in,int*out,int*err)
{
    struct strbuf**strs=strbuf_split_buf(onecommand,strlen(onecommand),' ',strlen(onecommand));
    char*instr=NULL,*outstr=NULL,*errstr=NULL;
    int in_index=-1,out_index=-1,err_index=-1,a_out_index=-1,a_err_index=-1;
    int n=0;
    if(in!=NULL)
    {
        for(int c=0;strs[c]!=NULL;c++)
        {
            if(strs[c]->buf[0]=='<'&&strs[c]->buf[1]=='\0'&&strs[c+1]!=NULL)
            {
                in_index=c+1;
                n++;
                break;
            }
        }
    }
    if(out!=NULL)
    {
        for(int c=0;strs[c]!=NULL;c++)
        {
            if(((strs[c]->buf[0]=='>'&&strs[c]->buf[1]=='\0')||
                (strs[c]->buf[0]=='1'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='\0')||
                (strs[c]->buf[0]=='&'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='\0'))
                &&strs[c+1]!=NULL)
            {
                out_index=c+1;
                n++;
                break;
            }
            else if(((strs[c]->buf[0]=='>'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='\0')||
                (strs[c]->buf[0]=='1'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='>'&&strs[c]->buf[3]=='\0')||
                (strs[c]->buf[0]=='&'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='>'&&strs[c]->buf[3]=='\0'))
                &&strs[c+1]!=NULL)
            {
                a_out_index=c+1;
                n++;
                break;
            }
        }
    }
    if(err!=NULL)
    {
        for(int c=0;strs[c]!=NULL;c++)
        {
            if(((strs[c]->buf[0]=='2'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='\0')||
                (strs[c]->buf[0]=='&'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='\0'))
                &&strs[c+1]!=NULL)
            {
                err_index=c+1;
                n++;
                break;
            }
            else if(((strs[c]->buf[0]=='2'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='>'&&strs[c]->buf[3]=='\0')||
                (strs[c]->buf[0]=='&'&&strs[c]->buf[1]=='>'&&strs[c]->buf[2]=='>'&&strs[c]->buf[3]=='\0'))
                &&strs[c+1]!=NULL)
            {
                a_err_index=c+1;
                n++;
                break;
            }
        }
    }
    if(in_index>0)
    {
        *in=open(strs[in_index]->buf,O_RDONLY);
    }
    if(out_index>0)
    {
        *out=open(strs[out_index]->buf,O_WRONLY|O_CREAT,0666);
    }
    else if(a_out_index>0)
    {
        *out=open(strs[a_out_index]->buf,O_WRONLY|O_APPEND|O_CREAT,0666);
    }
    if(err_index>0)
    {
        *err=open(strs[err_index]->buf,O_WRONLY|O_CREAT,0666);
    }
    else if(a_err_index>0)
    {
        *err=open(strs[a_err_index]->buf,O_WRONLY|O_APPEND|O_CREAT,0666);
    }

    //释放内存
    for(int c=0;strs[c]!=NULL;c++)
    {
        strbuf_release(strs[c]);
        free(strs[c]);
    }
    free(strs);
}
void mysystem(char*command,char*lastdir)
{
    char*chr;
    if((chr=strchr(command,'&'))!=NULL)
    {
        *chr='\0';
        bg=1;
    }//处理&
    else if((chr=strchr(command,'\n'))!=NULL)*chr='\0';//消去换行符
    struct strbuf**strs=strbuf_split_buf(command,strlen(command),'|',strlen(command));
    int count=0;
    for(;strs[count]!=NULL;count++)
        ;
    int pfd[count-1][2];
    for(int c=0;c<count-1;c++)
        if(pipe(pfd[c])==-1)
        {
            fprintf(fdopen(STDERR_FILENO,"w"),"errno:pipe");
            return ;
        }
    //处理第一个命令的标准输入重定向
    int in=-1;
    redir(strs[0]->buf,&in,NULL,NULL);
    //处理最后一个命令的输出重定向
    int out=-1;
    int err=-1;
    redir(strs[count-1]->buf,NULL,&out,&err);
    
    if(count==0)goto here;
    for(int c=0;c<count;c++)
    {
        if(c==0&&count==1)onecommand(strs[c]->buf,lastdir,in,out,err);
        else if(c==0)onecommand(strs[c]->buf,lastdir,in,pfd[c][1],-1);
        else if(c==count-1)onecommand(strs[c]->buf,lastdir,pfd[c-1][0],out,err);
        else onecommand(strs[c]->buf,lastdir,pfd[c-1][1],pfd[c][1],-1);
    }
    if(bg=0)
    {
        while(wait(NULL)!=-1||errno!=ECHILD);
    }

    here:
    //关闭管道、重定向
    for(int c=0;c<count-1;c++)
    {
        close(pfd[c][0]);
        close(pfd[c][1]);
    }
    if(in>0)close(in);
    if(out>0)close(out);
    if(err>0)close(err);
    //释放内存
    for(int c=0;c<count;c++)
    {
        strbuf_release(strs[c]);
        free(strs[c]);
    }
    free(strs);
    bg=0;
}

void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if(sb->len+extra>=sb->alloc)
    {
        sb->buf=(char*)realloc(sb->buf,sb->len+extra+1);
        sb->alloc=sb->len+extra+1;
    }
}
void strbuf_init(struct strbuf*sb,size_t alloc)
{
    sb->alloc=alloc;
    sb->len=0;
    sb->buf=(char*)calloc(alloc,1);
}
void strbuf_release(struct strbuf *sb)
{
    if(sb->buf!=NULL)free(sb->buf);
    sb->buf=NULL;
    sb->alloc=0;
    sb->len=0;
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