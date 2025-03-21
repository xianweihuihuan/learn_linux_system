#pragma once
#include<iostream>
#include<semaphore.h>


namespace Xianwei{
    class sem{
    public:
        sem(int value)
            :_init_value(value)
        {
            int n = sem_init(&_sem,0,_init_value);
            (void)n;
        }

        void P(){
            int n = sem_wait(&_sem);
            (void)n;
        }

        void V(){
            int n = sem_post(&_sem);
            (void)n;
        }

        ~sem(){
            int n = sem_destroy(&_sem);
            (void)n;
        }
    private:
        sem_t _sem;
        int _init_value;
    };
}