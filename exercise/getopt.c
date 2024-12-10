#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    /*argv[1] = "-m";
    int tmp = getopt(argc,argv,"a::bc::e");
    printf("%c",tmp);*/
    int ch;
    extern char *optarg;
    extern int optopt;
    while ((ch = getopt(argc, argv, "a:bc::d::e:")) != EOF)
    {
        switch (ch)
        {
        case 'a':
            printf("执行了%c选项，参数为%s\n", ch, optarg);
            break;
        case 'b':
            printf("执行了%c选项，参数为%s\n", ch, optarg);
            break;
        case 'c':
            printf("执行了%c选项，参数为%s\n", ch, optarg);
            break;
        case 'd':
            printf("执行了%c选项，参数为%s\n", ch, optarg);
            break;
        case 'e':
            printf("执行了%c选项，参数为%s\n", ch, optarg);
            break;
        case '?':
            printf("%c不是一个选项\n", optopt);
            break;
        }
    }
}