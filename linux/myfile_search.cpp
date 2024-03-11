#define Threads_max 20
#define Depth_max 10
#include"mythread"
#include<filesystem>

namespace fs=std::filesystem;
struct SearchConfig {
    std::string root_path=".";    // 要搜索的根目录
    std::string file_type=".cpp";    // 要搜索的文件类型，如 ".txt"、".cpp" 等
    int max_concurrency=Threads_max;      // 最大并发数
    int max_depth=Depth_max;            // 最大搜索深度
    bool skip_hidden=0;         // 是否跳过隐藏文件或目录
    std::vector<std::string> skip_paths;   // 要跳过的目录或文件的路径
};
void search();
int main(int argc,char**argv)
{
    SearchConfig config;
    {
        int tmp=0;
        for(int c=1;c<argc;c++)
        {
            if(argv[c][0]=='-')
            {
                if(argv[c][1]=='a')config.skip_hidden=1;
                if(argv[c][1]=='s'&&(c+1)<argc)config.skip_paths.push_back(argv[++c]);
            }
            else if(tmp==0)
            {
                tmp=1;
                config.root_path=argv[c];
            }
            else if(tmp==1)
            {
                tmp=2;
                config.file_type=argv[c];
            }
        }
    }

}
void search()
{
    
}