#define Threads_max 10
#define Depth_max 20
#include"mythread"
#include<filesystem>

namespace fs=std::filesystem;
struct SearchConfig {
    std::string root_path=".";    // 要搜索的根目录
    std::string file_type=".cpp";    // 要搜索的文件类型，如 ".txt"、".cpp" 等
    int max_concurrency=Threads_max;      // 最大并发数
    int max_depth=Depth_max;            // 最大搜索深度
    bool skip_hidden=1;         // 是否跳过隐藏文件或目录
    std::vector<std::string> skip_paths;   // 要跳过的目录或文件的路径
};
struct afile
{
    afile(int val,fs::path val2):depth(val),file(val2){}
    int depth=0;
    fs::path file;
};
bool skip_file(const fs::path& file,const SearchConfig&config);
void search(SearchConfig &config);
void type_print(const fs::path &file,const SearchConfig &config);
mpmc<afile> dir;
threadpool threads;
int main(int argc,char**argv)
{
    SearchConfig config;
    {
        for(int c=1;c<argc;c++)
        {
            if(argv[c][0]=='-')
            {
                auto tmp=argv[c][1];
                switch (tmp)
                {
                    case 'a':
                        config.skip_hidden=0;
                        break;
                    case 's':
                        if((++c)<argc)config.skip_paths.push_back(argv[c]);
                        break;
                    case 't':
                        if((++c)<argc)config.file_type=argv[c];
                        break;
                    case 'r':
                        if((++c)<argc)config.root_path=argv[c];
                        break;
                    default:
                        break;
                }
            }
        }
    }
    std::cout<<config.root_path<<std::endl;
    dir.push(unique_ptr<afile>(new afile(0,config.root_path)));
    threads.addtask(search,config);
    threads.terminal();
    std::cout<<threads.thread_num()<<std::endl;
    return 0;
}
void search(SearchConfig& config)
{
    try{
        unique_ptr<afile> tmp;
        if((tmp=dir.pop())==nullptr||tmp.get()->depth==config.max_depth||!fs::exists(tmp.get()->file))return ;
        int next=tmp.get()->depth+1;
        fs::directory_iterator dir_ptr(tmp.get()->file,fs::directory_options::skip_permission_denied);
        for(auto&a:dir_ptr)
        {
            if(skip_file(a.path(),config)||
                (config.skip_hidden==1&&a.path().filename().c_str()[0]=='.'))continue;
            if(a.is_directory())
            {
                dir.push(unique_ptr<afile>(new afile(next,a)));
                threads.addtask(search,config);
            }
            else type_print(a.path(),config);
        }
    }catch(fs::filesystem_error){}
}
bool skip_file(const fs::path& file,const SearchConfig&config)
{  
    if(config.skip_paths.size()==0)return 0;
    else 
    for(auto &tmp:config.skip_paths)
    {
        if(tmp==file.filename())return 1;
    }
    return 0;
}
void type_print(const fs::path &file,const SearchConfig &config)
{
    if(file.extension()==config.file_type)printf("%s\n",file.c_str());
}