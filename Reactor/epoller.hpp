#pragma once
#include "Log.hpp"
#include <sys/epoll.h>

namespace Xianwei
{
    class Epoller
    {
    private:
        int Ctrl(int sockfd, uint32_t event, int flag)
        {
            epoll_event ev;
            ev.data.fd = sockfd;
            ev.events = event;
            int n = epoll_ctl(_epfd, flag, sockfd, &ev);
            return n;
        }

    public:
        Epoller()
            : _epfd(-1)
        {
        }

        void init()
        {
            _epfd = epoll_create(256);
            if (_epfd < 0)
            {
                LOG(Xianwei::FATAL) << "epoll create error";
                exit(1);
            }
            LOG(INFO) << "epoll create success";
        }

        int Wiat(epoll_event revs[], int num, int timeout)
        {
            int n = epoll_wait(_epfd, revs, num, timeout);
            if (n < 0)
            {
                LOG(ERROR) << "epoll wait error";
            }
            return n;
        }
        
        void Add(int sockfd, uint32_t event)
        {
            int n = Ctrl(sockfd, event, EPOLL_CTL_ADD);
            if (n < 0)
            {
                LOG(ERROR) << "epoll add error";
            }
        }

        void Update(int sockfd, uint32_t event)
        {
            int n = Ctrl(sockfd, event, EPOLL_CTL_MOD);
            if (n < 0)
            {
                LOG(ERROR) << "epoll update error";
            }
        }

        void Del(int sockfd)
        {
            int n = epoll_ctl(_epfd, EPOLL_CTL_DEL, sockfd, nullptr);
            if (n < 0)
            {
                LOG(ERROR) << "epoll del error";
            }
        }

        ~Epoller() = default;

    private:
        int _epfd;
    };
}