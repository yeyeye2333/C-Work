#ifndef chat_Server_Clannel_nocheck
#define chat_Server_Clannel_nocheck

#include<unistd.h>
#include"chat_Server_Clannel.h"
#include<tuple>
class Clannel_nocheck:public Clannel{
public:
    Clannel_nocheck(Reactor*_reac,int _fd,uint32_t _rev)
        :Clannel(_reac,_fd,_rev){}
    ~Clannel_nocheck(){}
    void deal();
private:
    template<typename...T>
    void _send(Type type,bool is,T&&...argvs);
    void quit()
    {
        reactor->reducefd(fd);
        close(fd);
    }
};
template<typename...T>
void Clannel_nocheck::_send(Type type,bool is,T&&...argvs)
{   
    std::tuple<T...>arg(argvs...);
    int len;
    string _head;
    if(type==Type::login)
    {
        if(sizeof...(argvs)==0)
        {
            set_Head(&_head,is,Type::login,0);
            len=_head.size();
            send(fd,(string((char*)&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
            return;
        }
        string _id;
        set_IDs(&_id,{argvs...});
        set_Head(&_head,is,Type::login,_id.size());
        len=_head.size();
        send(fd,(string((char*)&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
        send(fd,_id.c_str(),_id.size(),0);
    }
    else if(type==Type::signup)
    {
        if(sizeof...(argvs)==0)
        {
            set_Head(&_head,is,Type::signup,0);
            len=_head.size();
            send(fd,(string((char*)&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
            return;
        }
        string _signup;
        set_Signup_info(&_signup,argvs...);
        set_Head(&_head,is,Type::login,_signup.size());
        len=_head.size();
        send(fd,(string((char*)&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
        send(fd,_signup.c_str(),_signup.size(),0);
    }
    else if(type==Type::logout)
    {
        set_Head(&_head,is,Type::logout,0);
        len=_head.size();
        send(fd,(string((char*)&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
    }
}
void Clannel_nocheck::deal()
{
    if(revent==EPOLLRDHUP)
    {
        quit();
        return;
    }
    else if(revent==EPOLLIN)
    {
        if(recv_cache()==0)return;
        else
        {
            chatroom::Login_info login;
            chatroom::Signup_info signup;
            int tmp;
            switch (head.type())
            {
                case Type::login:
                    login.ParseFromString(cache);
                    if(reactor->DB.login(login.uid(),login.password())==0)_send(Type::login,0);
                    else _send(Type::login,1,reactor->DB.search_mess(login.uid()));
                    break;
                
                case Type::signup:
                    signup.ParseFromString(cache);
                    if((tmp=reactor->DB.signup(signup.name(),signup.password()))==0)_send(Type::signup,0);
                    else _send(Type::signup,1,tmp);
                    break;

                case Type::logout:
                    login.ParseFromString(cache);
                    _send(Type::logout,reactor->DB.logout(login.uid(),login.password()));
                    break;

                default:
                    quit();
                    return;
            }
            reset();
        }
    }
}

#endif