#ifndef chat_Socket
#define chat_Socket
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<string>
#include<iostream>
class Socket_listen{
public:
    Socket_listen(int _domain=AF_INET6,int type=SOCK_STREAM|SOCK_CLOEXEC):domain(_domain)
    {
        fd=socket(domain,type,0);
    }
    ~Socket_listen()
    {
        if(fd!=-1)close(fd);
    }
    bool _bind(int port=50000);
    bool _listen()
    {
        return !listen(fd,SOMAXCONN);
    }
    int _accept(sockaddr* addr=nullptr,socklen_t len=0)
    {
        return accept4(fd,addr,&len,SOCK_CLOEXEC);
    }
private:
    int domain;
    int fd;
};
bool Socket_listen::_bind(int port)
{
    sockaddr_in6 in6;
    sockaddr_in in;
    if(domain==AF_INET6)
    {
        memset(&in6,0,sizeof(in6));
        in6.sin6_family=AF_INET6;
        in6.sin6_addr=in6addr_any;
        in6.sin6_port=htons(port);
        int optval=1;
        setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
        return !bind(fd,(sockaddr*)&in6,sizeof(in6));
    }
    else 
    {
        memset(&in,0,sizeof(in));
        in.sin_family=AF_INET;
        in.sin_addr.s_addr=INADDR_ANY;
        in.sin_port=htons(port);
        int optval=1;
        setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
        return !bind(fd,(sockaddr*)&in,sizeof(in));
    }
}

class Socket_client{
public:
    Socket_client(int domain=AF_INET6,int type=SOCK_STREAM)
    {
        fd=socket(domain,type,0);
    }
    ~Socket_client(){}
    bool _connect(sockaddr*addr)
    {
        return !connect(fd,addr,sizeof(*addr));
    }
private:
    int fd;
};

#endif