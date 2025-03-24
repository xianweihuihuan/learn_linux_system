#pragma once
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Xianwei
{
    class InetAddr
    {
    private:
        void Portntohs(){
            _port = ntohs(_addr.sin_port);
        }
        
        void Ipntohp(){
            char buffer[1024];
            const char* ip = inet_ntop(AF_INET,&_addr.sin_addr,buffer,sizeof(buffer)); 
            _ip = buffer;
        }

    public:
        InetAddr(uint16_t port)
            :_ip(""),
             _port(port)
        { 
            _addr.sin_family = AF_INET;
            _addr.sin_port = htons(_port);
            _addr.sin_addr.s_addr = INADDR_ANY;
        }

        InetAddr(const struct sockaddr_in& addr)
            :_addr(addr)
        {
            Portntohs();
            Ipntohp();
        }

        InetAddr(){

        }
        struct sockaddr* Netaddr(){
            return (struct sockaddr*)&_addr;
        }

        socklen_t len(){
            return sizeof(_addr); 
        }

        std::string Ip(){
            return _ip;
        }

        uint16_t port(){
            return _port;
        }
        
        void Setaddr(struct sockaddr_in cp){
            _addr = cp;
            Portntohs();
            Ipntohp();
        }
    private:
        std::string _ip;
        uint16_t _port;
        struct sockaddr_in _addr; 
    };
}