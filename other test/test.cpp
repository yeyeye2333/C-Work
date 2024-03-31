#include<stop_token>
#include<thread>
int main()
{
    std::stop_source a;
    std::stop_token b(a.get_token());
    std::stop_token c(a.get_token());
    printf("%d&&%d",b.stop_requested(),c.stop_requested());
}