#ifndef chat_Server_Clannel_checked
#define chat_Server_Clannel_checked
#include"chat_Server_Clannel.h"
class Clannel_checked:public Clannel{
public:
    Clannel_checked(Reactor*_reac,int _fd,uint32_t _rev,int _uid)
        :Clannel(_reac,_fd,_rev),uid(_uid){}
    ~Clannel_checked(){}
    int deal();
private:
    Clannel_type recv_cache();
    int uid;
    Clannel_head head;
    std::string cache;
};
int Clannel_checked::deal()
{

}
Clannel_type Clannel_checked::recv_cache()
{
    
}


#endif