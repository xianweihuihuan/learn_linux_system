#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace std;
int main()
{
    int cnt = 0;
    int ptd[2] = {0};
    int p = pipe(ptd);
    if (p == -1)
    {
        cout << "Create pipe failure" << endl;
        exit(EXIT_FAILURE);
    }
    p = fork();
    if (p == 0)
    {
        char buffer[1024];
        close(ptd[0]);
        while (true)
        {
            snprintf(buffer, sizeof(buffer), "I am child,I say:my pid is %d,cnt=%d", getpid(), cnt++);
            write(ptd[1], buffer, strlen(buffer));
            sleep(1);
        }
    }
    else
    {
        char buffer[1024];
        while (true)
        {
            close(ptd[1]);
            int sz = read(ptd[0], buffer, sizeof(buffer) - 1);
            buffer[sz] = '\n';
            cout << buffer;
            cout << "father:cnt=" << cnt << endl;
            sleep(1);
        }
    }
}