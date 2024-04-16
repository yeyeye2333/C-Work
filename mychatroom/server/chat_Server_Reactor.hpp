#ifndef chat_Server_Reactor
#define chat_Server_Reactor
#include<unistd.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<vector>
#include<memory>
#include<map>
#include<mutex>
#include<shared_mutex>
#include"chat_Server_Save.hpp"
#include"../chatroom.pb.h"
#include<mutex>
#define maxlen 65536
#define maxhead_len 10


using chatroom::Type;
class Reactor;
enum Reactor_type{nocheck,checked};
class Clannel{
public:
    Clannel(Reactor*_reac,int _fd,uint32_t _rev)
        :reactor(_reac),fd(_fd),revent(_rev),ulock(mtx,std::defer_lock){}
    virtual void deal()=0;
    virtual ~Clannel(){}
    virtual void _send(Type type,bool is,std::vector<string> v0={},std::vector<string> v1={},
            std::vector<string> v2={},std::vector<string> v3={},int id=0,int id2=0){};
    void set_event(uint32_t=0);
protected:
    Reactor*reactor;
    int fd;
    uint32_t revent;
    bool recv_cache();
    void reset();
    virtual void quit()=0;
    chatroom::Head head;
    std::string cache;
    std::mutex mtx;
    std::unique_lock<std::mutex> ulock;
};
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
    void _send(Type type,bool is,std::vector<string> v0={},std::vector<string> v1={},
            std::vector<string> v2={},std::vector<string> v3={},int id=0,int id2=0);
private:
    void realsend(const string& sendstr,bool is,Type type);
    void quit();
    int uid;
    static std::map<int,int> uid_map;
    static std::mutex uid_mtx;
    std::unique_lock<std::mutex> uid_ulock;
};
std::map<int,int> Clannel_checked::uid_map;
std::mutex Clannel_checked::uid_mtx;
class Clannel_nocheck:public Clannel{
public:
    Clannel_nocheck(Reactor*_reac,int _fd,uint32_t _rev)
        :Clannel(_reac,_fd,_rev){}
    ~Clannel_nocheck(){}
    void deal();
    void _send(Type type,bool is,std::vector<string> v0,std::vector<string> v1,
            std::vector<string> v2,std::vector<string> v3,int id,int id2){}
private:
    void _send(Type type,bool is,int id=0,std::vector<string> v={});
    void quit();
};
struct fd_key
{
    fd_key()=default;
    fd_key(Clannel*ptr,bool is):clannel(ptr),heart(is){}
    std::shared_ptr<Clannel> clannel;
    bool heart;
};

