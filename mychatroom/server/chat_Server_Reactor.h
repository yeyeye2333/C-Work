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
#include<shared_mutex>
enum Reactor_type{nocheck,checked};

struct fd_key
{
    std::shared_ptr<Clannel> clannel;
    bool heart;
};

class Reactor{
public:
    Reactor():ulock(mtx,std::defer_lock),slock(mtx,std::defer_lock){efd=epoll_create1(EPOLL_CLOEXEC);}
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
            ulock.lock();
            fd_map.insert({fd,key(new Clannel_nocheck(this,fd,0),1)});
            cur++;
            ulock.unlock();
        }
        else if(type==checked)
        {
            ev.data.fd=fd;
            ev.events=EPOLLIN|EPOLLRDHUP;
            if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev)==-1)return 0;
            ulock.lock();
            fd_map.insert({fd,key(new Clannel_checked(this,fd,0,std::forward(argvs)...),1)});
            cur++;
            ulock.unlock();
        }
        return 1;
    }
    bool reducefd(int fd)
    {
        ulock.lock();
        fd_map.erase(fd);
        if(epoll_ctl(efd,EPOLL_CTL_DEL,fd,nullptr)==0)
        {
            cur--;
            ulock.unlock();
            return 1;
        }
        else
        {
            ulock.unlock();
            return 0;
        }
    }
    template<typename...T>
    bool modfd(int fd,Reactor_type type=Reactor_type::checked,T&&...argvs)
    {
        reducefd(fd);
        return addfd(fd,type,std::forward(argvs)...);
    }
    std::vector<std::shared_ptr<Clannel>> poll(int maxevents=10,int timeout=-1)
    {
        epoll_event ev_list[maxevents];
        int ev_num;
        std::vector<std::shared_ptr<Clannel>> tmp;
        if((ev_num=epoll_wait(efd,ev_list,maxevents,timeout))>0)
        {
            slock.lock();
            for(int c=0;c<ev_num;c++)
            {
                fd_map[ev_list[c].data.fd].clannel->set_event(ev_list[c].events);
                tmp.push_back(fd_map[ev_list[c].data.fd].clannel);
            }
            slock.unlock();
        }
        return tmp;
    }
    int heart_check()
    {
        int res=0;
        ulock.lock();
        for(auto tmp:fd_map)
        {
            if(tmp.second.heart==1)tmp.second.heart=0;
            else
            {
                res++;
                fd_map.erase(tmp.first);
            }
        }
        ulock.unlock();
        return res;
    }
private:
    int efd;
    int cur;
    std::map<int,fd_key> fd_map;
    std::shared_mutex mtx;
    std::unique_lock<std::shared_mutex> ulock;
    std::shared_lock<std::shared_mutex> slock;
    Save DB;
};

#endif