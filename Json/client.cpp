#include <sstream>
#include <jsoncpp/json/json.h>
#include "socket.hpp"


int main(int argc,char* argv[]){
    std::string ip = argv[1];
    uint16_t port = htons(std::stoi(argv[2]));
    int fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in ttt;
    socklen_t len = sizeof(ttt);
    memset(&ttt,0,len);
    ttt.sin_addr.s_addr = inet_addr(ip.c_str());
    ttt.sin_family = AF_INET;
    ttt.sin_port = port;
    connect(fd,(sockaddr*)&ttt,len);
    while(true){
        std::string one;
        std::cin>>one;
        std::string two;
        std::cin>>two;
        Json::Value root;
        root["one"] = one;
        root["two"] = two;
        std::string ss;
        Json::FastWriter writer;
        ss = writer.write(root);
        send(fd,ss.c_str(),ss.size(),0);
        char buffer[1024];
        int n = recv(fd,buffer,sizeof(buffer)-1,0);
        if(n > 0){
            buffer[n] = 0;
            std::string ans = buffer;
            Json::Reader reader;
            Json::Value an;
            if(reader.parse(ans,an)){
                std::string out = an["answer"].asString();
                std::cout<<out<<std::endl;
            }
        }
    }
}