class Reactor{
    friend Clannel_checked;
    friend Clannel_nocheck;
public:
    Reactor():ulock(mtx,std::defer_lock),slock(mtx,std::defer_lock){efd=epoll_create1(EPOLL_CLOEXEC);}
    ~Reactor(){close(efd);}
    void addreac(Reactor*ptr)
    {
        reactors.push_back(ptr);
        reac_num++;
    }
    template<typename...T>
    bool addfd(int fd,Reactor_type type=Reactor_type::nocheck,int id=0)
    {
        if(efd==-1)return 0;
        epoll_event ev;
        if (type==nocheck)
        {
            ev.data.fd=fd;
            ev.events=EPOLLIN|EPOLLRDHUP;
            if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev)==-1)return 0;
            ulock.lock();
            fd_map.emplace(fd,fd_key(new Clannel_nocheck(this,fd,0),1));
            cur++;
            ulock.unlock();
        }
        else if(type==checked)
        {
            ev.data.fd=fd;
            ev.events=EPOLLIN|EPOLLRDHUP;
            if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev)==-1)return 0;
            ulock.lock();
            fd_map.emplace(fd,fd_key(new Clannel_checked(this,fd,0,id),1));
            cur++;
            ulock.unlock();
        }
        return 1;
    }
    bool reducefd(int fd)
    {
        ulock.lock();
        fd_map.erase(fd);
        if(epoll_ctl(efd,EPOLL_CTL_DEL,fd,nullptr)==0)
        {
            cur--;
            ulock.unlock();
            return 1;
        }
        else
        {
            ulock.unlock();
            return 0;
        }
    }
    template<typename...T>
    bool modfd(int fd,Reactor_type type,int id)
    {
        reducefd(fd);
        return addfd(fd,type,id);
    }
    std::vector<std::shared_ptr<Clannel>> poll(int maxevents=10,int timeout=-1)
    {
        epoll_event ev_list[maxevents];
        int ev_num;
        std::vector<std::shared_ptr<Clannel>> tmp;
        if((ev_num=epoll_wait(efd,ev_list,maxevents,timeout))>0)
        {
            slock.lock();
            for(int c=0;c<ev_num;c++)
            {
                fd_map[ev_list[c].data.fd].clannel->set_event(ev_list[c].events);
                tmp.push_back(fd_map[ev_list[c].data.fd].clannel);
            }
            slock.unlock();
        }
        return tmp;
    }
    int heart_check()
    {
        int res=0;
        ulock.lock();
        for(auto&tmp:fd_map)
        {
            if(tmp.second.heart==1)tmp.second.heart=0;
            else
            {
                res++;
                fd_map.erase(tmp.first);
            }
        }
        ulock.unlock();
        return res;
    }
    void heart_beat(int fd)
    {
        ulock.lock();
        try{
            fd_map.at(fd).heart=1;
        }catch(std::out_of_range){}
        ulock.unlock();
        return ;
    }
    
private:
    int efd;
    int cur;
    std::map<int,fd_key> fd_map;
    std::shared_mutex mtx;
    std::unique_lock<std::shared_mutex> ulock;
    std::shared_lock<std::shared_mutex> slock;
    Save DB;
    std::vector<Reactor*> reactors;
    int reac_num=0;
};



void Clannel::set_event(uint32_t event)
{
    revent=event;
}
bool Clannel::recv_cache()
{
    if(head.has_type()==0)
    {
        char head_len=0;
        if(recv(fd,&head_len,sizeof(head_len),MSG_DONTWAIT)!=sizeof(head_len)||head_len>maxhead_len||head_len<0)
        {
            quit();
            return 0;
        }
        char headstr[head_len];
        if(recv(fd,headstr,head_len,MSG_DONTWAIT)<head_len)
        {
            quit();
            return 0;
        }
        head.ParseFromArray(headstr,head_len);
        if(head.len()>maxlen||head.len()<0)
        {
            quit();
            return 0;
        }
        if(head.len()==0)return 1;
        char tmp[head.len()];
        int tmp_len;
        if((tmp_len=recv(fd,tmp,head.len(),MSG_DONTWAIT))<head.len())
        {
            if(tmp_len<0)return 0;
            cache.assign(tmp,tmp_len);
            return 0;
        }
        else
        {
            cache.assign(tmp,tmp_len);
            return 1;
        }
    }
    else
    {
        char tmp[head.len()-cache.size()];
        int tmp_len;
        if((tmp_len=recv(fd,tmp,head.len()-cache.size(),MSG_DONTWAIT))<(head.len()-cache.size()))
        {
            if(tmp_len<0)return 0;
            cache.append(tmp,tmp_len);
            return 0;
        }
        else
        {
            cache.append(tmp,tmp_len);
            return 1;
        }
    }
}
void Clannel::reset()
{
    head.clear_len();
    head.clear_type();
    head.clear_is();
    cache="";
}

