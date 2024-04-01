#ifndef chat_Server_Save_mysql
#define chat_Server_Save_mysql

#include<mysql/mysql.h>
#include<hiredis/hiredis.h>
#include<mutex>
#include<string>
#include<memory>
#include<vector>

using std::string;
using std::unique_ptr;

enum {se,in,de,up,cr,dr};

class mysql_res{
public:
    mysql_res(MYSQL_RES*input):res(input,mysql_free_result){}
    int col_num()
    {
        if(res==nullptr)return 0;
        else
        {
            mysql_num_fields(res.get());
        }
    }
    std::vector<string> getrow()
    {
        std::vector<string> tmp;
        int num=col_num();
        MYSQL_ROW row;
        if((row=mysql_fetch_row(res.get()))!=nullptr)
        {
            for(int c=0;c<num;c++)
            {
                tmp.push_back(row[c]);
            }
        }
        return std::move(tmp);
    }
private:
    unique_ptr<MYSQL_RES,void(*)(MYSQL_RES*)> res;
};

class mysql_table{
public:
    mysql_table(string _dbname,string _user="root",string _passwd="123456",string _host="",int _port=0):ulock(mtx,std::defer_lock)
    {
        mysql_library_init(0,nullptr,nullptr);
        mysql_init(&db);
        mysql_real_connect(&db,_host.c_str(),_user.c_str(),_passwd.c_str(),_dbname.c_str(),_port,nullptr,0);
    }
    ~mysql_table()
    {
        mysql_close(&db);
        mysql_library_end();
    }
    bool ping()
    {
        return !mysql_ping(&db);
    }
    mysql_res s_f_wh_or(string name,string from,string where="",string order="");
    bool i_type_val(string name,string type,string val);
    bool d_wh(string name,string where);
    bool up_set_wh(string name,string set,string wh);
    bool c_val_eng(string name,string val_pri,string engine="innodb");
    bool drop(string name);
    string error_what()
    {
        return mysql_error(&db);
    }
private:
    string getsql(int flag,string a,string b="",string c="",string d="");
private:
    MYSQL db;
    std::timed_mutex mtx;
    std::unique_lock<std::timed_mutex> ulock;
};



mysql_res mysql_table::s_f_wh_or(string col,string name,string where,string order)
{
    string sql=getsql(se,col,name,where,order);
    ulock.lock();
    mysql_real_query(&db,sql.c_str(),sql.size());
    auto ret=mysql_store_result(&db);
    ulock.unlock();
    return ret;
}
bool mysql_table::i_type_val(string name,string type,string val)
{
    string sql=getsql(in,name,type,val);
    return !mysql_real_query(&db,sql.c_str(),sql.size());
}
bool mysql_table::d_wh(string name,string where)
{
    string sql=getsql(de,name,where);
    return !mysql_real_query(&db,sql.c_str(),sql.size());
}
bool mysql_table::up_set_wh(string name,string set,string wh)
{
    string sql=getsql(up,name,set,wh);
    return !mysql_real_query(&db,sql.c_str(),sql.size());
}
bool mysql_table::c_val_eng(string name,string val_pri,string engine)
{
    string sql=getsql(cr,name,val_pri,engine);
    return !mysql_real_query(&db,sql.c_str(),sql.size());
}
bool mysql_table::drop(string name)
{  
    string sql=getsql(dr,name);
    return !mysql_real_query(&db,sql.c_str(),sql.size());
}
string mysql_table::getsql(int flag,string a,string b,string c,string d)
{
    string sql;
    switch (flag)
    {
        case se:
            if(d.size()!=0)
                sql="select "+a+" from "+b+" where "+c+" order by "+d;
            else if(c.size()!=0)
                sql="select "+a+" from "+b+" where "+c;
            else 
                sql="select "+a+" from "+b;
            break;
        
        case in:
            sql="insert into "+a+"("+b+")values("+c+")";

        case de:
            sql="delete from "+a+" where "+b;

        case up:
            sql="update "+a+" set "+b+" where "+c;

        case cr:
            sql="create table "+a+"("+b+")engine="+c;

        case dr:
            sql="drop table "+a;

        default:
            break;
    }
    return std::move(sql);
}


#endif