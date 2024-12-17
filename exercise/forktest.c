#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int cnt = 10;
    pid_t id = fork();
    if (id == 0)
    {
        while (cnt)
        {
            printf("我是子进程，pid:%d,ppid:%d,cnt=%d\n", getpid(), getppid(), cnt);
            cnt--;
            sleep(1);
        }
    }
    else
    {

        while (1)
        {
            printf("我是父进程，pid:%d,ppid:%d,cnt=%d\n", getpid(), getppid(), cnt);
            sleep(1);
        }
    }
}