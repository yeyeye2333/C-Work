#include<mysql/mysql.h>
#include<hiredis/hiredis.h>
#include<mutex>
#include<string>
#include<memory>
#include<vector>

using std::string;
using std::unique_ptr;
enum {se,in,de,up,cr,dr};
class DB_table{
public:
    DB_table(char*dbname,char*user="root",char*passwd="123456"):ulock(mtx,std::defer_lock)
    {
        mysql_library_init(0,nullptr,nullptr);
        mysql_init(db);
        mysql_real_connect(db,nullptr,user,passwd,dbname,0,nullptr,0);
    }
    ~DB_table()
    {
        mysql_close(db);
        mysql_library_end();
    }
    DB_res s_f_wh_or(string name,string from,string where,string order);
    bool i_type_val(string name,string type,string val);
    bool d_wh(string name,string where);
    bool u_set_wh(string name,string set,string wh);
    bool c_val_eng(string name,string val_pri,string engine);
    bool drop(string name);
private:
    string getsql(int flag,string a,string b,string c,string d);
private:
    MYSQL*db;
    std::timed_mutex mtx;
    std::unique_lock<std::timed_mutex> ulock;
};
class DB_res{
public:
    DB_res(MYSQL_RES*input):res(input,mysql_free_result){}
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
        int num=col_num();
        for(int c=0;c<num;c++)
        {
            
        }
    }
private:
    unique_ptr<MYSQL_RES,void(*)(MYSQL_RES*)> res;
};

int main()
{

}

DB_res DB_table::s_f_wh_or(string col,string name,string where="",string order="")
{
    string sql=getsql(se,col,name,where,order);
    ulock.lock();
    mysql_query(db,sql.c_str());
    auto ret=mysql_store_result(db);
    ulock.unlock();
    return ret;
}
bool DB_table::i_type_val(string name,string type,string val)
{
    string sql=getsql(in,name,type,val);
    mysql_query(db,sql.c_str());

}
bool DB_table::d_wh(string name,string where)
{

}
bool DB_table::u_set_wh(string name,string set,string wh)
{

}
bool DB_table::c_val_eng(string name,string val_pri,string engine="innodb")
{

}
bool DB_table::drop(string name)
{  

}
string DB_table::getsql(int flag,string a,string b="",string c="",string d="")
{
    string sql;
    switch (flag)
    {
        case se:
            if(d.size()!=0)
                sql="select"+a+"from"+b+"where"+c+"order by"+d;
            else if(c.size()!=0)
                sql="select"+a+"from"+b+"where"+c;
            else 
                sql="select"+a+"from"+b;
            break;
        
        case in:
            sql="insert into"+a+"("+b+")values("+c+")";

        case de:
            sql="delete from"+a+"where"+b;

        case up:
            sql="update"+a+"set"+b+"where"+c;

        case cr:
            sql="create table"+a+"("+b+")engine="+c;

        case dr:
            sql="drop table"+a;

        default:
            break;
    }
    return std::move(sql);
}