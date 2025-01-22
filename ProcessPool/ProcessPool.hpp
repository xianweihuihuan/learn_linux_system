#include <iostream>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "Task.hpp"
#include <cstdlib>
#include <vector>
#include <string>
#include <assert.h>

class Channel
{
public:
    Channel(const int &wfd, const pid_t &pid)
        : _wfd(wfd), _pid(pid)
    {
        _name = "channel-" + std::to_string(_pid) + "-" + std::to_string(wfd);
    }

    void Send(int code)
    {
        int n = write(_wfd, &code, sizeof(code));
        (void)n;
    }

    std::string &Name()
    {
        return _name;
    }

    int Fd()
    {
        return _wfd;
    }

    pid_t SubId()
    {
        return _pid;
    }

    void Closewfd()
    {
        close(_wfd);
    }

    void Waitpro()
    {
        waitpid(_pid, nullptr, 0);
    }

protected:
    int _wfd;
    pid_t _pid;
    std::string _name;
};

class ChannelManager
{
public:
    ChannelManager()
        : _next(0)
    {
    }

    void insert(int wfd, int pid)
    {
        _channels.emplace_back(wfd, pid);
    }

    Channel &Select()
    {
        auto &e = _channels[_next];
        _next++;
        _next %= _channels.size();
        return e;
    }

    void PrintChannel()
    {
        for (auto &channel : _channels)
        {
            std::cout << channel.Name() << std::endl;
        }
    }

    void closeawfd()
    {
        for (auto &e : _channels)
        {
            e.Closewfd();
            std::cout << "关闭: " << e.Name() << std::endl;
        }
    }

    void waitalpro()
    {
        for (auto &e : _channels)
        {
            e.Waitpro();
            std::cout << "回收: " << e.Name() << std::endl;
        }
    }

protected:
    std::vector<Channel> _channels;
    int _next;
};

class ProcessPool
{
public:
    ProcessPool(int num)
        : _Process_num(num)
    {
        _tm.Register(download);
        _tm.Register(upload);
        _tm.Register(Printlog);
    }

    void Start()
    {
        for (int i = 0; i < _Process_num; i++)
        {
            int pipefd[2] = {0};
            int n = pipe(pipefd);
            if (n < 0)
            {
                perror("Create pipe failed");
                exit(EXIT_FAILURE);
            }
            pid_t cid = fork();
            if (cid < 0)
            {
                perror("Create process failed");
                exit(EXIT_FAILURE);
            }
            else if (cid == 0)
            {
                close(pipefd[1]);
                Work(pipefd[0]);
                exit(EXIT_SUCCESS);
            }
            else
            {
                close(pipefd[0]);
                _pool.insert(pipefd[1], cid);
            }
        }
    }

    void Work(int rfd)
    {
        while (true)
        {
            int code = 0;
            int sz = read(rfd, &code, sizeof(code));
            if (sz > 0)
            {
                if (sz != sizeof(code))
                {
                    continue;
                }
                std::cout << "子进程[" << getpid() << "]收到一个任务码: " << code << std::endl;
                _tm.Execute(code);
                        }
            else if (sz == 0)
            {
                std::cout << "父进程退出" << std::endl;
                break;
            }
            else
            {
                std::cout << "读取失败" << std::endl;
                break;
            }
        }
    }

    void Debug()
    {
        _pool.PrintChannel();
    }

    void Run()
    {
        int taskcode = _tm.code();
        auto &c = _pool.Select();
        std::cout << "选择了一个子进程: " << c.Name() << std::endl;
        c.Send(taskcode);
        std::cout << "发送了一个任务码: " << taskcode << std::endl;
    }

    void End()
    {
        _pool.closeawfd();
        _pool.waitalpro();
    }

protected:
    ChannelManager _pool;
    int _Process_num;
    TaskManager _tm;
};