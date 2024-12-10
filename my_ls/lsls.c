#include "ls.h"

int main(int argc, char *argv[])
{
    extern int optopt;
    char filename[10][200];
    long filecount = 0;
    extern int mode[200];
    int tmp;
    while ((tmp = getopt(argc, argv, "alRtris::")) != EOF)
    {
        switch (tmp)
        {
        case 'a':
        case 'l':
        case 'R':
        case 't':
        case 'r':
        case 'i':
        case 's':
            mode[tmp]++;
            break;
        default:
            printf("没有这个选项：-%c\n", optopt);
            exit(EXIT_FAILURE);
        }
    }
    char ori[200];
    char *ttt = getcwd(ori, 200);
    if (ttt == NULL)
    {
        perror("obtain pwd fail");
        exit(EXIT_FAILURE);
    }
    tmp = 1;
    while (tmp < argc)
    {
        if (argv[tmp][0] != '-')
        {
            if (argv[tmp][0] == '/')
            {
                strcpy(filename[filecount], argv[tmp]);
                filecount++;
            }
            else
            {
                sprintf(filename[filecount], "%s/%s", ori, argv[tmp]);
                filecount++;
            }
        }
        tmp++;
    }
    if (filecount == 0)
    {
        ls(".");
    }
    else
    {
        for (int i = 0; i < 1; i++)
        {
            printf("%s:\n", filename[i]);
            ls(filename[i]);
        }
    }
    return 0;
}
