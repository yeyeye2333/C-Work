#include<memory>
#include<iostream>
#include <algorithm>
namespace ttttt{
struct test
{
    public:
        void print()
        {
            std::cout<<num;
        }
    protected:
        int num=100;
};
void tttt(test a,int b)
{
    std::cout<<1;
}
struct hhh:public test
{
    void mypr()
    {
        std::cout<<num;
    }
};
template <typename T>T ttt()
{
    return 2;
}
bool com(int c=10)
{
    std::cout<<10;
}
template <typename T> void out(T a)
{
    a();
}}

void tttt(ttttt::test a,double b)
{
    std::cout<<2;
}
int main()
{
    tttt(ttttt::test(),1.0);
    char b[10]="hello";
}