#ifndef chat_Server_Reactor
#define chat_Server_Reactor
#include"chat_Server_Clannel_common.h"
#include<unistd.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<vector>
#include<memory>
#include<map>
enum Reactor_type{common};

class Reactor{
public:
    Reactor(){efd=epoll_create1(EPOLL_CLOEXEC);}
    ~Reactor(){close(efd);}
    bool addfd(int fd,Reactor_type type=Reactor_type::common)
    {
        if(efd==-1)return 0;
        epoll_event ev;
        if (type==common)
        {
            ev.data.fd=fd;
            ev.events=EPOLLIN|EPOLLRDHUP;
            if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev)==-1)return 0;
        }
        else if(type==listen)
        {
            ev.data.fd=fd;
            ev.events=EPOLLIN;
            if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev)==-1)return 0;
        }
        fd_map[fd]=type;
        cur++;
        return 1;
    }
    bool reducefd(int fd)
    {
        fd_map.erase(fd);
        if(epoll_ctl(efd,EPOLL_CTL_DEL,fd,nullptr)==0)
        {
            cur--;
            return 1;
        }
        else return 0;
    }
    std::vector<std::unique_ptr<Clannel>> poll(int maxevents,int timeout=-1)
    {
        epoll_event ev_list[maxevents];
        int ev_num;
        std::vector<std::unique_ptr<Clannel>> tmp;
        if((ev_num=epoll_wait(efd,ev_list,maxevents,-1))>0)
        {
            for(int c=0;c<ev_num;c++)
            {
                
            }
        }
    }
private:
    int efd;
    int cur;
    std::map<int,Reactor_type> fd_map;
};

#endif