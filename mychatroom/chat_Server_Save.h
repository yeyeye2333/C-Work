#ifndef chat_Server_Save
#define chat_Server_Save

#include"chat_Server_Save_mysql.h"
#include"chat_Server_Save_redis.h"


class Save{
public:
    Save()
    {
        save_mysql.c_val_eng("u_info","uid int not null auto_increment,\
                                        user_name char(30) not null,\
                                        passwd char(30) not null,\
                                        group_num int not null,\
                                        primary key(uid)");
        save_mysql.c_val_eng("u_relation","uid int not null,\
                                            fri_uid int not null,\
                                            blok bool not null,\
                                            mess_num tinyint not null,\
                                            file_num tinyint not null,\
                                            has_mess bool not null,\
                                            primary key(uid,fri_uid)");
        save_mysql.c_val_eng("u_request","send_uid int not null,\
                                            recv_uid int not null,\
                                            primary key(send_uid,recv_uid)");
        save_mysql.c_val_eng("u_messages","uid int not null,\
                                            fri_uid int not null,\
                                            message varchar(1000) not null,\
                                            time datetime not null");
        save_mysql.c_val_eng("u_files","uid int not null,\
                                            fri_uid int not null,\
                                            files blob not null,\
                                            time datetime not null");//blob<=65535

        save_mysql.c_val_eng("g_members","gid int not null,\
                                            uid int not null,\
                                            is_manager bool not null,\
                                            primary key(gid,uid)");
        save_mysql.c_val_eng("g_info","gid int not null auto_increment,\
                                        group_master int not null,\
                                        group_name char(30) not null,\
                                        mess_num smallint not null,\
                                        file_num smallint not null,\
                                        primary key(gid)");        
        save_mysql.c_val_eng("g_request","send_uid int not null,\
                                            recv_uid int not null,\
                                            primary key(send_uid,recv_uid)");       
        save_mysql.c_val_eng("g_messages","gid int not null,\
                                            uid int not null,\
                                            message varchar(1000) not null,\
                                            time datetime not null");   
        save_mysql.c_val_eng("g_files","gid int not null,\
                                        uid int not null,\
                                        files blob not null,\
                                        time datetime not null");                                                                                                                                  
    }
    //user
    mysql_res select_u_info(const string &uid)
    {
        return save_mysql.s_f_wh_or("uid,user_name,password,group_num","u_info","uid="+uid);
    }
    mysql_res select_u_relation(const string &uid,const string&fri_uid="")
    {
        if(fri_uid.size()==0)return save_mysql.s_f_wh_or("uid,fri_uid,blok,mess_num,file_num,has_mess","u_relation","uid="+uid);
        else return save_mysql.s_f_wh_or("uid,fri_uid,blok,mess_num,file_num,has_mess","u_relation",
                                "uid="+uid+" and fri_uid="+fri_uid);
    }
    mysql_res select_u_request(const string &recv_uid)
    {
        return save_mysql.s_f_wh_or("send_uid,recv_uid","u_request","recv_uid="+recv_uid);
    }
    mysql_res select_u_messages(const string &uid,const string&fri_uid)
    {
        return save_mysql.s_f_wh_or("uid,fri_uid,message,time","u_messages",
                                "uid="+uid+" and fri_uid="+fri_uid);
    }
    mysql_res select_u_files(const string &uid,const string&fri_uid)
    {
        return save_mysql.s_f_wh_or("uid,fri_uid,file,time","u_files",
                                "uid="+uid+" and fri_uid="+fri_uid);
    }

    bool insert_u_info(const string &password,const string &user_name="",const string &group_num="0")
    {
        return save_mysql.i_type_val("u_info","user_name,password,group_num",
                                "\'"+user_name+"\',\'"+password+"\',"+group_num);
    }
    bool insert_u_relation(const string &uid,const string &fri_uid,const string &blok="0",const string &mess_num="0",
                        const string &file_num="0",const string &has_mess="0")
    {
        return save_mysql.i_type_val("u_relation","uid,fri_uid,blok,mess_num,file_num,has_mess",
                                uid+","+fri_uid+","+blok+","+mess_num+","+file_num+","+has_mess);
    }
    bool insert_u_request(const string &send_uid,const string &recv_uid)
    {
        return save_mysql.i_type_val("u_request","send_uid,recv_uid",
                                send_uid+","+recv_uid);
    }
    bool insert_u_messages(const string &uid,const string &fri_uid,const string &message,const string &time)
    {
        return save_mysql.i_type_val("u_messages","uid,fri_uid,message,time",
                                uid+","+fri_uid+",\'"+message+"\',\'"+time+"\'");
    }
    bool insert_u_files(const string &uid,const string &fri_uid,const string &file,const string &time)
    {
        return save_mysql.i_type_val("u_files","uid,fri_uid,file,time",
                                uid+","+fri_uid+",\'"+file+"\',\'"+time+"\'");
    }

