#pragma once
#include <iostream>
#include <pthread.h>

namespace Xianwei
{
    class mutex
    {
    public:
        mutex(const mutex &tmp) = delete;

        const mutex &operator=(const mutex &tmp) = delete;

        mutex()
        {
            int n = pthread_mutex_init(&_lock, nullptr);
            (void)n;
        }

        void lock()
        {
            int n = pthread_mutex_lock(&_lock);
            (void)n;
        }

        void unlock()
        {
            int n = pthread_mutex_unlock(&_lock);
            (void)n;
        }

        ~mutex()
        {
            int n = pthread_mutex_destroy(&_lock);
            (void)n;
        }

        pthread_mutex_t *operator&()
        {
            return &_lock;
        }

    private:
        pthread_mutex_t _lock;
    };

    class mutexmaster
    {
    public:
        mutexmaster(mutex &mtx)
            : _mtx(mtx)
        {
            mtx.lock();
        }

        ~mutexmaster()
        {
            _mtx.unlock();
        }

    private:
        mutex &_mtx;
    };
}
