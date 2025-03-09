#pragma once
#include<iostream>
#include<pthread.h>
#include"mutex.hpp"

namespace Xianwei{
    class cond{
    public:
        cond(){
            int n = pthread_cond_init(&_cond,nullptr);
            (void)n;
        }
        
        void wait(mutex& mtx){
            int n = pthread_cond_wait(&_cond,&mtx);
            (void)n;
        }

        void Notice(){
            int n = pthread_cond_signal(&_cond);
            (void)n;
        }

        void Noticeall(){
            int n = pthread_cond_broadcast(&_cond);
            (void)n;
        }

        ~cond(){
            int n = pthread_cond_destroy(&_cond);
            (void)n;
        }
        
    private:
        pthread_cond_t _cond;
    };
}