    bool delete_u_info(const string&uid)
    {
        return save_mysql.d("u_info where uid="+uid);
    }
    bool delete_u_relation(const string&uid,const string&fri_uid="")
    {
        if(fri_uid.size()==0)return save_mysql.d("u_relation where uid="+uid+" or fri_uid="+uid);
        else return save_mysql.d("u_relation where uid="+uid+" and fri_uid="+fri_uid);
    }
    bool delete_u_request(const string&send_uid,const string&recv_uid="")
    {
        if(recv_uid.size()==0)return save_mysql.d("u_request where send_uid="+send_uid+" or recv_uid="+send_uid);
        else return save_mysql.d("u_request where send_uid="+send_uid+" and recv_uid="+recv_uid);
    }
    bool delete_u_messages_num(const string&uid,const string&fri_uid="",int num=1)
    {
        if(fri_uid.size()==0)return save_mysql.d("u_messages where uid="+uid+" or fri_uid="+uid);
        else return save_mysql.d("u_messages where uid="+uid+" and fri_uid="+fri_uid+" order by time limit "+std::to_string(num));
    }
    bool delete_u_files(const string&uid,const string&fri_uid="",int num=1)
    {
        if(fri_uid.size()==0)return save_mysql.d("u_files where uid="+uid+" or fri_uid="+uid);
        else return save_mysql.d("u_files where uid="+uid+" and fri_uid="+fri_uid+" order by time limit "+std::to_string(num));
    }

    bool update_u_info();
    bool update_u_relation();

    //group
    mysql_res select_g_memebers_gid(const string &gid)
    {
        return save_mysql.s_f_wh_or("gid,uid,is_manager","g_members","gid="+gid);
    }
    mysql_res select_g_memebers_uid(const string &uid)
    {
        return save_mysql.s_f_wh_or("gid,uid,is_manager","g_members","uid="+uid);
    }
    mysql_res select_g_info(const string &gid)
    {
        return save_mysql.s_f_wh_or("gid,group_master,group_name,mess_num,file_num","g_info","gid="+gid);
    }
    mysql_res select_g_request(const string &gid)
    {
        return save_mysql.s_f_wh_or("send_uid,recv_gid","g_request","gid="+gid);
    }
    mysql_res select_g_messages(const string &gid)
    {
        return save_mysql.s_f_wh_or("gid,uid,message,time","g_messages","gid="+gid);
    }
    mysql_res select_g_files(const string &gid)
    {
        return save_mysql.s_f_wh_or("gid,uid,file,time","g_files","gid="+gid);
    }

    bool insert_g_memebers(const string &gid,const string &uid,const string &is_manager)
    {
        return save_mysql.i_type_val("g_members","gid,uid,is_manager",
                                    gid+","+uid+","+is_manager);
    }
    bool insert_g_info(const string &group_master,const string &group_name,const string &mess_num,const string &file_num)
    {
        return save_mysql.i_type_val("g_info","group_master,group_name,mess_num,file_num",
                                    group_master+",\'"+group_name+"\',"+mess_num+","+file_num);
    }
    bool insert_g_request(const string &send_uid,const string &recv_gid)
    {
        return save_mysql.i_type_val("g_request","send_uid,recv_gid",
                                    send_uid+","+recv_gid);
    }
    bool insert_g_messages(const string &gid,const string &uid,const string &message,const string &time)
    {
        return save_mysql.i_type_val("g_messages","gid,uid,message,time",
                                    gid+","+uid+",\'"+message+"\',\'"+time+"\'");
    }
    bool insert_g_files(const string &gid,const string &uid,const string &file,const string &time)
    {
        return save_mysql.i_type_val("g_files","gid,uid,file,time",
                                    gid+","+uid+",\'"+file+"\',\'"+time+"\'");
    }

    bool delete_g_memebers_gid(const string &gid,const string &uid="")
    {
        if(uid.size()==0)return save_mysql.d("g_members where gid="+gid);
        else return save_mysql.d("g_members where gid="+gid+" and uid="+uid);
    }
    bool delete_g_members_gid(const string &uid)
    {
        return save_mysql.d("g_members where uid="+uid);
    }
    bool delete_g_info(const string &gid)
    {
        return save_mysql.d("g_info where gid="+gid);
    }
    bool delete_g_request_gid(const string &recv_gid,const string &send_uid="")
    {
        if(send_uid.size()==0)return save_mysql.d("g_request where recv_gid="+recv_gid);
        return save_mysql.d("g_request where recv_gid="+recv_gid+" and send_uid="+send_uid);
    }
    bool delete_g_request_uid(const string &send_uid)
    {
        return save_mysql.d("g_request where send_uid="+send_uid);
    }
    bool delete_g_messages(const string &gid,int num=-1)
    {
        if(num=-1)return save_mysql.d("g_messages where gid="+gid);
        else return save_mysql.d("g_messages where gid="+gid+" order by time limit "+std::to_string(num));
    }
    bool delete_g_files(const string &gid,int num=-1)
    {
        if(num=-1)return save_mysql.d("g_files where gid="+gid);
        else return save_mysql.d("g_files where gid="+gid+" order by time limit "+std::to_string(num));
    }

    bool update_g_members();
    bool update_g_info();
private:
    mysql_table save_mysql;
    redis_text save_redis;
};

#endif