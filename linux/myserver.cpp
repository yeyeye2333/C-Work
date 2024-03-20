#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<iostream>
#include<unistd.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<signal.h>
#include<string>
#define Server_port "50000"
void lis_connect(int lfd,int ep_fd,sockaddr*addr=nullptr,socklen_t len=0);
void deal_cli(epoll_event ev);
void deal_n(int fd);
int main()
{
    sigset_t sig;
    sigemptyset(&sig);
    sigaddset(&sig,SIGPIPE);
    sigprocmask(SIG_BLOCK,&sig,nullptr);
    addrinfo*result,hints;
    int lfd,optval=1;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=0;
    hints.ai_flags=AI_PASSIVE|AI_NUMERICSERV;
    getaddrinfo(nullptr,Server_port,&hints,&result);
    for(auto rp=result;rp!=nullptr;rp=rp->ai_next)
    {
        lfd=socket(rp->ai_family,rp->ai_socktype|SOCK_CLOEXEC|SOCK_NONBLOCK,rp->ai_protocol);
        setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
        if(bind(lfd,rp->ai_addr,rp->ai_addrlen)==0)
            break;
        close(lfd);
        if(rp->ai_next==nullptr)std::cerr<<"err:listen port"<<'\n';
    }
    freeaddrinfo(result);
    listen(lfd,SOMAXCONN);
    int ep_fd=epoll_create1(EPOLL_CLOEXEC);
    epoll_event ev;
    ev.data.fd=lfd;
    ev.events=EPOLLIN;
    epoll_event ev_list[10];
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,lfd,&ev);
    for(;;)
    {
        int tmp;
        if((tmp=epoll_wait(ep_fd,ev_list,10,-1))>0)
        {
            // printf("tttttttmmmmmmmmppppppppp is %d\n",tmp);
            for(;tmp!=0;tmp--)
            {
                if(ev_list[tmp-1].data.fd==lfd)lis_connect(lfd,ep_fd);
                else deal_cli(ev_list[tmp-1]);
            }
        }
    }
}
void lis_connect(int lfd,int ep_fd,sockaddr*addr,socklen_t len)
{
    auto tmp=len;
    auto tmp2=accept4(lfd,addr,&len,SOCK_CLOEXEC);
    if(tmp2<0)std::cerr<<"err:accept"<<std::endl;
    len=tmp;
    epoll_event ev;
    ev.data.fd=tmp2;
    ev.events=EPOLLIN|EPOLLRDHUP;
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,tmp2,&ev);
}
void deal_cli(epoll_event ev)
{
    int fd=ev.data.fd;
    if(ev.events&EPOLLRDHUP)
    {
        close(fd);
    }
    else if(ev.events&EPOLLIN)deal_n(fd);
}
void deal_n(int fd)
{
    sockaddr_storage a_addr;
    socklen_t addrlen=sizeof(a_addr);
    int buf;
    recvfrom(fd,&buf,sizeof(buf),0,(sockaddr*)&a_addr,&addrlen);
    // printf("bufffffffffff is %d\n",buf);
    bool is6=0;
    if(a_addr.ss_family==AF_INET6)is6=1;
    char*str=nullptr;
    if(buf<=1000)
    {
        str=(char*)malloc(sizeof(char)*buf+1);
        recv(fd,str,buf,0);
        str[buf]='\0';
        std::string str2=str;
        std::string str3;
        free(str);
        int send_buf;
        if(is6==0)
        {
            const sockaddr_in*in_addr=(sockaddr_in*)&a_addr;
            char*addr_str=(char*)malloc(sizeof(char)*INET_ADDRSTRLEN+1);
            inet_ntop(AF_INET,&(in_addr->sin_addr.s_addr),addr_str,INET_ADDRSTRLEN);
            // printf("buf is %d\n",buf);
            send_buf=strlen(addr_str)+1+buf;
            send(fd,&send_buf,sizeof(send_buf),MSG_DONTWAIT);
            str3=addr_str;
            free(addr_str);
        }
        else
        {
            const sockaddr_in6*in6_addr=(sockaddr_in6*)&a_addr;
            char*addr_str=(char*)malloc(sizeof(char)*INET6_ADDRSTRLEN+1);
            inet_ntop(AF_INET6,&(in6_addr->sin6_addr.__in6_u),addr_str,INET6_ADDRSTRLEN);
            // printf("buf is %d\n",buf);
            send_buf=strlen(addr_str)+1+buf;
            send(fd,&send_buf,sizeof(send_buf),MSG_DONTWAIT);
            str3=addr_str;
            free(addr_str);
        }
        str3+=":"+str2;
        send(fd,str3.c_str(),send_buf,MSG_DONTWAIT);
    }
    else
    {
        buf=strlen("too long!!!");
        std::string str;
        int send_buf;
        if(is6==0)
        {
            const sockaddr_in*in_addr=(sockaddr_in*)&a_addr;
            char*addr_str=(char*)malloc(sizeof(char)*INET_ADDRSTRLEN);
            inet_ntop(AF_INET,&(in_addr->sin_addr.s_addr),addr_str,INET_ADDRSTRLEN);
            send_buf=strlen(addr_str)+1+buf;
            send(fd,&send_buf,sizeof(send_buf),MSG_DONTWAIT);
            str=addr_str;
            free(addr_str);
        }
        else
        {
            const sockaddr_in6*in6_addr=(sockaddr_in6*)&a_addr;
            char*addr_str=(char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
            inet_ntop(AF_INET6,&(in6_addr->sin6_addr.__in6_u),addr_str,INET6_ADDRSTRLEN);
            send_buf=strlen(addr_str)+1+buf;
            send(fd,&send_buf,sizeof(send_buf),MSG_DONTWAIT);
            str=addr_str;
            free(addr_str);
        }
        str=str+":"+"too long!!!";
        send(fd,str.c_str(),send_buf,MSG_DONTWAIT);
    }
}