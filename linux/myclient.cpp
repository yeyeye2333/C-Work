#include<sys/socket.h>
#include<iostream>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#define  Server_port "50000"
void send_ser(char*a,int cfd);
void deal_n(int cfd);
int main(int argc,char**argv)
{
    int cfd;
    addrinfo*result,hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=0;
    hints.ai_flags=AI_NUMERICSERV;
    getaddrinfo(nullptr,Server_port,&hints,&result);
    for(auto rp=result;rp!=nullptr;rp=rp->ai_next)
    {
        cfd=socket(rp->ai_family,rp->ai_socktype|SOCK_CLOEXEC,rp->ai_protocol);
        if(connect(cfd,rp->ai_addr,rp->ai_addrlen)==0)
            break;
        close(cfd);
        if(rp->ai_next==nullptr)std::cerr<<"err:connect"<<'\n';
    }
    freeaddrinfo(result);
    char in[1001];
    printf("发送：");
    while(fgets(in,1001,stdin)!=nullptr)
    {
        if(in[0]!='\n')
        {
            send_ser(in,cfd);
        }
        printf("发送：");
    }
}
void send_ser(char*a,int cfd)
{
    char*tmp;
    if((tmp=strchr(a,'\n'))!=nullptr)*tmp='\0';
    int buf=strlen(a);
    write(cfd,&buf,sizeof(buf));
    write(cfd,a,strlen(a));
    deal_n(cfd);
}
void deal_n(int cfd)
{
    int buf;
    read(cfd,&buf,sizeof(buf));
    char*str=(char*)malloc(buf+1);
    if(read(cfd,str,buf)<buf)std::cerr<<"接收错误\n";
    else
    {
        str[buf]='\0';
        printf("%s\n",str);
    }
    free(str);
}