#include"comman.hpp"


int main(){
    readfifo fifo(".","fifo");
    int fd = open("./xianwei",O_APPEND|O_CREAT | O_WRONLY,0644);
    while(fifo.readfromfifo(fd)){
        
    }
    close(fd);
}