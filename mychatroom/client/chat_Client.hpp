#ifndef chat_Client
#define chat_Client
#include"../chat_Threadpool.hpp"
#include"../chat_Socket.hpp"
#include"chat_Client_Clannel.hpp"
#include"chat_Client_Clannel_recv.hpp"
#include"chat_Client_Clannel_send.hpp"
#include<mutex>

class Client{
public:
    Client():threads(0){}
    bool start(const string&);
private:
    int fd;
    Clannel_send tosend;
    Clannel_recv torecv;
    Threadpool threads;
    void initial();
    void mainUI();
    void userUI();
    void groupUI();
    void recv_work()
    {
        while(true)torecv._recv();
    }
};
bool Client::start(const string&server)
{
    Socket_client cli;
    if(cli._connect(server)==0)exit(EXIT_FAILURE);
    fd=cli._fd();
    tosend.chfd(fd);
    torecv.chfd(fd);
    initial();
    return 0;
}
void Client::initial()
{
    threads.addthread();
    threads.addtask([this](){return this->recv_work();});
    string tmp;
    do{
        tmp="";
        do{
            pr;
            std::cout<<"1.登录\t2.注册\t3.注销\t(按\"q/Q\"退出)\n选择:";
            std::cin>>tmp;
        }while(tmp.size()==0);
        int uid;
        string name;
        string password;
        switch (tmp[0])
        {
            case 'q':case 'Q':
                exit(EXIT_SUCCESS);
                break;
            
            case '1':
                std::cout<<"输入uid:"<<std::flush;
                std::cin>>uid;
                std::cout<<"输入密码:"<<std::flush;
                std::cin>>password;
                tosend._send(Type::login,uid,0,password);
                if(tosend.ret())mainUI();
                break;
            
            case '2':
                std::cout<<"输入名字:"<<std::flush;
                std::cin>>name;
                std::cout<<"输入密码:"<<std::flush;
                std::cin>>password;
                tosend._send(Type::signup,0,0,name,password);
                break;

            case '3':
                std::cout<<"输入uid:"<<std::flush;
                std::cin>>uid;
                std::cout<<"输入密码:"<<std::flush;
                std::cin>>password;
                tosend._send(Type::logout,uid,0,password);
                break;

            default:
                break;
        }
    }while(true);
}
void Client::mainUI()
{
    std::cout<<"Welcome "<<tosend.uid<<std::endl;
    string tmp;
    do{
        tmp="";
        do{
            pr;
            std::cout<<"1.查看已有好友\t2.加好友\t3.删好友\t4.查看好友申请\t5.与好友聊天\n"
                    "6.查看已进群聊\t7.创建群聊\t8.解散群聊\t9.加群\t10.进入群聊\t(按\"q/Q\"退出)\n选择:";
            std::cin>>tmp;
        }while(tmp.size()==0);
        int id;
        string name;
        string password;
        switch (tmp[0])
        {
            case 'q':case 'Q':
                tosend.uid=0;
                return;
                break;
            
            case '1':
                if(tmp.size()>1&&tmp[1]=='0')
                {
                    std::cout<<"输入群ID:"<<std::flush;
                    std::cin>>id;
                    tosend._send(Type::g_confirm,id);
                    if(tosend.ret())groupUI();
                }
                else tosend._send(Type::u_search);
                break;
            
            case '2':
                std::cout<<"输入对方ID:"<<std::flush;
                std::cin>>id;
                tosend._send(Type::u_request,id);
                break;

            case '3':
                std::cout<<"输入对方ID:"<<std::flush;
                std::cin>>id;
                tosend._send(Type::u_del,id);
                break;

            case '4':
                tosend._send(Type::u_listreq);
                if(tosend.ret())
                {
                    std::cout<<"输入要加好友:"<<std::flush;
                    std::cin>>id;
                    tosend._send(Type::u_add,id);
                }
                break;

            case '5':
                std::cout<<"输入对方ID:"<<std::flush;
                std::cin>>id;
                tosend._send(Type::fri_confirm,id);
                if(tosend.ret())userUI();
                break;

            case '6':
                tosend._send(Type::g_search);
                break;

            case '7':
                std::cout<<"输入群名:"<<std::flush;
                std::cin>>name;
                tosend._send(Type::g_create,0,0,name);
                break;

            case '8':
                std::cout<<"输入群id:"<<std::flush;
                std::cin>>id;
                tosend._send(Type::g_disban,id);
                break;

            case '9':
                std::cout<<"输入群ID:"<<std::flush;
                std::cin>>id;
                tosend._send(Type::g_add);
                break;

            default:
                break;
        }
    }while(true);
}
void Client::userUI()
{
    string tmp;
    do{
        tmp="";
        do{
            std::cout<<"正在和"<<tosend.in_uid<<"聊天"<<std::endl;
            pr;
            std::cout<<"1.发送消息\t2.发送文件\t3.屏蔽对方\t4.解除屏蔽\n"
                "5.查询消息历史\t6.查询文件历史\t7.获取文件\t(按\"q/Q\"退出)\n选择:";
            std::cin>>tmp;
        }while(tmp.size()==0);
        int id;
        string name;
        string context;
        switch (tmp[0])
        {
            case 'q':case 'Q':
                tosend.in_uid=0;
                return;
                break;
            
            case '1':
                std::cout<<"输入消息内容(不超过1000字):"<<std::flush;
                std::cin>>context;
                tosend._send(Type::u_message,tosend.in_uid,0,context);
                break;

            case '2':
                //文件
                break;    
            
            case '3':
                tosend._send(Type::u_blok,tosend.in_uid);
                break;

            case '4':
                tosend._send(Type::u_unblok,tosend.in_uid);
                break;

            case '5':
                tosend._send(Type::u_m_history,tosend.in_uid);
                break;

            case '6':
                tosend._send(Type::u_f_history0,tosend.in_uid);
                break;

            case '7':
                std::cout<<"输入文件名:"<<std::flush;
                std::cin>>name;
                tosend._send(Type::u_f_history1,tosend.in_uid,0,name);
                break;

            default:
                break;
        }
    }while(true);
}
void Client::groupUI()
{
    string tmp;
    do{
        tmp="";
        do{
            std::cout<<"正在群"<<tosend.in_gid<<"中"<<std::endl;
            pr;
            std::cout<<"1.发送消息\t2.发送文件\t3.退群\t4.列出群成员\n"
                "5.增加管理\t6.删除管理\t7.查看进群申请\t8.移除群成员\n"
                "9.查询消息历史\t10.查询文件历史\t11.获取文件\t(按\"q/Q\"退出)\n选择:";
            std::cin>>tmp;
        }while(tmp.size()==0);
        int id;
        string name;
        string context;
        switch (tmp[0])
        {
            case 'q':case 'Q':
                tosend.in_gid=0;
                return;
                break;
            
            case '1':
                if(tmp.size()>1&&tmp[1]=='0')tosend._send(Type::g_f_history0,tosend.in_gid);
                else if(tmp.size()>1&&tmp[1]=='1')
                {
                    std::cout<<"输入文件名:"<<std::flush;
                    std::cin>>name;
                    tosend._send(Type::g_f_history1,tosend.in_gid,0,name);
                }
                else
                {
                    std::cout<<"输入消息内容(不超过1000字):"<<std::flush;
                    std::cin>>context;
                    tosend._send(Type::g_message,tosend.in_gid,0,context);
                }
                break;
            
            case '2':
                //文件
                break;

            case '3':
                tosend._send(Type::g_quit,tosend.in_gid);
                break;

            case '4':
                tosend._send(Type::g_members,tosend.in_gid);
                break;

            case '5':
                std::cout<<"输入成员ID:"<<std::flush;
                std::cin>>id;
                tosend._send(Type::g_addmanager,tosend.in_gid,id);
                break;

            case '6':
                std::cout<<"输入成员ID:"<<std::flush;
                std::cin>>id;
                tosend._send(Type::g_delmanager,tosend.in_gid,id);
                break;

            case '7':
                tosend._send(Type::g_listreq,tosend.in_gid);
                if(tosend.ret())
                {
                    std::cout<<"输入拉入对象ID:"<<std::flush;
                    std::cin>>id;
                    tosend._send(Type::g_add,tosend.in_gid,id);
                }
                break;

            case '8':
                std::cout<<"输入成员ID:"<<std::flush;
                std::cin>>id;
                tosend._send(Type::g_del,tosend.in_gid,id);
                break;

            case '9':
                tosend._send(Type::g_m_history,tosend.in_gid);
                break;

            default:
                break;
        }
    }while(true);
}
#endif