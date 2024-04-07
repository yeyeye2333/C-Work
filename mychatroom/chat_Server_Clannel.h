#ifndef chat_Server_Clannel
#define chat_Server_Clannel

#include<string>
#include"chat_Server_Reactor.h"
#include"chat_Server_Save.h"
#define maxlen 65536
enum Clannel_type{nothing,login,signup,logout};
struct Clannel_head
{
    int len=0;
    Clannel_type type=Clannel_type::nothing;
};

class Clannel{
public:
    Clannel(Reactor*_reac,int _fd,uint32_t _rev)
        :reactor(_reac),fd(_fd),revent(_rev){}
    virtual int deal()=0;
    virtual ~Clannel(){}
protected:
    Reactor*reactor;
    int fd;
    uint32_t revent;
};

#endif