void set_Head(string*s_ptr,bool is,Type type,int len)
{
    chatroom::Head _Head;
    _Head.set_is(is);
    _Head.set_len(len);
    _Head.set_type(type);
    _Head.SerializeToString(s_ptr);
}
void set_File(string*s_ptr,const std::vector<int> &obj={},const std::vector<string>& name={},const std::vector<string>& context={},const std::vector<string>& date={},int gid=0)
{
    chatroom::File _File;
    for(auto &tmp:obj)_File.add_obj(tmp);
    for(auto &tmp:name)_File.add_name(tmp);
    for(auto &tmp:context)_File.add_context(tmp);
    for(auto &tmp:date)_File.add_date(tmp);
    if(gid!=0)_File.set_gid(gid);
    _File.SerializePartialToString(s_ptr);
}
void set_Message(string*s_ptr,const std::vector<int>&obj={},const std::vector<string>&context={},const std::vector<string>&date={},int gid=0)
{
    chatroom::Message _Message;
    for(auto &tmp:obj)_Message.add_obj(tmp);
    for(auto &tmp:context)_Message.add_context(tmp);
    for(auto &tmp:date)_Message.add_date(tmp);
    if(gid!=0)_Message.set_gid(0);
    _Message.SerializePartialToString(s_ptr);
}
void set_IDs(string*s_ptr,const std::vector<int>&id={})
{
    chatroom::IDs _IDs;
    for(auto &tmp:id)_IDs.add_id(tmp);
    _IDs.SerializePartialToString(s_ptr);
}
void set_Signup_info(string*s_ptr,const int& uid,const string&name="",const string&password="")
{
    chatroom::Signup_info _Signup;
    _Signup.set_uid(uid);
    _Signup.set_name(name);
    _Signup.set_password(password);
    _Signup.SerializePartialToString(s_ptr);
}
void set_Login_info(string*s_ptr,const int&uid,const string&password="")
{
    chatroom::Login_info _Login;
    _Login.set_uid(uid);
    _Login.set_password(password);
    _Login.SerializePartialToString(s_ptr);
}
void set_Group_uid(string*s_ptr,const int&uid,const string&name="")
{
    chatroom::Group_uid _Group;
    _Group.set_uid(uid);
    _Group.set_name(name);
    _Group.SerializePartialToString(s_ptr);
}
void set_Strs(string*s_ptr,const std::vector<string>&str)
{
    chatroom::Strs _Str;
    for(auto&tmp:str)_Str.add_str(tmp);
    _Str.SerializePartialToString(s_ptr);
}


