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
    template <class T>
    class pthread
    {
        using func_t = std::function<void(T)>;

    private:
        static void *done(void *arge)
        {
            pthread<T> *p = static_cast<pthread<T> *>(arge);
            p->_stat = RUNNING;
            p->_func(p->_data);
            return nullptr;
        }

    public:
        void in(T)
        {
            std::cout << "Not deliver a task" << std::endl;
        }

        pthread(T data, func_t func = in)
            : _data(data), _func(func), _joinable(true), _stat(NEW)
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
        T _data;
    };
}