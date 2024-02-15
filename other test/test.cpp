#include<memory>
#include<iostream>
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
int main()
{
    hhh a;
    a.print();  
    a.mypr();
}