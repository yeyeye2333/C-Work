#ifndef chat_Server_Clannel
#define chat_Server_Clannel
#include"chat_Server_Reactor.h"
class Clannel{
public:
    Clannel(Reactor*_reac,int _fd,uint32_t _rev)
        :reactor(_reac),fd(_fd),revent(_rev){}
    virtual int deal()=0;
    virtual ~Clannel(){}
private:
    Reactor*reactor;
    int fd;
    uint32_t revent;
};

#endif