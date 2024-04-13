#include<tuple>
#include<vector>
#include<iostream>
#include<map>
#include<memory>
#include<string.h>
#include"chat_Socket.hpp"
using std::string;
int main()
{
    Socket_listen a;
    a._bind();
    a._listen();
    while(1)std::cerr<<a._accept()<<std::endl;
}