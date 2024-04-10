#ifndef chat_Server_Clannel_checked
#define chat_Server_Clannel_checked

#include<unistd.h>
#include"chat_Server_Clannel.h"

class Clannel_checked:public Clannel{
public:
    Clannel_checked(Reactor*_reac,int _fd,uint32_t _rev,int _uid)
        :Clannel(_reac,_fd,_rev),uid(_uid),uid_ulock(mtx,std::defer_lock)
    {
        uid_ulock.lock();
        uid_map.insert({uid,fd});
        uid_ulock.unlock();
    }
    ~Clannel_checked(){}
    void deal();
private:
    template<typename...T>
    void _send(Type type,bool is,T&&...argvs);
    void quit()
    {
        uid_ulock.lock();
        uid_map.erase(uid);
        uid_ulock.unlock();
        reactor->reducefd(fd);
        close(fd);
    }
    int uid;
    static std::map<int,int> uid_map;
    static std::mutex uid_mtx;
    std::unique_lock<std::mutex> uid_ulock;
};
template<typename...T>
void _send(Type type,bool is,T&&...argvs)
{
    std::tuple<T...>arg(argvs...);
    int len;
    string _head;
    switch (type)
    {
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
void Clannel_checked::deal()
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
            chatroom::Message mess;
            chatroom::File file;
            chatroom::IDs id;
            switch (head.type())
            {
                    case Type::u_search:
                        reactor->DB.u_search(uid);
                        _send();
                        break;
                    case Type::u_request:
                        reactor->DB.u_request(uid);
                        break;
                    case Type::u_listreq:
                        reactor->DB.u_listreq(uid);
                        break;
                    case Type::u_add:
                        reactor->DB.u_add(uid);
                        break;
                    case Type::u_del:
                        reactor->DB.u_del(uid);
                        break;
                    case Type::u_blok:
                        reactor->DB.u_blok(uid);
                        break;
                    case Type::u_unblok:
                        reactor->DB.u_unblok(uid);
                        break;
                    case Type::u_message:
                        reactor->DB.u_message(uid);
                        break;
                    case Type::u_file:
                        reactor->DB.u_file(uid);
                        break;
                    case Type::u_m_history:
                        reactor->DB.u_m_history(uid);
                        break;
                    case Type::u_f_history0:
                        reactor->DB.u_f_history0(uid);
                        break;
                    case Type::u_f_history1:
                        reactor->DB.u_f_history1(uid);
                        break;
                    case Type::g_create:
                        reactor->DB.g_create(uid);
                        break;
                    case Type::g_disban:
                        reactor->DB.g_disban(uid);
                        break;
                    case Type::g_request:
                        reactor->DB.g_request(uid);
                        break;
                    case Type::g_listreq:
                        reactor->DB.g_listreq(uid);
                        break;
                    case Type::g_add:
                        reactor->DB.g_add(uid);
                        break;
                    case Type::g_del:
                        reactor->DB.g_del(uid);
                        break;
                    case Type::g_search:
                        reactor->DB.g_search(uid);
                        break;
                    case Type::g_message:
                        reactor->DB.g_message(uid);
                        break;
                    case Type::g_file:
                        reactor->DB.g_file(uid);
                        break;
                    case Type::g_quit:
                        reactor->DB.g_quit(uid);
                        break;
                    case Type::g_members:
                        reactor->DB.g_members(uid);
                        break;
                    case Type::g_addmannager:
                        reactor->DB.g_addmannager(uid);
                        break;
                    case Type::g_delmanager:
                        reactor->DB.g_delmanager(uid);
                        break;
                    case Type::g_m_history:
                        reactor->DB.g_m_history(uid);
                        break;
                    case Type::g_f_history0:
                        reactor->DB.g_f_history0(uid);
                        break;
                    case Type::g_f_history1:
                        reactor->DB.g_f_history1(uid);
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