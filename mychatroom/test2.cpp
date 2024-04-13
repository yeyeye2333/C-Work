#include"chat_Socket.hpp"
#include<unistd.h>
int main()
{
    Socket_client b;
    b._connect();
    int a=-1;
    std::cerr<<send(b._fd(),&a,sizeof(a),0);
    sleep(100);
}