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
            if(argv[tmp][0]=='/'){
                strcpy(filename[filecount],argv[tmp]);
                filecount++;
            }else{
                snprintf(filename[filecount],MAX_PATH,"%s/%s",ori,argv[tmp]);
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
        for (int i = 0; i < filecount; i++)
        {   

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
