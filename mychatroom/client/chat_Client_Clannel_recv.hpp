#ifndef chat_Client_Clannel_recv
#define chat_Client_Clannel_recv
#include"chat_Client_Clannel.hpp"

class Clannel_recv:public Clannel{
public:
    Clannel_recv(int _fd=-1):Clannel(_fd){}
    void _recv();
    ~Clannel_recv(){}
private:
};
void Clannel_recv::_recv()
{
    char len;
    recv(fd,&len,sizeof(len),0);
    char tmp[len];
    recv(fd,tmp,len,0);
    chatroom::Head _head;
    _head.ParseFromArray(tmp,len);
    std::cerr<<_head.DebugString();
    char tmp2[_head.len()];
    if(_head.len()>0)recv(fd,tmp2,_head.len(),0);
    chatroom::File _file;
    chatroom::Message _mess;
    chatroom::Signup_info _signup;
    chatroom::Login_info _login;
    chatroom::IDs _id;
    chatroom::Strs _str;
    if(_head.type()!=Type::notify_g_f&&_head.type()!=Type::notify_g_m&&_head.type()!=Type::notify_g_req
    &&_head.type()!=Type::notify_u_f&&_head.type()!=Type::notify_u_m&&_head.type()!=Type::notify_u_req)recv_ret=_head.is();
    switch(_head.type())
    {
        case Type::login:
            if(_head.is()==0)
            {
                uid=0;
                std::cerr<<"账户不存在/密码错误\n";
            }
            else
            {
                _id.ParseFromArray(tmp2,_head.len());
                std::cout<<"离线时有用户发送消息:"<<std::endl;
                for(int c=0;c<_id.id_size();c++)std::cout<<_id.id(c)<<std::endl;
            }
            break;

        case Type::signup:
            if(_head.is()==0)std::cerr<<"注册失败\n";
            else
            {
                _signup.ParseFromArray(tmp2,_head.len());
                std::cout<<"uid:"<<_signup.uid()<<std::endl;
            }
            break;

        case Type::logout:
            if(_head.is()==0)std::cerr<<"注销失败,账号/密码错误\n";
            else std::cout<<"注册成功"<<std::endl;
            break;
            
        case Type::u_search:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _mess.ParseFromArray(tmp2,_head.len());
                std::cout<<"用户\t\tID"<<std::endl;
                for(int c=0;c<_mess.obj_size();c++)
                {
                    std::cout<<_mess.context(c)<<"\t\t"<<_mess.obj(c)<<std::endl;
                }
            }
            break;
    
        case Type::u_request:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"请求成功"<<std::endl;
            break;
        
        case Type::u_listreq:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _str.ParseFromArray(tmp2,_head.len());
                for(int c=0;c<_str.str_size();c++)
                {
                    std::cout<<_str.str(c)<<"\t";
                }
                std::cout<<std::endl;
            }
            break;
        
        case Type::u_add:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"添加成功"<<std::endl;
            break;
        
        case Type::u_del:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"删除成功"<<std::endl;
            break;
        
        case Type::u_blok:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"屏蔽成功"<<std::endl;
            break;
        
        case Type::u_unblok:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"已解除屏蔽"<<std::endl;
            break;
        
        case Type::u_message:
            if(_head.is()==0)std::cerr<<"操作失败/被屏蔽\n";
            else std::cout<<"发送成功"<<std::endl;
            break;
        
        case Type::u_file:
            if(_head.is()==0)std::cerr<<"操作失败/被屏蔽\n";
            else std::cout<<"发送成功"<<std::endl;
            break;
        
        case Type::u_m_history:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _mess.ParseFromArray(tmp2,_head.len());
                for(int c=0;c<_mess.obj_size();c++)
                {
                    std::cout<<_mess.obj(c)<<":\t"<<_mess.context(c)<<"\ttime:"<<_mess.date(c)<<std::endl;
                }
            }
            break;
        
        case Type::u_f_history0:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _file.ParseFromArray(tmp2,_head.len());
                for(int c=0;c<_file.obj_size();c++)
                {
                    std::cout<<_file.obj(c)<<":\t"<<_file.name(c)<<"\ttime:"<<_file.date(c)<<std::endl;
                }
            }
            break;
        
        case Type::u_f_history1:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                //创建文件
            }
            break;
        
        case Type::g_create:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _id.ParseFromArray(tmp2,_head.len());
                std::cout<<"创建成功 gid="<<_id.id(0)<<std::endl;
            }
            break;
        
        case Type::g_disban:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"解散成功"<<std::endl;
            break;
        
        case Type::g_request:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"请求成功"<<std::endl;
            break;
        
        case Type::g_listreq:
            if(_head.is()==0)std::cerr<<"操作失败/不是管理员\n";
            else
            {
                _str.ParseFromArray(tmp2,_head.len());
                for(int c=0;c<_str.str_size();c++)
                {
                    std::cout<<_str.str(c)<<"\t";
                }
                std::cout<<std::endl;
            }
            break;
        
        case Type::g_add:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"添加成功"<<std::endl;
            break;
        
        case Type::g_del:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"删除成功"<<std::endl;
            break;
        
        case Type::g_search:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _mess.ParseFromArray(tmp2,_head.len());
                std::cout<<"组名\t\tID";
                for(int c=0;c<_mess.obj_size();c++)
                {
                    std::cout<<_mess.obj(c)<<"\t\t"<<_mess.context(c)<<std::endl;
                }
            }
            break;
        
        case Type::g_message:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"发送成功"<<std::endl;
            break;
        
        case Type::g_file:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"发送成功"<<std::endl;
            break;
        
        case Type::g_quit:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"已退群"<<std::endl;
            break;
        
        case Type::g_members:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _mess.ParseFromArray(tmp2,_head.len());
                std::cout<<"UID\t\t管理员?";
                for(int c=0;c<_mess.obj_size();c++)
                {
                    std::cout<<_mess.obj(c)<<"\t\t";
                    if(_mess.context(c)=="1")std::cout<<"是";
                    else std::cout<<"不是";
                }
            }
            break;
        
        case Type::g_addmanager:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"设置成功"<<std::endl;
            break;
        
        case Type::g_delmanager:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else std::cout<<"取消成功"<<std::endl;
            break;
        
        case Type::g_m_history:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _mess.ParseFromArray(tmp2,_head.len());
                for(int c=0;c<_mess.obj_size();c++)
                {
                    std::cout<<_mess.obj(c)<<":\t"<<_mess.context(c)<<"\ttime:"<<_mess.date(c)<<std::endl;
                }
            }
            break;
        
        case Type::g_f_history0:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                _file.ParseFromArray(tmp2,_head.len());
                for(int c=0;c<_file.obj_size();c++)
                {
                    std::cout<<_file.obj(c)<<":\t"<<_file.name(c)<<"\ttime:"<<_file.date(c)<<std::endl;
                }
            }
            break;
        
        case Type::g_f_history1:
            if(_head.is()==0)std::cerr<<"操作失败\n";
            else
            {
                //创建文件
            }
            break;
        
        case Type::g_confirm:
            if(_head.is()==0)
            {
                in_gid=0;
                std::cerr<<"错误：不是组成员\n";
            }
            else std::cout<<"正在群"<<in_gid<<"中"<<std::endl;
            break;

        case Type::fri_confirm:
            if(_head.is()==0)
            {
                in_uid=0;
                std::cerr<<"错误：对方不是好友\n";
            }
            else std::cout<<"正在与"<<in_uid<<"聊天"<<std::endl;
            break;

        case Type::notify_u_req:

            break;
        
        case Type::notify_u_m:

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
    send_continue=1;
    cond.notify_all();
}


#endif