#include <iostream>
#include <ctime>
#include <vector>
#include <assert.h>

typedef void (*task_t)();

void download()
{
    std::cout << "正在进行一个下载任务" << std::endl;
}
void Printlog()
{
    std::cout << "正在进行一个打印日志的任务" << std::endl;
}
void upload()
{
    std::cout << "正在进行一个上传的任务" << std::endl;
}

class TaskManager
{
public:
    TaskManager()
    {
        srand(time(nullptr));
    }

    void Register(task_t task)
    {
        _tasks.push_back(task);
    }

    int code()
    {
        return rand() % _tasks.size();
    }
    
    void Execute(const int &code)
    {
        assert(code >= 0 && code < _tasks.size());
        _tasks[code]();
    }

protected:
    std::vector<task_t> _tasks;
};