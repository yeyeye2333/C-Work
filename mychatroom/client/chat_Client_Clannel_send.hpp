#ifndef chat_Client_Clannel_send
#define chat_Client_Clannel_send
#include"chat_Client_Clannel.hpp"

class Clannel_send:public Clannel{
public:
    Clannel_send(int _fd=-1):Clannel(_fd){}
    template<typename ...T>
    void _send(Type,int id1=0,int id2=0,const string&s1="",const string&s2="");
    ~Clannel_send(){}
private:
    void realsend(const string& sendstr,Type type);
};
template<typename ...T>
void Clannel_send::_send(Type type,int id1,int id2,const string&s1,const string&s2)
{
    send_continue=0;
    string sendstr;
    switch (type)
    {
        case Type::login:
            uid=id1;
            set_Login_info(&sendstr,id1,s1);
            realsend(sendstr,Type::login);
            break;

        case Type::signup:
            set_Signup_info(&sendstr,s1,s2);
            realsend(sendstr,Type::signup);
            break;

        case Type::logout:
            set_Login_info(&sendstr,id1,s2);
            realsend(sendstr,Type::logout);
            break;

        case Type::u_search:
            realsend("",Type::u_search);
            break;
    
        case Type::u_request:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::u_request);
            break;
        
        case Type::u_listreq:
            realsend("",Type::u_listreq);
            break;
        
        case Type::u_add:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::u_add);
            break;
        
        case Type::u_del:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::u_del);
            break;
        
        case Type::u_blok:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::u_blok);
            break;
        
        case Type::u_unblok:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::u_unblok);
            break;
        
        case Type::u_message:
            set_Message(&sendstr,{id1},{s1});
            realsend(sendstr,Type::u_message);
            break;
        
        case Type::u_file:
            set_File(&sendstr,{id1},{s1},{s2});
            realsend(sendstr,Type::u_file);
            break;
        
        case Type::u_m_history:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::u_m_history);
            break;
        
        case Type::u_f_history0:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::u_f_history0);
            break;
        
        case Type::u_f_history1:
            set_File(&sendstr,{id1},{s1});
            realsend(sendstr,Type::u_f_history1);
            break;
        
        case Type::g_create:
            set_Group_uid(&sendstr,0,s1);
            realsend(sendstr,Type::g_create);
            break;
        
        case Type::g_disban:
            set_Group_uid(&sendstr,id1);
            realsend(sendstr,Type::g_disban);
            break;
        
        case Type::g_request:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::g_request);
            break;
        
        case Type::g_listreq:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::g_listreq);
            break;
        
        case Type::g_add:
            set_IDs(&sendstr,{id1,id2});
            realsend(sendstr,Type::g_add);
            break;
        
        case Type::g_del:
            set_IDs(&sendstr,{id1,id2});
            realsend(sendstr,Type::g_del);
            break;
        
        case Type::g_search:
            realsend(sendstr,Type::g_search);
            break;
        
        case Type::g_message:
            set_Message(&sendstr,{id1},{s1});
            realsend(sendstr,Type::g_message);
            break;
        
        case Type::g_file:
            set_File(&sendstr,{id1},{s1},{s2});
            realsend(sendstr,Type::g_file);
            break;
        
        case Type::g_quit:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::g_quit);
            break;
        
        case Type::g_members:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::g_members);
            break;
        
        case Type::g_addmanager:
            set_IDs(&sendstr,{id1,id2});
            realsend(sendstr,Type::g_addmanager);
            break;
        
        case Type::g_delmanager:
            set_IDs(&sendstr,{id1,id2});
            realsend(sendstr,Type::g_delmanager);
            break;
        
        case Type::g_m_history:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::g_m_history);
            break;
        
        case Type::g_f_history0:
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::g_f_history0);
            break;
        
        case Type::g_f_history1:
            set_File(&sendstr,{id1},{s1});
            realsend(sendstr,Type::g_f_history1);
            break;

        case Type::g_confirm:
            in_gid=id1;
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::g_confirm);
            break;

        case Type::fri_confirm:
            in_uid=id1;
            set_IDs(&sendstr,{id1});
            realsend(sendstr,Type::fri_confirm);
            break;

        default:
            break;
    }
    std::cerr<<"等待响应...\n";
    ulock.lock();
    cond.wait(ulock,[](){return send_continue==1;});
    ulock.unlock();
}
void  Clannel_send::realsend(const string &sendstr,Type type)
{
    string sendhead;
    set_Head(&sendhead,type,sendstr.size());
    char len=sendhead.size();
    string a(string(&len,sizeof(len))+sendhead);
    if(send(fd,(string(&len,sizeof(len))+sendhead).c_str(),sizeof(len)+len,0)==-1)exit(EXIT_FAILURE);
    if(sendstr.size()>0)if(send(fd,sendstr.c_str(),sendstr.size(),0)==-1)exit(EXIT_FAILURE);
}

#endif
