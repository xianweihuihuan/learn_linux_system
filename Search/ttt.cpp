#include "thread_pool.hpp"

using task_t = std::function<void()>;

int main()
{
    std::string root_path;
    std::cout << "请输入目录：";
    std::cin >> root_path;
    std::string file_type;
    std::cout << "请输入文件类型：";
    std::cin >> file_type;
    int num = 0;
    std::cout << "请输入最大并发数：";
    std::cin >> num;
    int max = 0;
    std::cout << "请输入最大搜索深度：";
    std::cin >> max;
    char charge;
    bool skip;
    std::cout << "请输入是否跳过隐藏目录或文件(y/n)：";
    std::cin >> charge;
    switch (charge)
    {
    case 'y':
        skip = true;
        break;
    case 'n':
        skip = false;
        break;
    default:
        exit(1);
    }
    std::vector<std::string> skip_path;
    int sum = 0;
    std::cout << "请输入要跳过的文件或目录数量：";
    std::cin >> sum;
    while (sum--)
    {
        std::string tmp;
        std::cout << "请输入要跳过的文件或目录：";
        std::cin >> tmp;
        skip_path.push_back(tmp);
    }
    chdir(root_path.c_str());
    char path[200];
    getcwd(path,200);
    std::string se = path;
    Xianwei::thread_pool pl(num);
    Xianwei::SearchConfig tt(se, file_type, num, max, skip, skip_path);
    pl.Start();
    pl.Enter(tt);
    
    pl.Stop();
    pl.Join();

}