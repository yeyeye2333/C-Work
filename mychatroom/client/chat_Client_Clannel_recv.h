#ifndef chat_Client_Clannel_recv
#define chat_Client_Clannel_recv
#include"chat_Client_Clannel.h"

class Clannel_recv:public Clannel{
public:
    Clannel_recv(int _fd):Clannel(_fd){}
    void _recv();
    ~Clannel_recv(){}
private:
};
void Clannel_recv::_recv()
{
    int len;
    recv(fd,&len,sizeof(len),0);
    char tmp[len];
    recv(fd,tmp,len,0);
    chatroom::Head _Head;
    _Head.ParseFromArray(tmp,len);
    char tmp2[_Head.len()];
    recv(fd,tmp2,_Head.len(),0);
    chatroom::File _File;
    chatroom::Message _Mess;
    chatroom::Signup_info _Signup;
    chatroom::Login_info _Login;
    chatroom::IDs _ID;
    switch(_Head.type())
    {
        case Type::login:
            break;

        case Type::signup:
            break;

        case Type::logout:
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
        
        case Type::notify_u_req:
            // 处理 notify_u_req 类型的逻辑
            break;
        
        case Type::notify_u_m:
            // 处理 notify_u_m 类型的逻辑
            break;
        
        case Type::notify_u_f:
            // 处理 notify_u_f 类型的逻辑
            break;
        
        case Type::notify_g_req:
            // 处理 notify_g_req 类型的逻辑
            break;
        
        case Type::notify_g_m:
            // 处理 notify_g_m 类型的逻辑
            break;
        
        case Type::notify_g_f:
            // 处理 notify_g_f 类型的逻辑
            break;

        default:
            break;
    }
}


#endif