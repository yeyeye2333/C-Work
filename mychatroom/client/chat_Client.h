#ifndef chat_Client
#define chat_Client
#include"../chat_Threadpool.h"
#include"../chat_Socket.h"
#include<mutex>

class Client{
public:
    Client():ulock(mtx,std::defer_lock){}
    bool start();
private:
    int fd;
    std::mutex mtx;
    std::unique_lock<std::mutex> ulock;
    void initial();
    int uid;
    std::string u_name;
    int in_uid;
    int in_gid;
    void mainUI();
    void userUI();
    void groupUI();
};
bool Client::start()
{
    Socket_client cli;
    if(cli._connect()==0)exit(EXIT_FAILURE);
    fd=cli._fd();
    initial();
}


#endif