//Clannel_checked
void  Clannel_checked::realsend(const string&sendstr,bool is,Type type)
{
    string sendhead;
    if(is==0)std::cerr<<"mysql_err:"<<reactor->DB.mysql_err()<<"\n";
    set_Head(&sendhead,is,type,sendstr.size());
    char len=sendhead.size();
    uid_ulock.lock();
    send(fd,(string(&len,sizeof(len))+sendhead).c_str(),sizeof(len)+len,0);
    if(sendstr.size()>0)send(fd,sendstr.c_str(),sendstr.size(),0);
    uid_ulock.unlock();
}
void Clannel_checked::_send(Type type,bool is,std::vector<string> v0,std::vector<string> v1,
                        std::vector<string> v2,std::vector<string> v3,int id,int id2)
{
    std::vector<int> tmp;
    string sendstr;
    switch (type)
    {
        case Type::u_search:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_Message(&sendstr,tmp,v1);
            realsend(sendstr,is,Type::u_search);
            break;
    
        case Type::u_request:
            realsend(sendstr,is,Type::u_request);
            break;
        
        case Type::u_listreq:
            set_Strs(&sendstr, v0);
            realsend(sendstr, is, Type::u_listreq);
            break;

        case Type::u_add:
            realsend(sendstr, is, Type::u_add);
            break;

        case Type::u_del:
            realsend(sendstr, is, Type::u_del);
            break;

        case Type::u_blok:
            realsend(sendstr, is, Type::u_blok);
            break;

        case Type::u_unblok:
            realsend(sendstr, is, Type::u_unblok);
            break;

        case Type::u_message:
            realsend(sendstr, is, Type::u_message);
            break;

        case Type::u_file:
            realsend(sendstr, is, Type::u_file);
            break;

        case Type::u_m_history:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_Message(&sendstr,tmp,v1,v2);
            realsend(sendstr, is, Type::u_m_history);
            break;

        case Type::u_f_history0:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_File(&sendstr,tmp,v1,{},v2);
            realsend(sendstr, is, Type::u_f_history0);
            break;

        case Type::u_f_history1:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_File(&sendstr,tmp,v1,v2,v3);
            realsend(sendstr, is, Type::u_f_history1);
            break;

        case Type::g_create:
            set_IDs(&sendstr,{id});
            realsend(sendstr, is, Type::g_create);
            break;

        case Type::g_disban:
            realsend(sendstr, is, Type::g_disban);
            break;

        case Type::g_request:
            realsend(sendstr, is, Type::g_request);
            break;

        case Type::g_listreq:
            set_Strs(&sendstr,v1);
            realsend(sendstr, is, Type::g_listreq);
            break;

        case Type::g_add:
            realsend(sendstr, is, Type::g_add);
            break;

        case Type::g_del:
            realsend(sendstr, is, Type::g_del);
            break;

        case Type::g_search:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_Message(&sendstr,tmp,v1);
            realsend(sendstr, is, Type::g_search);
            break;

        case Type::g_message:
            realsend(sendstr, is, Type::g_message);
            break;

        case Type::g_file:
            realsend(sendstr, is, Type::g_file);
            break;

        case Type::g_quit:
            realsend(sendstr, is, Type::g_quit);
            break;

        case Type::g_members:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_Message(&sendstr,tmp,v1);
            realsend(sendstr, is, Type::g_members);
            break;

        case Type::g_addmanager:
            realsend(sendstr, is, Type::g_addmanager);
            break;

        case Type::g_delmanager:
            realsend(sendstr, is, Type::g_delmanager);
            break;

        case Type::g_m_history:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_Message(&sendstr,tmp,v1,v2);
            realsend(sendstr, is, Type::g_m_history);
            break;

        case Type::g_f_history0:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_File(&sendstr,tmp,v1,{},v2);
            realsend(sendstr, is, Type::g_f_history0);
            break;

        case Type::g_f_history1:
            for(auto &tmp2:v0)tmp.push_back(std::stoi(tmp2));
            set_File(&sendstr,tmp,v1,v2,v3);
            realsend(sendstr, is, Type::g_f_history1);
            break;

        case Type::notify_u_req:
            set_IDs(&sendstr,{id});
            realsend(sendstr, is, Type::notify_u_req);
            break;

        case Type::notify_u_m:
            set_Message(&sendstr,{id},v0);
            realsend(sendstr, is, Type::notify_u_m);
            break;

        case Type::notify_u_f:
            set_File(&sendstr,{id},v0);
            realsend(sendstr, is, Type::notify_u_f);
            break;

        case Type::notify_g_req:
            set_IDs(&sendstr,{id,id2});
            realsend(sendstr, is, Type::notify_g_req);
            break;

        case Type::notify_g_m:
            set_Message(&sendstr,{id},v0,{},id2);
            realsend(sendstr, is, Type::notify_g_m);
            break;

        case Type::notify_g_f:
            set_File(&sendstr,{id},v0,{},{},id2);
            realsend(sendstr, is, Type::notify_g_f);
            break;

        case Type::fri_confirm:
            realsend(sendstr, is, Type::fri_confirm);
            break;

        case Type::g_confirm:
            realsend(sendstr, is, Type::g_confirm);
            break;

        default:
            break;
    }
}
void Clannel_checked::deal()
{
    if(revent&EPOLLRDHUP)
    {
        quit();
        return;
    }
    else if(revent&EPOLLIN)
    {
        if(recv_cache()==0)return;
        else
        {
            chatroom::File _file;
            chatroom::Message _mess;
            chatroom::IDs _id;
            chatroom::Group_uid _group;
            chatroom::Strs _str;
            std::vector<std::vector<string>> tmpvv;
            std::vector<string> tmpv;
            // std::cerr<<head.DebugString();
            int tmpi;
            bool is=1;
            switch (head.type())
            {
                case Type::u_search:
                    tmpvv=reactor->DB.u_search(uid);
                    if(reactor->DB.mysql_iserr())is=0;
                    _send(Type::u_search,is,tmpvv[0],tmpvv[1]);
                    break;

                case Type::u_request:
                    _id.ParseFromString(cache);
                    is=reactor->DB.u_request(uid,_id.id(0));
                    _send(Type::u_request,is);
                    if(is)
                    {
                        for(auto&tmp:reactor->reactors)
                        {
                            try{
                                tmp->fd_map.at(uid_map.at(_id.id(0))).clannel->
                                _send(Type::notify_u_req,is,{},{},{},{},uid);
                            }catch(std::out_of_range){}
                        }
                    }
                    break;

                case Type::u_listreq:
                    tmpv = reactor->DB.u_listreq(uid);
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::u_listreq, is, tmpv);
                    break;

                case Type::u_add:
                    _id.ParseFromString(cache);
                    is=reactor->DB.u_add(uid,_id.id(0));
                    _send(Type::u_add,is);
                    break;

                case Type::u_del:
                    _id.ParseFromString(cache);
                    is=reactor->DB.u_del(uid,_id.id(0));
                    _send(Type::u_del,is);
                    break;

                case Type::u_blok:
                    _id.ParseFromString(cache);
                    is=reactor->DB.u_blok(uid,_id.id(0));
                    _send(Type::u_blok,is);
                    break;

                case Type::u_unblok:
                    _id.ParseFromString(cache);
                    is=reactor->DB.u_unblok(uid,_id.id(0));
                    _send(Type::u_unblok,is);
                    break;

                case Type::u_message:
                    _mess.ParseFromString(cache);
                    is=reactor->DB.u_message(uid,_mess.obj(0),_mess.context(0));
                    _send(Type::u_message,is);
                    if(is)
                    {
                        for(auto&tmp:reactor->reactors)
                        {
                            try{
                                tmp->fd_map.at(uid_map.at(_mess.obj(0))).clannel->
                                _send(Type::notify_u_m,is,{_mess.context(0)},{},{},{},uid);
                            }catch(std::out_of_range){}
                        }
                    }
                    break;

                case Type::u_file:
                    _file.ParseFromString(cache);
                    is=reactor->DB.u_file(uid,_file.obj(0),_file.context(0),_file.name(0));
                    _send(Type::u_file,is);
                    if(is)
                    {
                        for(auto&tmp:reactor->reactors)
                        {
                            try{
                                tmp->fd_map.at(uid_map.at(_file.obj(0))).clannel->
                                _send(Type::notify_u_f,is,{_file.name(0)},{},{},{},uid);
                            }catch(std::out_of_range){}
                        }
                    }
                    break;

                case Type::u_m_history:
                    _id.ParseFromString(cache);
                    tmpvv = reactor->DB.u_m_history(uid,_id.id(0));
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::u_m_history, is, tmpvv[0], tmpvv[1],tmpvv[2]);
                    break;

                case Type::u_f_history0:
                    _id.ParseFromString(cache);
                    tmpvv = reactor->DB.u_f_history0(uid,_id.id(0));
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::u_f_history0, is, tmpvv[0], tmpvv[1],tmpvv[2]);
                    break;

                case Type::u_f_history1:
                    _file.ParseFromString(cache);
                    tmpvv = reactor->DB.u_f_history1(uid,_file.obj(0),_file.name(0));
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::u_f_history1, is, tmpvv[0],tmpvv[1],tmpvv[2],tmpvv[3]);
                    break;

                case Type::g_create:
                    _group.ParseFromString(cache);
                    tmpi = reactor->DB.g_create(uid,_group.name());
                    if(tmpi== 0)_send(Type::g_create,0);
                    _send(Type::g_create,1,{},{},{},{},tmpi);
                    break;

                case Type::g_disban:
                    _id.ParseFromString(cache);
                    is=reactor->DB.g_disban(uid,_id.id(0));
                    _send(Type::g_disban,is);
                    break;

                case Type::g_request:
                    _id.ParseFromString(cache);
                    is=reactor->DB.g_request(uid,_id.id(0));
                    _send(Type::g_request,is);
                    if(is)
                    {                
                        auto managers=reactor->DB.g_manager(_id.id(0));
                        for(auto&tmp:managers)
                        {
                            for(auto&tmp2:reactor->reactors)
                            {
                                try{
                                    tmp2->fd_map.at(uid_map.at(tmp)).clannel->
                                    _send(Type::notify_g_req,is,{},{},{},{},uid,_id.id(0));
                                }catch(std::out_of_range){}
                            }
                        }  
                    }
                    break;

                case Type::g_listreq:
                    _id.ParseFromString(cache);
                    tmpv = reactor->DB.g_listreq(uid,_id.id(0));
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::g_listreq, is, tmpv);
                    break;

                case Type::g_add:
                    _id.ParseFromString(cache);
                    is=reactor->DB.g_add(uid,_id.id(0),_id.id(1));
                    _send(Type::g_add,is);
                    break;

                case Type::g_del:
                    _id.ParseFromString(cache);
                    is=reactor->DB.g_del(uid,_id.id(0),_id.id(1));
                    _send(Type::g_del,is);
                    break;

                case Type::g_search:
                    tmpvv = reactor->DB.g_search(uid);
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::g_search, is, tmpvv[0], tmpvv[1]);
                    break;

                case Type::g_message:
                    _mess.ParseFromString(cache);
                    is=reactor->DB.g_message(uid,_mess.obj(0),_mess.context(0));
                    _send(Type::g_message,is);
                    if(is)
                    {                
                        auto members=reactor->DB.g_members(_mess.obj(0));
                        for(auto&tmp:members)
                        {
                            for(auto&tmp2:reactor->reactors)
                            {
                                try{
                                    tmp2->fd_map.at(uid_map.at(tmp)).clannel->
                                    _send(Type::notify_g_m,is,{_mess.context(0)},{},{},{},uid,_mess.obj(0));
                                }catch(std::out_of_range){}
                            }
                        }  
                    }
                    break;

                case Type::g_file:
                    _file.ParseFromString(cache);
                    is=reactor->DB.g_file(uid,_file.obj(0),_file.name(0),_file.context(0));
                    _send(Type::g_file,is);
                    if(is)
                    {                
                        auto members=reactor->DB.g_members(_file.obj(0));
                        for(auto&tmp:members)
                        {
                            for(auto&tmp2:reactor->reactors)
                            {
                                try{
                                    tmp2->fd_map.at(uid_map.at(tmp)).clannel->
                                    _send(Type::notify_g_f,is,{_file.name(0)},{},{},{},uid,_file.obj(0));
                                }catch(std::out_of_range){}
                            }
                        }  
                    }
                    break;

                case Type::g_quit:
                    _id.ParseFromString(cache);
                    is=reactor->DB.g_quit(uid,_id.id(0));
                    _send(Type::g_quit,is);
                    break;

                case Type::g_members:
                    _id.ParseFromString(cache);
                    tmpvv = reactor->DB.g_members(uid,_id.id(0));
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::g_members, is, tmpvv[0], tmpvv[1]);
                    break;

                case Type::g_addmanager:
                    _id.ParseFromString(cache);
                    is=reactor->DB.g_addmanager(uid,_id.id(0),_id.id(1));
                    _send(Type::g_addmanager,is);
                    break;

                case Type::g_delmanager:
                    _id.ParseFromString(cache);
                    is=reactor->DB.g_delmanager(uid,_id.id(0),_id.id(1));
                    _send(Type::g_delmanager,is);
                    break;

                case Type::g_m_history:
                    _id.ParseFromString(cache);
                    tmpvv = reactor->DB.g_m_history(uid,_id.id(0));
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::g_m_history, is, tmpvv[0], tmpvv[1],tmpvv[2]);
                    break;

                case Type::g_f_history0:
                    _id.ParseFromString(cache);
                    tmpvv = reactor->DB.g_f_history0(uid,_id.id(0));
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::g_f_history0, is, tmpvv[0], tmpvv[1],tmpvv[2]);
                    break;

                case Type::g_f_history1:
                    _file.ParseFromString(cache);
                    tmpvv = reactor->DB.g_f_history1(uid,_file.obj(0),_file.name(0));
                    if(reactor->DB.mysql_iserr())is = 0;
                    _send(Type::g_f_history1, is, tmpvv[0],tmpvv[1],tmpvv[2],tmpvv[3]);
                    break;

                case Type::g_confirm:
                    _id.ParseFromString(cache);
                    is=reactor->DB.g_confirm(uid,_id.id(0));
                    _send(Type::g_confirm,is);
                    break;

                case Type::fri_confirm:
                    _id.ParseFromString(cache);
                    is=reactor->DB.fri_confirm(uid,_id.id(0));
                    _send(Type::fri_confirm,is);
                    break;

                case Type::heart_check:
                    reactor->heart_beat(fd);
                    break;
                
                default:
                    quit();
                    return;
            }
            reset();
        }
    }
}
void Clannel_checked::quit()
{
    uid_ulock.lock();
    uid_map.erase(uid);
    uid_ulock.unlock();
    reactor->reducefd(fd);
    close(fd);
}

