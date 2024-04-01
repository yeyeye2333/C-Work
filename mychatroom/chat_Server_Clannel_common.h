#ifndef chat_Server_Clannel_common
#define chat_Server_Clannel_common
#include"chat_Server_Clannel.h"
class Clannel_common:public Clannel{
public:
    Clannel_common(Reactor*_reac,int _fd,uint32_t _rev)
        :Clannel(_reac,_fd,_rev){}
    ~Clannel_common(){}
    int deal();
};
int Clannel_common::deal()
{

}

#endif