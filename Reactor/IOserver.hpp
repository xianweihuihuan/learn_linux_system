#pragma once
#include "connection.hpp"

namespace Xianwei
{
    class IOserver : public connection
    {
        class reactor;

    public:
        IOserver(int sockfd)
        {
            SetNonblock(sockfd);
            Setfd(sockfd);
            Setevent(EPOLLIN | EPOLLET);
        }

        void Send()
        {
            while (true)
            {
                ssize_t n = send(Fd(), outbuffer().c_str(), outbuffer().size(), 0);
                if (n > 0)
                {
                    clearoutbuf(n);
                }
                else if (n == 0)
                {
                    break;
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        break;
                    }
                    else if (errno == EINTR)
                    {
                        continue;
                    }
                    else
                    {
                        Excepter();
                        break;
                    }
                }
            }
            if (!Isoutbufempty())
            {
                auto test = Owner();
                Owner()->enableReadWrite(Fd(), true, true);
            }
            else
            {
                Owner()->enableReadWrite(Fd(), true, false);
            }
        }

        void Recv()
        {
            while (true)
            {
                char buffer[1024];
                ssize_t n = recv(Fd(), buffer, sizeof(buffer) - 1, 0);
                if (n > 0)
                {
                    buffer[n] = 0;
                    Appendin(buffer);
                }
                else if (n == 0)
                {
                    Excepter();
                    return;
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        break;
                    }
                    else if (errno == EINTR)
                    {
                        continue;
                    }
                    else
                    {
                        Excepter();
                        return;
                    }
                }
            }
            std::string result;
            if (_f)
            {
                result = _f(inbuffer());
                inbuffer().clear();
            }
            Appendout(result);
            if (!Isoutbufempty())
            {
                Owner()->enableReadWrite(Fd(), true, true);
            }
        }

        void Excepter()
        {
            Owner()->decare(Fd());
        }

        void Register(func_t f)
        {
            _f = f;
        }

        ~IOserver() = default;

    private:
        func_t _f;
    };
}