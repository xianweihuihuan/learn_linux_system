#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "InetAddr.hpp"
#include <cstring>
#include <unistd.h>
#include <memory>
#include "Log.hpp"

namespace Xianwei
{
    class Socket
    {
    public:
        Socket() = default;
        virtual ~Socket() = default;
        virtual void socketordie() = 0;
        virtual bool bindordie(int port) = 0;
        virtual bool listenordie() = 0;
        virtual int Accept(InetAddr *client) = 0;
        virtual int Recv(std::string *out) = 0;
        virtual int Send(const std::string &in) = 0;
        virtual void Close() = 0;
        virtual int Fd() = 0;
        void build(int port)
        {
            socketordie();
            bindordie(port);
            listenordie();
        }
    };

    class TCP_Server : public Socket
    {
    public:
        TCP_Server()
            : _sockfd(-1)
        {
        }

        TCP_Server(int newfd)
            : _sockfd(newfd)
        {
        }

        void socketordie()
        {
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if(_sockfd < 0){
                LOG(ERROR)<<"TCP SOCKET create error";
            }
            LOG(INFO)<<"TCP SOCKET create success"<<_sockfd;
        }

        bool bindordie(int port)
        {
            if(_sockfd == -1){
                return false;
            }
            InetAddr se(port);
            int n = bind(_sockfd, se.Netaddr(), se.len());
            if (n < 0)
            {
                LOG(ERROR)<<"bind error";
                return false;
            }
            LOG(INFO)<<"bind success";
            return true;
        }

        bool listenordie()
        {
            int n = listen(_sockfd, 8);
            if (n < 0)
            {
                LOG(ERROR)<<"listen error";
                return false;
            }
            LOG(INFO)<<"begin listen";
            return true;
        }

        int Accept(InetAddr *client)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int newfd = accept(_sockfd, (struct sockaddr *)&peer, &len);
            client->Setaddr(peer);
            return newfd;
        }

        int Recv(std::string *out)
        {
            char buffer[1024];
            int n = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                buffer[n] = 0;
                *out = buffer;
            }
            return n;
        }

        int Send(const std::string &in)
        {
            int n = send(_sockfd, in.c_str(), in.size(), 0);
            return n;
        }

        void Close()
        {
            close(_sockfd);
        }

        int Fd()
        {
            return _sockfd;
        }
        ~TCP_Server() = default;

    private:
        int _sockfd;
    };
}
