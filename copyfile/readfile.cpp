#include"comman.hpp"


int main(){
    namedfifo ff(".","fifo");
    std::string in;
    std::cout<<"请输入要拷贝的文件名：";
    std::cin>>in;
    int fd = open(in.c_str(),O_RDONLY);
    if(fd < 0){
        EXIT_ERR("open file")
    }
    writefifo fifo(".","fifo");
    char buffer[1024];
    int n = 0;
    while(n = read(fd,buffer,sizeof(buffer))){
        //std::cout<<buffer;
        fifo.writetofifo(buffer,n);
    }
    close(fd);
}