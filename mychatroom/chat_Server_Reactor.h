#ifndef chat_Server_Reactor
#define chat_Server_Reactor
#include"chat_Server_Clannel_nocheck.h"
#include"chat_Server_Clannel_checked.h"
#include<unistd.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<vector>
#include<memory>
#include<map>
#include<mutex>
enum Reactor_type{nocheck,checked};

struct fd_key
{
    std::shared_ptr<Clannel> clannel;
    bool heart;
};

class Reactor{
public:
    Reactor():ulock(mtx,std::defer_lock){efd=epoll_create1(EPOLL_CLOEXEC);}
    ~Reactor(){close(efd);}
    template<typename...T>
    bool addfd(int fd,Reactor_type type=Reactor_type::nocheck,T&&...argvs)
    {
        if(efd==-1)return 0;
        epoll_event ev;
        if (type==nocheck)
        {
            ev.data.fd=fd;
            ev.events=EPOLLIN|EPOLLRDHUP;
            if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev)==-1)return 0;
            fd_map.emplace(fd,new Clannel_nocheck(this,fd,0));
        }
        else if(type==checked)
        {
            ev.data.fd=fd;
            ev.events=EPOLLIN|EPOLLRDHUP;
            if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev)==-1)return 0;
            fd_map.insert(fd,key(new Clannel_checked(this,fd,0,std::forward(argvs)...),1));
        }
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
    template<typename...T>
    bool modfd(int fd,Reactor_type type=Reactor_type::checked,T&&...argvs)
    {
        reducefd(fd);
        return addfd(fd,type,std::forward(argvs)...);
    }
    std::vector<std::unique_ptr<Clannel>> poll(int maxevents,int timeout=-1)
    {
        epoll_event ev_list[maxevents];
        int ev_num;
        std::vector<std::shared_ptr<Clannel>> tmp;
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
    std::map<int,fd_key> fd_map;
    std::mutex mtx;
    std::unique_lock<std::mutex> ulock;
};

#endif