#pragma once
#include <string>
#include <unordered_map>
#include "epoller.hpp"
#include <memory>
#include "connection.hpp"
#include <map>

namespace Xianwei
{
    using connect_t = std::shared_ptr<connection>;
    const int ev_nums = 64;
    class reactor
    {
    private:
        bool Isexist(int fd)
        {
            return _cn.find(fd) != _cn.end();
        }

    public:
        reactor()
            : _isrunning(false),
              _ep(std::make_unique<Epoller>())
        {
            _ep->init();
        }

        void insertcn(connect_t cn)
        {
            auto pos = _cn.find(cn->Fd());
            if (pos == _cn.end())
            {
                _cn.insert(std::make_pair(cn->Fd(), cn));
                _ep->Add(cn->Fd(), cn->Events());
                cn->Setowner(this);
            }
        }

        void enableReadWrite(int sockfd, bool read, bool write)
        {
            if (Isexist(sockfd))
            {
                uint32_t event = ((read ? EPOLLIN : 0) | (write ? EPOLLOUT : 0) | EPOLLET);
                _cn[sockfd]->Setevent(event);
                _ep->Update(sockfd, event);
            }
        }

        void decare(int fd)
        {
            if (Isexist(fd))
            {
                _ep->Del(fd);
                LOG(INFO)<<"不再关心fd："<<fd;
                _cn[fd]->Close();
                LOG(INFO)<<"关闭fd："<<fd;
                _cn.erase(fd);
                LOG(INFO)<<"将fd："<<fd<<"从连接中移除";
            }
        }

        void Loop()
        {
            _isrunning = true;
            while (_isrunning)
            {
                int n = _ep->Wiat(_rev, ev_nums, -1);
                print(n);
                Dispatcher(n);
            }
            _isrunning = false;
        }

        void Dispatcher(int n)
        {
            for (int i = 0; i < n; i++)
            {
                int sd = _rev[i].data.fd;
                uint32_t revents = _rev[i].events;
                if ((revents & EPOLLERR) || (revents & EPOLLHUP))
                {
                    revents = (EPOLLIN | EPOLLOUT);
                }
                if ((revents & EPOLLIN) && Isexist(sd))
                {
                    _cn[sd]->Recv();
                }
                if ((revents & EPOLLOUT) && Isexist(sd))
                {
                    _cn[sd]->Send();
                }
            }
        }

        void print(int n)
        {
            std::cout << "管理的文件描述符:";
            for (auto& x : _cn)
            {
                std::cout<<x.second->Fd()<<" ";
            }
            std::cout << std::endl;
        }
        ~reactor() = default;

    private:
        std::unique_ptr<Epoller> _ep;
        std::unordered_map<int, connect_t> _cn;
        bool _isrunning;
        epoll_event _rev[ev_nums];
    };
}