//Clannel_nocheck
void Clannel_nocheck::_send(Type type,bool is,int id,std::vector<string> v)
{
    char len;
    string _head;
    if(is==0)std::cerr<<reactor->DB.mysql_err()<<"\n";
    if(type==Type::login)
    {
        if(is==0)
        {
            set_Head(&_head,is,Type::login,0);
            len=_head.size();
            send(fd,(string(&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
            return;
        }
        string _id;
        std::vector<int> tmparg;
        for(int c=0;c<v.size();c++)
        {
            tmparg.push_back(std::stoi(v[c]));
        }
        set_IDs(&_id,tmparg);
        set_Head(&_head,is,Type::login,_id.size());
        len=_head.size();
        send(fd,(string(&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
        send(fd,_id.c_str(),_id.size(),0);
    }
    else if(type==Type::signup)
    {
        if(is==0)
        {
            set_Head(&_head,is,Type::signup,0);
            len=_head.size();
            send(fd,(string(&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
            return;
        }
        string _signup;
        set_Signup_info(&_signup,id);
        set_Head(&_head,is,Type::login,_signup.size());
        len=_head.size();
        send(fd,(string(&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
        send(fd,_signup.c_str(),_signup.size(),0);
    }
    else if(type==Type::logout)
    {
        set_Head(&_head,is,Type::logout,0);
        len=_head.size();
        send(fd,(string(&len,sizeof(len))+_head).c_str(),len+sizeof(len),0);
    }
}
void Clannel_nocheck::deal()
{
    if(revent&EPOLLRDHUP)
    {
        quit();
        return;
    }
    else if(revent&EPOLLIN)
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
                    else
                    {
                        reactor->modfd(fd,Reactor_type::checked,login.uid());
                        _send(Type::login,1,0,reactor->DB.search_mess(login.uid()));
                    }
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

                case Type::heart_check:
                    reactor->heart_beat(fd);
                    break;

                default:
                    quit();
                    return;
            }
            reset();
        }
    }
}
void Clannel_nocheck::quit()
{
    reactor->reducefd(fd);
    close(fd);
}



#endif