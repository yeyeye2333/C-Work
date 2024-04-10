#ifndef chat_Server
#define chat_Server
#include"../chat_Timer.h"
#include"../chat_Threadpool.h"
#include"../chat_Socket.h"
#include"chat_Server_Reactor.h"
char cur_time[20]={0};//%Y-%m-%d %I:%M:%S

class Server{
public:
    Server(int _num=4):num(_num),threads(0){}
    void start();
private:
    int num;
    Threadpool threads;
    void handler(Reactor&);
};
void Server::handler(Reactor& one)
{
    while (true)
    {
        auto pollque=one.poll();
        for(auto tmp:pollque)
        {
            tmp->deal();
        }
    }
}
void Server::start()
{
    Reactor reactor[num];
    for(int c=0;c<num;c++)    
    {
        threads.addthread();
        threads.addtask(handler,reactor[c]);
    }
    Socket_listen listen;
    listen._bind();
    if(listen._listen()==0)exit(EXIT_FAILURE);
    int fd;
    while(true)
    {
        fd=listen._accept();
        reactor[fd/num].addfd(fd);
    }
}

#endif