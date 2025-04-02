#pragma once
#include <pthread.h>
#include <iostream>
#include <string>
#include <functional>
#include <unistd.h>

namespace Xianwei
{
    static int number = 1;
    enum STAT
    {
        NEW,
        RUNNING,
        STOP
    };
    class pthread
    {
        using func_t = std::function<void()>;

    private:
        static void *done(void *arge)
        {
            pthread *p = static_cast<pthread *>(arge);
            p->_stat = RUNNING;
            p->_func();
            return nullptr;
        }

    public:
        static void in()
        {
            std::cout << "Not deliver a task" << std::endl;
        }

        pthread(func_t func = in)
            :  _func(func), _joinable(true), _stat(NEW)
        {
            _pid = getpid();
            _name = "Thread" + std::to_string(number++);
        }

        bool Start()
        {
            if (_stat != RUNNING)
            {
                int n = pthread_create(&_tid, nullptr, done, this);
                if (n != 0)
                {
                    perror("Create thread fail");
                    return false;
                }
                return true;
            }
            perror("This thread is running");
            return false;
        }

        bool stop()
        {
            if (_stat == RUNNING)
            {
                int n = pthread_cancel(_tid);
                if (n != 0)
                {
                    perror("This thread cancel fail");
                    return false;
                }
                return true;
            }
            perror("This thread is not running");
            return false;
        }

        bool join()
        {
            if (_joinable)
            {
                int n = pthread_join(_tid, nullptr);
                if (n != 0)
                {
                    perror("jion thread fail");
                    return false;
                }
                return true;
            }
            perror("this thread don't need join");
            return false;
        }

        bool Detach()
        {
            _joinable = false;
            pthread_detach(_tid);
            return true;
        }

        std::string Getname(){
            return _name;
        }

        ~pthread()
        {
        }

    private:
        pthread_t _tid;
        std::string _name;
        pid_t _pid;
        bool _joinable;
        STAT _stat;
        func_t _func;
    };
}