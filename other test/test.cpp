#include<memory>
#include<iostream>
#include <algorithm>
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
}
int main()
{
    hhh a;
    a.print();  
    a.mypr();
    ttt<int>();
    char b[10]="hello";
    out(com);
}