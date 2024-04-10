#include"chat_Socket.h"
#include"../other test/Foo.pb.h"
int main()
{
    Socket_client a;
    a._connect();
    test::Foo b;
    b.set_a("hhh");
    b.set_b(233);
    b.set_c(243242);
    std::string str;
    b.SerializeToString(&str);
    int tmp=str.size();
    write(a._fd(),&tmp,sizeof(tmp));
    write(a._fd(),str.c_str(),str.size());
}