#define PROTOBUF_CONSTEXPR
#include"../other test/Foo.pb.h"
#include"chat_Socket.h"
#include<string>
#include<iostream>
int main()
{
    Socket_listen lis;
    lis._bind();
    lis._listen();
    int cli=lis._accept();
    test::Foo a;
    int len;
    read(cli,&len,sizeof(len));
    char str[len];
    read(cli,str,len);
    a.ParseFromArray(str,len);
    std::cout<<a.DebugString()<<"\nfrom\n"<<std::string(str,len)<<"is"<<len<<std::endl;
    std::cout<<a.a()<<" and "<<a.b()<<" "<<a.c();
}