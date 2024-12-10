#include "my_ls.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        ls_none(argc, argv);
        return 0;
    }
    else
    {
        extern int optopt;
        char **filename = (char **)malloc(100 * sizeof(char *));
        for (int i = 0; i < 100; i++)
        {
            filename[i] = (char *)malloc(200);
        }
        char **filename1 = (char **)malloc(100 * sizeof(char *));
        for (int i = 0; i < 100; i++)
        {
            filename1[i] = (char *)malloc(200);
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
                break;
            }
        }
        char ori[200];
        getcwd(ori, 200);
        tmp = 1;
        while (tmp < argc)
        {
            if (argv[tmp][0] != '-')
            {
                strcpy(filename1[filecount], argv[tmp]);
                chdir(argv[tmp]);
                char ttmmpp[200];
                getcwd(ttmmpp, 200);
                strcpy(filename[filecount], ttmmpp);
                filecount++;
                chdir(ori);
            }
            tmp++;
        }
        if (filecount > 1)
        {
            for (int i = 0; i < filecount; i++)
            {
                ls(filename,filename1, i,filecount);
                printf("\n");
            }
        }
        else
        {
            int i = 0;
            ls(filename, filename1,i,filecount);
        }
        for(int i = 0;i<100;i++){
            free(filename[i]);
            free(filename1[i]);
        }
        free(filename1);
        free(filename);
    }
}

