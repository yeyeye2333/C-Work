#include<string>
#include<vector>
#include<iostream>
#include<memory>
#include<functional>
#include<filesystem>
using std::cout;
using std::string;
using std::vector;

namespace fs=std::filesystem;
int main()
{
    fs::path a(".");
    fs::directory_iterator b(a);
    for(auto &tmp:b)
    {
        std::cout<<tmp<<std::endl;
    }
}