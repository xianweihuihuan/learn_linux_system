#pragma once
#include "Log.hpp"
#include <string>
#include "InetAddr.hpp"
#include "socket.hpp"
#include "common.hpp"
#include <functional>
#include "Reactor.hpp"

namespace Xianwei
{
    class reactor;
    using func_t = std::function<std::string(std::string &)>;
    class connection
    {
    public:
        connection()
            : _sockfd(-1),
              _event(0)
        {
        }

        void Setfd(int fd)
        {
            _sockfd = fd;
        }

        void Setevent(uint32_t events)
        {
            _event = events;
        }

        void Setowner(reactor *owner)
        {
            _owner = owner;
        }

        int Fd()
        {
            return _sockfd;
        }

        uint32_t Events()
        {
            return _event;
        }

        reactor *Owner()
        {
            return _owner;
        }

        void Appendin(const std::string &in)
        {
            _inbuffer += in;
        }

        void Appendout(const std::string &out)
        {
            _outbuffer += out;
        }

        void Close()
        {
            if (_sockfd > 0)
            {
                close(_sockfd);
            }
        }

        void clearoutbuf(int n)
        {
            _outbuffer.erase(0, n);
        }

        bool Isoutbufempty()
        {
            return _outbuffer.empty();
        }

        std::string &outbuffer()
        {
            return _outbuffer;
        }

        std::string &inbuffer()
        {
            return _inbuffer;
        }

        virtual void Send() = 0;
        virtual void Recv() = 0;
        virtual void Excepter() = 0;
        virtual ~connection() = default;

    private:
        int _sockfd;
        std::string _inbuffer;
        std::string _outbuffer;
        InetAddr _peer_addr;
        reactor *_owner;
        uint32_t _event;
    };

}