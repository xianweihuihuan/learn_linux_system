#include "ls.h"

int main(int argc, char *argv[])
{
    extern int optopt;
    char **filename = (char **)malloc(100 * sizeof(char *));
    for (int i = 0; i < 100; i++)
    {
        filename[i] = (char *)malloc(MAX_PATH);
    }
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
        case '?':
            printf("没有这个选项：-%c\n", optopt);
            exit(EXIT_FAILURE);
        }
    }
    char ori[200];
    getcwd(ori, 200);
    tmp = 1;
    while (tmp < argc)
    {
        if (argv[tmp][0] != '-')
        {
            chdir(argv[tmp]);
            char ttmmpp[200];
            char *iii = getcwd(ttmmpp, 200);
            if (iii == NULL)
            {
                perror("obtain pwd failed");
                exit(EXIT_FAILURE);
            }
            strcpy(filename[filecount], ttmmpp);
            filecount++;
            int o = chdir(ori);
            if (o == -1)
            {
                perror("failed chdir");
                exit(EXIT_FAILURE);
            }
        }
        tmp++;
    }
    if (filecount == 0)
    {
        char ori[MAX_PATH];
        char *rt = getcwd(ori, MAX_PATH);
        if (rt == NULL)
        {
            perror("obtain pwd failed");
            exit(EXIT_FAILURE);
        }
        ls(ori);
    }
    else
    {
        for (int i = 0; i < filecount; i++)
        {
            int u = chdir(filename[i]);
            if (u == -1)
            {
                perror("failed chdir");
                exit(EXIT_FAILURE);
            }
            printf("%s:\n", filename[i]);
            ls(filename[i]);
        }
    }
    for (int i = 0; i < 100; i++)
    {
        free(filename[i]);
    }
    free(filename);
}
