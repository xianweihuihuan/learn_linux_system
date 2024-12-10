#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include <getopt.h>
#define MAX_PATH 1000
//信息结构体
typedef struct
{
    char *filename;
    struct stat info;
} Fileinfo;

void do_ls(char *dirname);
void mode_to_letters(mode_t num, char *mode);
void print_fileinfo(Fileinfo fileinfo);
void print_filename(char *filename, mode_t filemode);
int compare(const void *a, const void *b);
int compare_t(const void *a, const void *b);
//附带参数
bool para_a = false;
bool para_l = false;
bool para_R = false;
bool para_t = false;
bool para_r = false;
bool para_i = false;
bool para_s = false;
int main(int argc, char **argv)
{
    char opt;
    while ((opt = getopt(argc, argv, "alisRrt")) != -1)
    {
        switch (opt)
        {
        case 'a':
            para_a = true;
            break;
        case 'l':
            para_l = true;
            break;
        case 'i':
            para_i = true;
            break;
        case 's':
            para_s = true;
            break;
        case 'R':
            para_R = true;
            break;
        case 'r':
            para_r = true;
            break;
        case 't':
            para_t = true;
            break;
        default:
            printf("The parameter you give are not allowed\n");
        }
    }

    int flag = 0;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            flag = 1;
            printf("%s:\n", argv[i]);
            do_ls(argv[i]);
        }
    }
    if (flag == 0)
        do_ls(".");

    return 0;
}

void print_filename(char *filename, mode_t filemode)
{
    if (S_ISDIR(filemode))
        printf("\033[01;32m%s\033[0m", filename);
    else if (S_ISCHR(filemode))
        printf("\033[40;33m%s\033[0m", filename);
    else if (S_ISBLK(filemode))
        printf("\033[40;33m%s\033[0m", filename);
    else if (S_ISLNK(filemode))
        printf("\033[30;42m%s\033[0m", filename);
    else if (S_ISREG(filemode))
    {
        if (filemode & S_IXUSR || filemode & S_IXGRP || filemode & S_IXOTH)
            printf("\033[01;32m%s\033[0m", filename);
        else
            printf("%s", filename);
    }
    else
        printf("%s", filename);
}

void do_ls(char *dirname)
{

    Fileinfo *fileinfo = malloc(sizeof(Fileinfo) * 10000);
    if (fileinfo == NULL)
    {
        perror("Fail to alloc");
        exit(EXIT_FAILURE);
    }

    int file_cnt = 0;
    DIR *dir_ptr;
    struct dirent *cur_dirent;
    if ((dir_ptr = opendir(dirname)) == NULL)
    {
        perror("Fail to open directory");
        exit(EXIT_FAILURE);
    }
    else
    {
        while ((cur_dirent = readdir(dir_ptr)) != NULL)
        {
            if (!para_a && cur_dirent->d_name[0] == '.')
                continue;
            char *pst = strdup(cur_dirent->d_name); 
            if (pst == NULL)
            {
                perror("Fail to alloc");
                exit(EXIT_FAILURE);
            }
            fileinfo[file_cnt++].filename = pst;
        }
    }

    for (int i = 0; i < file_cnt; i++)
    {
        char pathname[MAX_PATH];
        snprintf(pathname, sizeof(pathname), "%s/%s", dirname, fileinfo[i].filename); // 使用 snprintf 避免缓冲区溢出
        if (lstat(pathname, &fileinfo[i].info) == -1)
        {
            perror("Fail to get informaiton");
            exit(EXIT_FAILURE);
            continue;
        }
    }

    if (para_t)
        qsort(fileinfo, file_cnt, sizeof(Fileinfo), compare_t);
    else
        qsort(fileinfo, file_cnt, sizeof(Fileinfo), compare);
    if (para_r)
    {
        int left = 0, right = file_cnt - 1;
        while (left < right)
        {
            Fileinfo temp = fileinfo[left];
            fileinfo[left++] = fileinfo[right];
            fileinfo[right--] = temp;
        }
    }

    for (int i = 0; i < file_cnt; i++)
    {
        print_fileinfo(fileinfo[i]);
    }
    if (para_R)
    {
        for (int i = 0; i < file_cnt; i++)
        {
            if (S_ISDIR(fileinfo[i].info.st_mode))
            {
                if (strcmp(fileinfo[i].filename, ".") != 0 && strcmp(fileinfo[i].filename, "..") != 0)
                {
                    char pathname[MAX_PATH];
                    snprintf(pathname, sizeof(pathname), "%s/%s", dirname, fileinfo[i].filename);
                    printf("\n%s:\n", pathname);
                    do_ls(pathname);
                    }
            }
        }
    }

    for (int i = 0; i < file_cnt; ++i)
    {
        free(fileinfo[i].filename);
        fileinfo[i].filename = NULL;
    }
    free(fileinfo);

    closedir(dir_ptr);
}

int compare(const void *a, const void *b)
{
    Fileinfo *_a = (Fileinfo *)a;
    Fileinfo *_b = (Fileinfo *)b;
    return strcmp(_a->filename, _b->filename);
}

void print_fileinfo(const Fileinfo fileinfo)
{
    if (para_l)
    {
        printf((S_ISDIR(fileinfo.info.st_mode)) ? "d" : "-");
        printf((fileinfo.info.st_mode & S_IRUSR) ? "r" : "-");
        printf((fileinfo.info.st_mode & S_IWUSR) ? "w" : "-");
        printf((fileinfo.info.st_mode & S_IXUSR) ? "x" : "-");
        printf((fileinfo.info.st_mode & S_IRGRP) ? "r" : "-");
        printf((fileinfo.info.st_mode & S_IWGRP) ? "w" : "-");
        printf((fileinfo.info.st_mode & S_IXGRP) ? "x" : "-");
        printf((fileinfo.info.st_mode & S_IROTH) ? "r" : "-");
        printf((fileinfo.info.st_mode & S_IWOTH) ? "w" : "-");
        printf((fileinfo.info.st_mode & S_IXOTH) ? "x" : "-");

        printf("%-2d ", (int)fileinfo.info.st_nlink); // 打印链接数

        struct passwd* user = getpwuid(fileinfo.info.st_uid);
        printf("%s ", user->pw_name); // 打印用户名

        struct group* gp = getgrgid(fileinfo.info.st_gid);
        printf("%s ", gp->gr_name); // 打印组名

        printf("%-10ld ", fileinfo.info.st_size); // 打印文件大小

        printf("%.12s ", ctime(&fileinfo.info.st_mtime) + 4); // 打印时间
    }
    if (para_i)
        printf("%-8lu ", fileinfo.info.st_ino);
    if (para_s)
        printf("%-8ld ", (long)fileinfo.info.st_blocks/2);
    
    print_filename(fileinfo.filename, fileinfo.info.st_mode);
    printf("\n");
}
int compare_t(const void *a, const void *b)
{
    Fileinfo *_a = (Fileinfo *)a;
    Fileinfo *_b = (Fileinfo *)b;
    return _a->info.st_mtime - _b->info.st_mtime;
}