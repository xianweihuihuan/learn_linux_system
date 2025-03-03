#include "shmtest—Comman.hpp"

int main()
{
    shm sh(CREATER, 4096);
    namedfifo fifo;
    readfifo rd;
    char *buf = (char *)sh.getshm();
    while (1)
    {
        if (rd.readfromfifo(1))
        {
            printf("Client say:%s\n", buf);
        }
        else
        {
            break;
        }
    }
}