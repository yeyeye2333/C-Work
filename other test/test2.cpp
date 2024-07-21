#include<iostream>
#include <linux/io_uring.h>
int main()
{
    int a[5];
    std::cerr<<sizeof(a)<<sizeof(*a);
}