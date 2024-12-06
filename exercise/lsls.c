#include "ls.h"

int main(int argc, char *argv[])
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
    if (mode['R'] == 0)
    {
        if (filecount == 0)
        {
            ls_n();
        }
        else
        {
            for(int i = 0;i<filecount;i++){
                ls_m(filename[i],filename1[i]);
            }
        }
    }else{
        if(filecount==0){
            ls_Rn();
        }else{
            int i =0;
            //for(int i = 0;i<filecount;i++){
                ls_Rm(filename[i],filename1[i]);
            //}
        }
    }
}