#include<string>
#include<vector>
#include<iostream>
using std::cout;
using std::string;
using std::vector;

vector<int> test(vector<int> a)
{
    return a;
}
class Test_1{
    std::string str="hello";
    double value=0.0;
public:
    typedef std::string sss;
    Test_1()=default;
};
int main()
{
    Test_1::sss ss;
    Test_1 a;
    cout<<static_cast<string>("hh").size()
        <<test({1,2,3})[2]<<std::endl;
    return 0; 
}