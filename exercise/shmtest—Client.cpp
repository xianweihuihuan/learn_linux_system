#include "shmtest—Comman.hpp"

int main()
{
    shm sh(USER, 4096);
    writefifo wr;
    char *buf = (char *)sh.getshm();
    int len = 0;
    for (char i = 'A'; i <= 'Z'; i++)
    {
        sleep(1);
        buf[len++] = i;
        buf[len++] = i;
        buf[len] = '\0';
        char *tmp = "##############0";
        wr.writetofifo(tmp, strlen(tmp));
    }
}