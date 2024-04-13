#ifndef chat_Server
#define chat_Server
#include"../chat_Timer.hpp"
#include"../chat_Threadpool.hpp"
#include"../chat_Socket.hpp"
#include"chat_Server_Reactor.hpp"
#include<time.h>
char cur_time[20]={0};//%Y-%m-%d %I:%M:%S

class Server{
public:
    Server(int _num=4):num(_num),threads(_num){}
    void start();
private:
    int num;
    Threadpool threads;
};
void handler(Reactor& one)
{
    while (true)
    {std::cerr<<"21"<<"\n";
        auto pollque=one.poll();
        for(auto tmp:pollque)
        {
            std::cerr<<"25"<<"\n";
            tmp->deal();
        }
    }
}
void Server::start()
{
    Timer timer(2,1);
    timer.init();
    Reactor reactor[num];
    for(int c=0;c<num;c++)    
    {
        threads.addtask(handler,std::ref(reactor[c]));
    }
    // timer.start();
    Socket_listen listen;
    listen._bind();
    if(listen._listen()==0)exit(EXIT_FAILURE);
    int fd;
    while(true)
    {
        if((fd=listen._accept())!=-1)
        {
            std::cerr<<"add="<<fd<<"\n";
            reactor[fd%num].addfd(fd);
        }
        if(timer.timer_flag>0)
        {
            auto tmp=time(nullptr);
            strftime(cur_time,2,"%Y-%m-%d %I:%M:%S",localtime(&tmp));
        }
    }
}

#endif