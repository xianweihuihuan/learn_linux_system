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
        char **filename = (char **)calloc(_PC_PATH_MAX+1, 100);
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
                //if(argv[tmp][0]!='/'){
                //    realpath(argv[tmp],filename[filecount]);
                //}else{
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
                DIR *dir;
                chdir(filename[i]);
                char tmpd[100];
                char *tmptmp = getcwd(tmpd, 100);
                dir = opendir(tmpd);               
                printf("%s:\n", filename[i]);
                struct fm *name = (struct fm *)malloc(10 * sizeof(struct fm));
                int sz = 10;
                int count = 0;
                struct dirent *rnm;
                while ((rnm = readdir(dir)) != NULL)
                {
                    if (count == sz)
                    {
                        int newn = 2 * sz;
                        name = (struct fm *)realloc(name, newn * sizeof(struct fm));
                        sz = newn;
                    }
                    name[count].file = *rnm;
                    stat(rnm->d_name, &name[count].stat);
                    count++;
                }
                if (mode['R'] == 0)
                {
                    if (mode['t'] != 0)
                    {
                        qsort(name, count, sizeof(struct fm), &sort_time);
                    }
                    else
                    {
                        qsort(name, count, sizeof(struct fm), &sort1);
                    }
                    if (mode['l'] == 0)
                    {
                        ls_normal(name, count);
                    }
                    else
                    {
                        ls_l(name, count);
                    }
                    printf("\n");
                    free(name);
                    name = NULL;
                    count = 0;
                    closedir(dir);
                }
            }
        }
        else
        {
            DIR *dir;
            if (filecount == 0)
            {
                char tmpd[100];
                char *tmptmp = getcwd(tmpd, 100);
                dir = opendir(tmpd);
            }
            else
            {   
                chdir(filename[0]);
                char tmpd[100];
                char *tmptmp = getcwd(tmpd, 100);
                dir = opendir(tmpd);
            }
            struct fm *name = (struct fm *)malloc(10 * sizeof(struct fm));
            int sz = 10;
            int count = 0;
            struct dirent *rnm;
            while ((rnm = readdir(dir)) != NULL)
            {
                if (count == sz)
                {
                    int newn = 2 * sz;
                    name = (struct fm *)realloc(name, newn * sizeof(struct fm));
                    sz = newn;
                }
                name[count].file = *rnm;
                stat(rnm->d_name, &name[count].stat);
                count++;
            }
            if (mode['R'] == 0)
            {
                if (mode['t'] != 0)
                {
                    qsort(name, count, sizeof(struct fm), &sort_time);
                }
                else
                {
                    qsort(name, count, sizeof(struct fm), &sort1);
                }
                if (mode['l'] == 0)
                {
                    ls_normal(name, count);
                }
                else
                {
                    ls_l(name, count);
                }
            }
            else
            {
                if (mode['t'] != 0)
                {
                    qsort(name, count, sizeof(struct fm), &sort_time);
                }
                else
                {
                    qsort(name, count, sizeof(struct fm), &sort1);
                }
                if (mode['l'] == 0)
                {
                    ls_R(name, count);
                }
                else
                {
                    ;
                }
            }
        }
    }
}
