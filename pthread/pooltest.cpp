#include "thread_pool.hpp"
#include "Task.hpp"
#include <unistd.h>

int main()
{
    char buffer[102];
    getcwd(buffer,sizeof(buffer));
    std::cout<<buffer<<std::endl;
    std::unique_ptr<Xianwei::thread_pool<task_t>> tp = std::make_unique<Xianwei::thread_pool<task_t>>();
    tp->start();

    int cnt = 10;
    char c;
    while (cnt--)
    {
        tp->push_task(Push);
        sleep(1);
    }
    tp->Stop();
    sleep(3);
    tp->Wait();
}