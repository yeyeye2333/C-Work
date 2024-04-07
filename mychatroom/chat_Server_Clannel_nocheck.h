#ifndef chat_Server_Clannel_nocheck
#define chat_Server_Clannel_nocheck
#include"chat_Server_Clannel.h"
class Clannel_nocheck:public Clannel{
public:
    Clannel_nocheck(Reactor*_reac,int _fd,uint32_t _rev)
        :Clannel(_reac,_fd,_rev){}
    ~Clannel_nocheck(){}
    int deal();
private:
    Clannel_type recv_cache();
    Clannel_head head;
    std::string cache;
};
int Clannel_nocheck::deal()
{

}
Clannel_type Clannel_nocheck::recv_cache()
{
    
}

#endif