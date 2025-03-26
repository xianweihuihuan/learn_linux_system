#pragma once
#include <fcntl.h>
#include <string>

namespace Xianwei{
    void SetNonblock(int sockfd){
        int fl = fcntl(sockfd,F_GETFL);
        if(fl < 0){
            return;
        }
        fcntl(sockfd,F_SETFL,fl|O_NONBLOCK);
    }

    std::string test(std::string& in){
        std::string message = "echo#";
        message+=in;
        return message;
    }
}
