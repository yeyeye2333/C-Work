#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<ctype.h>
#include<libgen.h>
#include<errno.h>
#ifndef __USE_XOPEN_EXTENDED
#define __USE_XOPEN_EXTENDED
#endif
#define _XOPEN_SOURCE 500
#include<unistd.h>
#include<ftw.h>
#ifndef __USE_XOPEN2K8
#define __USE_XOPEN2K8
#endif
#include<dirent.h>
void noR(const char*pathname);
void output(char*pathname);
int myselect(const struct dirent *a);
int _cmp(const struct dirent **a, const struct dirent **b);
int r_cmp(const struct dirent **a, const struct dirent **b);
int t_cmp(const struct dirent **a, const struct dirent **b);
int rt_cmp(const struct dirent **a, const struct dirent **b);
int func(const char*pathname,const struct stat*statbuf,int type,struct FTW*ftwbuf);
static _Bool _a,_l,_R,_t,_r,_i,_s;
int main(int argc,char**argv)
{
    int tmp=argc-1;
    for(int c=1;c<argc;c++)//记选项
    {
        if(argv[c][0]=='-')
        {
            for(int cc=1;cc<strlen(argv[c]);cc++)
                switch(argv[c][cc])
                {
                    case 'a': _a=1;break;
                    case 'l': _l=1;break;
                    case 'R': _R=1;break;
                    case 't': _t=1;break;
                    case 'r': _r=1;break;
                    case 'i': _i=1;break;
                    case 's': _s=1;break;
                    default:
                        break;
                }
            tmp--;
        }
    }
    char name[tmp][pathconf("/",_PC_PATH_MAX)];
    tmp=0;
    for(int c=1;c<argc;c++)//记名字
    {
        if(argv[c][0]!='-')
        {
            if(strlen(argv[c])>pathconf("/",_PC_PATH_MAX))
                printf("the filename too long:%s",argv[c]);
            else memcpy(name[tmp++],argv[c],strlen(argv[c])+1);
        }
    }
    struct stat test;
    errno=0;
    if(tmp==0&&_R==0)noR(".");
    else if(tmp==0&&_R==1)nftw(".",func,20,FTW_PHYS);
    else for(int c=0;c<tmp;c++)
    {
        if(lstat(name[c],&test)!=0)
        {
            printf("myls:无权限/文件不存在:%s\n",name[c]);
            continue;
        }
        if(_R==0&&((test.st_mode&__S_IFMT)!=__S_IFDIR))
        {
            output(name[c]);
        }
        else if(_R==0)noR(name[c]);
        else if(nftw(name[c],func,20,FTW_PHYS)==-1)
        {
            printf("errno:nftw %d",errno);
        }
    }
    return 0;
}
void noR(const char*pathname)
{
    DIR*tmp=opendir(pathname);
    struct dirent**all=NULL;
    int count=0;
    if(_a==1&&_t==1&&_r==1)
    count=scandir(pathname,&all,NULL,rt_cmp);
    else if(_a==1&&_t==1)
        count=scandir(pathname,&all,NULL,t_cmp);
    else if(_a==1&&_r==1)
        count=scandir(pathname,&all,NULL,r_cmp);
    else if(_t==1&&_r==1)
        count=scandir(pathname,&all,myselect,rt_cmp);
    else if(_a==1)
        count=scandir(pathname,&all,NULL,_cmp);
    else if(_t==1)
        count=scandir(pathname,&all,myselect,t_cmp);
    else if(_r==1)
        count=scandir(pathname,&all,myselect,r_cmp);
    else 
        count=scandir(pathname,&all,myselect,_cmp);
    for(int c=0;c<count;c++)
    {
        char add[strlen(all[c]->d_name)+strlen(pathname)+2];
        memcpy(add,pathname,strlen(pathname));
        add[strlen(pathname)]='/';
        memcpy(add+strlen(pathname)+1,all[c]->d_name,strlen(all[c]->d_name)+1);
        output(add);
        free(all[c]);
    }
    free(all);
    closedir(tmp);
    printf("\n");
}
void output(char*pathname)
{
    struct stat tmp;
    char base[strlen(basename(pathname))+1];
    memcpy(base,basename(pathname),strlen(basename(pathname))+1);
    if(lstat(pathname,&tmp)==-1)
    {
        if(_l==1)printf("myls: 无法打开 '%s': 权限不够\n",pathname);
        else printf("myls: 无法打开 '%s': 权限不够 \t",pathname);
        return ;
    }
    char file[11]={"??????????"};
    switch(tmp.st_mode&__S_IFMT)
    {
        case __S_IFREG:file[0]='-';break;
        case __S_IFDIR:file[0]='d';break;
        case __S_IFCHR:file[0]='c';break;
        case __S_IFBLK:file[0]='b';break;
        case __S_IFIFO:file[0]='p';break;
        case __S_IFSOCK:file[0]='s';break;
        case __S_IFLNK:file[0]='l';break;
        default:
            break;
    }
    file[1]=tmp.st_mode&0400?'r':'-';
    file[2]=tmp.st_mode&0200?'w':'-';
    file[3]=tmp.st_mode&04100?'s':(tmp.st_mode&04000?'S':(tmp.st_mode&0100?'x':'-'));
    file[4]=tmp.st_mode&040?'r':'-';
    file[5]=tmp.st_mode&020?'w':'-';
    file[6]=tmp.st_mode&02010?'g':(tmp.st_mode&04000?'G':(tmp.st_mode&010?'x':'-'));
    file[7]=tmp.st_mode&04?'r':'-';
    file[8]=tmp.st_mode&02?'w':'-';
    file[9]=tmp.st_mode&01001?'t':(tmp.st_mode&04000?'T':(tmp.st_mode&01?'x':'-'));
    struct passwd*user=getpwuid(tmp.st_uid);
    struct group*group=getgrgid(tmp.st_gid);
    char*time=ctime(&tmp.st_mtime);
    char result[13];
    memcpy(result,time+4,4);
    memcpy(result+4,time+8,3);
    memcpy(result+7,time+11,5);
    result[12]='\0';
    char buf[pathconf("/",_PC_PATH_MAX)];
    if(_l==1&&_i==1&&_s==1)
    {
        if(file[0]=='l')
        {
            readlink(pathname,buf,pathconf("/",_PC_PATH_MAX));
            printf("%7ld %7ld %10s %3ld %s %s %5ld %s %s -> %s\n",tmp.st_ino,tmp.st_blocks,file,tmp.st_nlink
                ,user->pw_name,group->gr_name,tmp.st_size,result,base,buf);
        }
        else 
        printf("%7ld %7ld %10s %3ld %s %s %5ld %s %s\n",tmp.st_ino,tmp.st_blocks,file,tmp.st_nlink
            ,user->pw_name,group->gr_name,tmp.st_size,result,base);
    }
    else if(_l==1&&_i==1)
    {
        if(file[0]=='l')
        {
            readlink(pathname,buf,pathconf("/",_PC_PATH_MAX));
            printf("%7ld %10s %3ld %s %s %5ld %s %s -> %s\n",tmp.st_ino,file,tmp.st_nlink
                ,user->pw_name,group->gr_name,tmp.st_size,result,base,buf);
        }
        else
        printf("%7ld %10s %3ld %s %s %5ld %s %s\n",tmp.st_ino,file,tmp.st_nlink
            ,user->pw_name,group->gr_name,tmp.st_size,result,base);
    }
    else if(_l==1&&_s==1)
    {
        if(file[0]=='l')
        {
            readlink(pathname,buf,pathconf("/",_PC_PATH_MAX));
            printf("%7ld %10s %3ld %s %s %5ld %s %s -> %s\n",tmp.st_blocks,file,tmp.st_nlink
                ,user->pw_name,group->gr_name,tmp.st_size,result,base,buf);
        }
        else
        printf("%7ld %10s %3ld %s %s %5ld %s %s\n",tmp.st_blocks,file,tmp.st_nlink
            ,user->pw_name,group->gr_name,tmp.st_size,result,base);
    }
    else if(_i==1&&_s==1)
    {
        printf("%7ld %7ld %s\n",tmp.st_ino,tmp.st_blocks,base);
    }
    else if(_l==1)
    {
        if(file[0]=='l')
        {
            readlink(pathname,buf,pathconf("/",_PC_PATH_MAX));
            printf("%10s %3ld %s %s %5ld %s %s -> %s\n",file,tmp.st_nlink
                ,user->pw_name,group->gr_name,tmp.st_size,result,base,buf);
        }
        else
        printf("%10s %3ld %s %s %5ld %s %s\n",file,tmp.st_nlink
            ,user->pw_name,group->gr_name,tmp.st_size,result,base);
    }
    else if(_i==1)
    {
        printf("%7ld %s\n",tmp.st_ino,base);
    }
    else if(_s==1)
    {
        printf("%7ld %s\n",tmp.st_blocks,base);
    }
    else
    {
        printf("%s \t",base);
    }
}
int myselect(const struct dirent *a)
{
    if(a->d_name[0]=='.')return 0;
    return 1;
}
int r_cmp(const struct dirent **a, const struct dirent **b)
{
    int max=strlen((*a)->d_name)>strlen((*b)->d_name)?strlen((*a)->d_name):strlen((*b)->d_name);
    char tmp1[strlen((*a)->d_name)+1];
    char tmp2[strlen((*b)->d_name)+1];
    for(int c=0;c<strlen((*a)->d_name);c++)
    {
        if(isupper((*a)->d_name[c]))
            tmp1[c]=tolower((*a)->d_name[c]);
        else tmp1[c]=(*a)->d_name[c];
    }
    for(int c=0;c<strlen((*b)->d_name);c++)
    {
        if(isupper((*b)->d_name[c]))
            tmp2[c]=tolower((*b)->d_name[c]);
        else tmp2[c]=(*b)->d_name[c];
    }
    for(int c=0;c<max+1;c++)
    {
        if(tmp1[c]>tmp2[c])return -1;
        else if(tmp1[c]<tmp2[c])return 1;
    }
    return 0;
}
int _cmp(const struct dirent **a, const struct dirent **b)
{
    int tmp;
    tmp=r_cmp(a,b);
    if(tmp>0)return -1;
    else if(tmp<0) return 1;
    else return 0;
}
int rt_cmp(const struct dirent **a, const struct dirent **b)
{
    struct stat tmp;
    struct stat tmp2;
    lstat((*a)->d_name,&tmp);
    lstat((*b)->d_name,&tmp2);
    if(tmp.st_mtime>tmp2.st_mtime)return 1;
    else if(tmp.st_mtime<tmp2.st_mtime)return -1;
    else return 0;
}
int t_cmp(const struct dirent **a, const struct dirent **b)
{
    struct stat tmp;
    struct stat tmp2;
    lstat((*a)->d_name,&tmp);
    lstat((*b)->d_name,&tmp2);
    if(tmp.st_mtime<tmp2.st_mtime)return 1;
    else if(tmp.st_mtime>tmp2.st_mtime)return -1;
    else return 0;
}
int func(const char*pathname,const struct stat*statbuf,int type,struct FTW*ftwbuf)
{
    if(type==FTW_DNR)printf("myls: 无法打开 '%s': 权限不够\n",pathname);
    if(type==FTW_NS)printf("%s error : ns\n",pathname);
    if(type!=FTW_D)return 0;
    printf("%s:\n",pathname);
    DIR*tmp=opendir(pathname);
    struct dirent**all=NULL;
    int count=0;
    if(_a==1&&_t==1&&_r==1)
        count=scandir(pathname,&all,NULL,rt_cmp);
    else if(_a==1&&_t==1)
        count=scandir(pathname,&all,NULL,t_cmp);
    else if(_a==1&&_r==1)
        count=scandir(pathname,&all,NULL,r_cmp);
    else if(_t==1&&_r==1)
        count=scandir(pathname,&all,myselect,rt_cmp);
    else if(_a==1)
        count=scandir(pathname,&all,NULL,_cmp);
    else if(_t==1)
        count=scandir(pathname,&all,myselect,t_cmp);
    else if(_r==1)
        count=scandir(pathname,&all,myselect,r_cmp);
    else 
        count=scandir(pathname,&all,myselect,_cmp);
    for(int c=0;c<count;c++)
    {
        char add[strlen(all[c]->d_name)+strlen(pathname)+2];
        memcpy(add,pathname,strlen(pathname));
        add[strlen(pathname)]='/';
        memcpy(add+strlen(pathname)+1,all[c]->d_name,strlen(all[c]->d_name)+1);
        output(add);
        free(all[c]);
    }
    free(all);
    printf("\n\n");
    closedir(tmp);
    return 0;
}
