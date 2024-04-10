#ifndef chat_Server_Clannel
#define chat_Server_Clannel

#include<string>
#include"chat_Server_Reactor.h"
#include"chat_Server_Save.h"
#include"../chatroom.pb.h"
#include<mutex>
#define maxlen 65536
#define maxhead_len 10

using chatroom::Type;
class Clannel{
public:
    Clannel(Reactor*_reac,int _fd,uint32_t _rev)
        :reactor(_reac),fd(_fd),revent(_rev),ulock(mtx,std::defer_lock){}
    virtual void deal()=0;
    virtual ~Clannel(){}
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

void Clannel::set_event(uint32_t event)
{
    revent=event;
}
bool Clannel::recv_cache()
{
    if(head.has_type()==0)
    {
        int head_len=0;
        if(recv(fd,&head_len,sizeof(head_len),MSG_DONTWAIT)<sizeof(head_len)||head_len>maxhead_len||head_len<0)
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
        char tmp[head.len()];
        int tmp_len;
        if((tmp_len=recv(fd,tmp,head.len(),MSG_DONTWAIT))<head.len())
        {
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
    cache="";
}

void set_Head(string*s_ptr,bool is,Type type,int len)
{
    chatroom::Head _Head;
    _Head.set_is(is);
    _Head.set_len(len);
    _Head.set_type(Type::login);
    _Head.SerializeToString(s_ptr);
}
void set_File(string*s_ptr,const std::vector<int> &obj={},const std::vector<string>& name={},const std::vector<string>& context={})
{
    chatroom::File _File;
    for(auto &tmp:obj)_File.add_obj(tmp);
    for(auto &tmp:name)_File.add_name(tmp);
    for(auto &tmp:context)_File.add_context(tmp);
    _File.SerializePartialToString(s_ptr);
}
void set_Message(string*s_ptr,const std::vector<int>&obj={},const std::vector<string>&context={})
{
    chatroom::Message _Message;
    for(auto &tmp:obj)_Message.add_obj(tmp);
    for(auto &tmp:context)_Message.add_context(tmp);
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


#endif