#include<iostream>
#include <linux/io_uring.h>
class A{
public:
    A()=default;
    A(A&&a){
        num=0;
        std::swap(this->num,a.num);
    }
    A& operator=(A&&a){
        num=0;
        std::swap(this->num,a.num);
        return *this;
    }
    int num;
};
int main()
{
    A a;
    A b;
    a=std::move(b);
}