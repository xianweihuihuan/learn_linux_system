#pragma once
#include "connection.hpp"
#include "IOserver.hpp"
#include "common.hpp"

namespace Xianwei
{
    class Listener : public connection
    {
    public:
        Listener(int port)
            : _port(port),
              _listensock(std::make_unique<TCP_Server>())
        {
            _listensock->build(_port);
            Setfd(_listensock->Fd());
            SetNonblock(_listensock->Fd());
            Setevent(EPOLLIN | EPOLLET);
        }

        void Recv()
        {
            while (true)
            {
                InetAddr peer;
                int newfd = _listensock->Accept(&peer);
                if (newfd > 0)
                {
                    LOG(INFO) << "收到一个新连接fd:" << newfd;
                    auto conn = std::make_shared<IOserver>(newfd);
                    conn->Setfd(newfd);
                    Owner()->insertcn(conn);
                    conn->Register(test);
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        LOG(INFO) << "收到了所有的连接";
                        break;
                    }
                    else if (errno == EINTR)
                    {
                        LOG(INFO) << "接收被信号中断";
                        continue;
                    }
                    else
                    {
                        LOG(ERROR) << "接收错误";
                        break;
                    }
                }
            }
        }

        void Send() {}

        void Excepter() {}

        int Sockfd()
        {
            return _listensock->Fd();
        }

        ~Listener()
        {
            _listensock->Close();
        }

    private:
        std::unique_ptr<Socket> _listensock;
        int _port;
    };

}