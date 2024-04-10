#ifndef chat_Client_Clannel_send
#define chat_Client_Clannel_send
#include"chat_Client_Clannel.h"

class Clannel_send:public Clannel{
public:
    Clannel_send(int _fd):Clannel(_fd){}
    template<typename ...T>
    void _send(Type,T&&...argvs);
    ~Clannel_send(){}
private:
    void realsend(string sendstr,Type type);
};
template<typename ...T>
void Clannel_send::_send(Type type,T&&...argvs)
{
    string sendstr;
    switch (type)
    {
        case Type::login:
            set_Login_info(sendstr,std::forward(argvs)...);
            realsend(sendstr,Type::login);
            break;

        case Type::signup:
            set_Signup_info(sendstr,std::forward(argvs)...);
            realsend(sendstr,Type::signup);
            break;

        case Type::logout:
            set_Login_info(sendstr,std::forward(argvs)...);
            realsend(sendstr,Type::logout);
            break;

        case Type::u_search:
            
            break;
    
        case Type::u_request:
            // 处理 u_request 类型的逻辑
            break;
        
        case Type::u_listreq:
            // 处理 u_listreq 类型的逻辑
            break;
        
        case Type::u_add:
            // 处理 u_add 类型的逻辑
            break;
        
        case Type::u_del:
            // 处理 u_del 类型的逻辑
            break;
        
        case Type::u_blok:
            // 处理 u_blok 类型的逻辑
            break;
        
        case Type::u_unblok:
            // 处理 u_unblok 类型的逻辑
            break;
        
        case Type::u_message:
            // 处理 u_message 类型的逻辑
            break;
        
        case Type::u_file:
            // 处理 u_file 类型的逻辑
            break;
        
        case Type::u_m_history:
            // 处理 u_m_history 类型的逻辑
            break;
        
        case Type::u_f_history0:
            // 处理 u_f_history0 类型的逻辑
            break;
        
        case Type::u_f_history1:
            // 处理 u_f_history1 类型的逻辑
            break;
        
        case Type::g_create:
            // 处理 g_create 类型的逻辑
            break;
        
        case Type::g_disban:
            // 处理 g_disban 类型的逻辑
            break;
        
        case Type::g_request:
            // 处理 g_request 类型的逻辑
            break;
        
        case Type::g_listreq:
            // 处理 g_listreq 类型的逻辑
            break;
        
        case Type::g_add:
            // 处理 g_add 类型的逻辑
            break;
        
        case Type::g_del:
            // 处理 g_del 类型的逻辑
            break;
        
        case Type::g_search:
            // 处理 g_search 类型的逻辑
            break;
        
        case Type::g_message:
            // 处理 g_message 类型的逻辑
            break;
        
        case Type::g_file:
            // 处理 g_file 类型的逻辑
            break;
        
        case Type::g_quit:
            // 处理 g_quit 类型的逻辑
            break;
        
        case Type::g_members:
            // 处理 g_members 类型的逻辑
            break;
        
        case Type::g_addmannager:
            // 处理 g_addmannager 类型的逻辑
            break;
        
        case Type::g_delmanager:
            // 处理 g_delmanager 类型的逻辑
            break;
        
        case Type::g_m_history:
            // 处理 g_m_history 类型的逻辑
            break;
        
        case Type::g_f_history0:
            // 处理 g_f_history0 类型的逻辑
            break;
        
        case Type::g_f_history1:
            // 处理 g_f_history1 类型的逻辑
            break;
        
        default:
            break;
    }
}
void  Clannel_send::realsend(string sendstr,Type type)
{
    string sendhead;
    set_Head(&sendhead,type,sendstr.size());
    int len=sendhead.size();
    send(fd,(string((char*)len,sizeof(len))+sendstr).c_str(),len+sendhead.size(),0);
    if(sendstr.size()>0)send(fd,sendstr.c_str(),sendstr.size(),0);
}

#endif
