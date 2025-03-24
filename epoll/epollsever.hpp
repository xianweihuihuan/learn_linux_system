#include "socket.hpp"
#include <sys/epoll.h>
#include <memory>
#include <string>
#include <iostream>
#include "Log.hpp"

const int defaultfd = -1;
const int revs_num = 64;

namespace Xianwei
{
    class epollserver
    {
    public:
        epollserver(int port)
            : _port(port),
              _isrunning(false),
              _epfd(-1),
              _listen_socket(std::make_unique<TCP_Server>())
        {
        }
        void init()
        {
            _listen_socket->build(_port);
            _epfd = epoll_create(256); // 为epoll模型分配文件描述符
            if (_epfd < 0)
            {
                LOG(ERROR) << "epoll create error";
                exit(1);
            }
            LOG(DEBUG) << "epoll create success";
            // 将listen套接字加入epoll模型
            epoll_event ln;
            ln.events = EPOLLIN;
            ln.data.fd = _listen_socket->Fd();
            int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, _listen_socket->Fd(), &ln);
            if (n < 0)
            {
                LOG(ERROR) << "epoll_ctl error";
                exit(1);
            }
        }

        void loop()
        {
            int timeout = 500;
            _isrunning = true;
            while (_isrunning)
            {
                int n = epoll_wait(_epfd, _revs, revs_num, timeout);
                switch (n)
                {
                case 0:
                    std::cout << "time out" << std::endl;
                    break;
                case -1:
                    LOG(ERROR) << "epoll_wait error";
                    break;
                default:
                    LOG(INFO) << "有事件就绪";
                    Dispatcher();
                    break;
                }
            }
        }

        void Accept(int fd)
        {
            InetAddr peer;
            int newfd = _listen_socket->Accept(&peer);
            if (newfd < 0)
            {
                return;
            }
            else
            {
                LOG(INFO) << "获得了一个新连接fd：" << newfd;
                epoll_event ev;
                ev.data.fd = newfd;
                ev.events = EPOLLIN;
                int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, newfd, &ev);
                if (n < 0)
                {
                    LOG(ERROR) << "epoll ctr error";
                    close(newfd);
                }
                LOG(INFO) << "epoll add success:" << newfd;
            }
        }

        void Recv(int fd)
        {
            char buffer[1024];
            int n = recv(fd, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                buffer[n] = 0;
                std::string message = "echo#";
                message += buffer;
                send(fd, message.c_str(), message.size(), 0);
                LOG(INFO) << "收到了一条消息：" << buffer;
            }
            else if (n == 0)
            {
                LOG(INFO) << "客户端" << fd << "退出";
                int m = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
                if (m < 0)
                {
                    LOG(ERROR) << "epoll ctr error";
                    exit(1);
                }
                close(fd);
            }
            // else if (n < 0)
            // {
            //     LOG(ERROR) << "读取错误";
            //     int m = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
            //     if (m < 0)
            //     {
            //         LOG(ERROR) << "epoll ctr error";
            //         return;
            //     }
            //     close(fd);
            // }
        }
        void Dispatcher()
        {
            for (int i = 0; i < revs_num; i++)
            {
                int fd = _revs[i].data.fd;
                int event = _revs[i].events;
                if (event & EPOLLIN)
                {
                    if (fd == _listen_socket->Fd())
                    {
                        Accept(fd);
                    }
                    else
                    {
                        Recv(fd);
                    }
                }
            }
        }

    private:
        uint16_t _port;
        std::unique_ptr<Socket> _listen_socket;
        bool _isrunning;
        int _epfd;
        struct epoll_event _revs[revs_num];
    };
}