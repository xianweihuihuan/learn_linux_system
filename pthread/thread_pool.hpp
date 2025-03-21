#include "Pool_thread.hpp"
#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include "mutex.hpp"
#include "cond.hpp"
#include <memory>
#include "Log.hpp"

namespace Xianwei
{
    
    using Pthread_t = std::shared_ptr<pthread>;
    template <class T>
    class thread_pool
    {
    private:
        bool IsEmpty()
        {
            return _tq.empty();
        }

        void task()
        {
            ENABLE_FILE();
            while (true)
            {
                T t;
                {
                    mutexmaster mtx(_lock);
                    while (IsEmpty() && _isrunning)
                    {
                        _waitnums++;
                        _cd.wait(_lock);
                        _waitnums--;
                    }
                    if (IsEmpty() && !_isrunning)
                    {
                        break;
                    }
                    t = _tq.front();
                    _tq.pop();
                }
                t();
            }
        }

    public:
        thread_pool(int num = 5)
            : _num(num), _isrunning(false), _waitnums(0)
        {
            ENABLE_FILE();
            for (int i = 0; i < _num; i++)
            {
                _thread.emplace_back(std::make_shared<pthread>(std::bind(&thread_pool::task, this)));
            }
        }

        void Stop()
        {
            mutexmaster mtx(_lock);
            if (_isrunning)
            {
                _isrunning = false;
                if (_waitnums > 0)
                {
                    _cd.Noticeall();
                }
            }
        }

        void start()
        {
            if (_isrunning)
            {
                return;
            }
            _isrunning = true;
            for (auto &x : _thread)
            {
                x->Start();
            }
        }

        void push_task(const T &task)
        {
            mutexmaster mtx(_lock);
            _tq.push(std::move(task));
            if (_waitnums)
            {
                _cd.Notice();
            }
        }

        void Wait()
        {
            ENABLE_FILE();
            for (auto &x : _thread)
            {
                x->join();
            }
        }

        ~thread_pool() = default;

    private:
        std::queue<T> _tq;
        std::vector<Pthread_t> _thread;
        mutex _lock;
        cond _cd;
        int _num;
        bool _isrunning;
        int _waitnums;
    };
}
