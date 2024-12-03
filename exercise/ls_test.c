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
        char **filename = (char **)calloc(_PC_PATH_MAX + 1, 100);
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
        tmp = 1;
        while (tmp < argc)
        {
            if (argv[tmp][0] != '-')
            {
                // if(argv[tmp][0]!='/'){
                //     realpath(argv[tmp],filename[filecount]);
                // }else{
                filename[filecount] = argv[tmp];
                //}
                filecount++;
            }
            tmp++;
        }
        if (filecount > 1)
        {
            for (int i = 0; i < filecount; i++)
            {
                printf("%s:\n", filename[i]);
                ls(filename, i);
                printf("\n");
            }
        }
        else
        {
            int i = 0;
            ls(filename, i);
        }
    }
}
