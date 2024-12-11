#include "ls.h"

int mode[200] = {0};
int sort1(const void *a, const void *b)
{
    int flag = 1;
    if (mode['r'] != 0)
    {
        flag = -1;
    }
    char ch1;
    char ch2;
    struct dirent *tmp1 = (struct dirent *)a;
    struct dirent *tmp2 = (struct dirent *)b;
    if (tmp1->d_name[0] >= 'a' && tmp1->d_name[0] <= 'z')
    {
        ch1 = tmp1->d_name[0] - 32;
    }
    else
    {
        ch1 = tmp1->d_name[0];
    }
    if (tmp2->d_name[0] >= 'a' && tmp2->d_name[0] <= 'z')
    {
        ch2 = tmp2->d_name[0] - 32;
    }
    else
    {
        ch2 = tmp2->d_name[0];
    }
    if (ch2 != ch1)
    {
        return (ch1 - ch2) * flag;
    }
    else
    {
        int aa = strlen(tmp1->d_name);
        int bb = strlen(tmp2->d_name);
        return (aa - bb) * flag;
    }
}
int sort_time(const void *a, const void *b)
{
    int flag = 1;
    if (mode['r'] != 0)
    {
        flag = -1;
    }
    struct fm *tmp1 = (struct fm *)a;
    struct fm *tmp2 = (struct fm *)b;
    return ((tmp2->stat.st_mtime) - (tmp1->stat.st_mtime)) * flag;
}
void ls(const char *dirname)
{
    int k = chdir(dirname);
    if (k == -1)
    {
        printf("failed chdir %s", dirname);
        return;
    }
    DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        perror("open this dir failed");
        return;
    }
    struct dirent *rnm;
    fm *name = (fm *)malloc(10 * sizeof(fm));
    if (name == NULL)
    {
        closedir(dir);
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    int alloc = 10;
    int sz = 0;
    while ((rnm = readdir(dir)) != NULL)
    {
        if (sz == alloc)
        {
            int newn = 2 * alloc;
            fm *ff = (fm *)realloc(name, sizeof(fm) * newn);
            if (ff == NULL)
            {
                perror("realloc failed");
                free(name);
                closedir(dir);
                exit(EXIT_FAILURE);
            }
            name = ff;
            alloc = newn;
        }
        if (access(rnm->d_name, R_OK) == -1)
        {
            int lo = strlen(dirname);
            char path[1000];
            if (dirname[lo - 1] != '/')
            {
                sprintf(path, "%s/%s", dirname, rnm->d_name);
            }
            else
            {
                sprintf(path, "%s%s", dirname, rnm->d_name);
            }

            printf("没有该文件权限：%s\n", path);
            continue;
        }
        strcpy(name[sz].file.d_name, rnm->d_name);
        name[sz].file.d_ino = rnm->d_ino;
        name[sz].file.d_off = rnm->d_off;
        name[sz].file.d_reclen = rnm->d_reclen;
        name[sz].file.d_type = rnm->d_type;
        char filen[1000];
        snprintf(filen, 1000, "%s/%s", dirname, rnm->d_name);
        int k = lstat(filen, &name[sz].stat);
        if (k == -1)
        {
            free(name);
            closedir(dir);
            perror("obtain stat failed");
            return;
        }
        sz++;
    }
    if (mode['t'] != 0)
    {
        qsort(name, sz, sizeof(struct fm), &sort_time);
    }
    else
    {
        qsort(name, sz, sizeof(struct fm), &sort1);
    }
    if (mode['l'] == 0)
    {
        int tmpp = 0;
        for (int i = 0; i < sz; i++)
        {
            if (mode['a'] == 0)
            {
                if (name[i].file.d_name[0] == '.')
                {
                    continue;
                }
            }
            if (mode['i'] != 0)
            {
                printf("%7ld ", name[i].stat.st_ino);
            }
            if (mode['s'] != 0)
            {
                printf("%-3ld", name[i].stat.st_blocks / 2);
            }
            if (S_ISDIR(name[i].stat.st_mode))
            {
                printf(Cyan "%-20s" Tail, name[i].file.d_name);
                tmpp++;
            }
            if (S_ISREG(name[i].stat.st_mode))
            {
                printf(Yellow "%-20s" Tail, name[i].file.d_name);
                tmpp++;
            }
            if (S_ISLNK(name[i].stat.st_mode))
            {
                printf(Green "%-20s" Tail, name[i].file.d_name);
                tmpp++;
            }
            if (tmpp == 5)
            {
                printf("\n");
                tmpp = 0;
            }
        }
        printf("\n");
    }
    else
    {
        static char *perm[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
        if (mode['s'] != 0)
        {
            int tmps = 0;
            for (int i = 0; i < sz; i++)
            {
                if (mode['a'] == 0 && name[i].file.d_name[0] == '.')
                {
                    continue;
                }
                tmps += name[i].stat.st_blocks;
            }
            printf("总计 %d\n", tmps / 2);
        }
        for (int i = 0; i < sz; i++)
        {
            if (mode['a'] == 0 && name[i].file.d_name[0] == '.')
            {
                continue;
            }
            int mask = 0700;
            if (mode['i'] != 0)
            {
                printf("%7ld ", name[i].stat.st_ino);
            }
            if (mode['s'] != 0)
            {
                printf("%-3ld", name[i].stat.st_blocks / 2);
            }
            if (S_ISDIR(name[i].stat.st_mode))
            {
                printf("d");
            }
            else if (S_ISLNK(name[i].stat.st_mode))
            {
                printf("l");
            }
            else
            {
                printf("-");
            }
            // printf("%c", (name[i].stat.st_mode & S_IRUSR) ? 'r' : '-');
            // printf("%c", (name[i].stat.st_mode & S_IWUSR) ? 'w' : '-');
            // printf("%c", (name[i].stat.st_mode & S_IXUSR) ? 'x' : '-');
            // printf("%c", (name[i].stat.st_mode & S_IRGRP) ? 'r' : '-');
            // printf("%c", (name[i].stat.st_mode & S_IWGRP) ? 'w' : '-');
            // printf("%c", (name[i].stat.st_mode & S_IXGRP) ? 'x' : '-');
            // printf("%c", (name[i].stat.st_mode & S_IROTH) ? 'r' : '-');
            // printf("%c", (name[i].stat.st_mode & S_IWOTH) ? 'w' : '-');
            // printf("%c", (name[i].stat.st_mode & S_IXOTH) ? 'x' : '-');
            for (int j = 3; j; --j)
            {
                printf("%3s", perm[(name[i].stat.st_mode & mask) >> (j - 1) * N_BITS]);
                mask >>= N_BITS;
            }
            printf(" ");
            printf("%ld ", name[i].stat.st_nlink);
            struct passwd *aaa = getpwuid(name[i].stat.st_uid);
            printf("%-10s ", aaa->pw_name);
            struct group *bbb = getgrgid(name[i].stat.st_gid);
            printf("%-10s ", bbb->gr_name);
            printf("%10ld ", name[i].stat.st_size);
            struct tm *ccc = localtime(&(name[i].stat.st_mtime));
            printf("%d年 %2d月 %2d日 %02d:%02d ", ccc->tm_yday + 1679, ccc->tm_mon + 1, ccc->tm_mday, ccc->tm_hour, ccc->tm_min);
            if (S_ISDIR(name[i].stat.st_mode))
            {
                printf(Cyan "%-20s" Tail "\n", name[i].file.d_name);
            }
            if (S_ISREG(name[i].stat.st_mode))
            {
                printf(Yellow "%-20s" Tail "\n", name[i].file.d_name);
            }
            if (S_ISLNK(name[i].stat.st_mode))
            {
                printf(Green "%-20s" Tail "\n", name[i].file.d_name);
            }
        }
        printf("\n");
    }
    if (mode['R'] != 0)
    {
        for (int i = 0; i < sz; i++)
        {
            if (strcmp(name[i].file.d_name, ".") == 0 || strcmp(name[i].file.d_name, "..") == 0)
            {
                continue;
            }
            if (mode['a'] == 0 && name[i].file.d_name[0] == '.')
            {
                continue;
            }
            if (S_ISDIR(name[i].stat.st_mode))
            {
                int lo = strlen(dirname);
                char path[MAX_PATH];
                if (dirname[lo - 1] != '/')
                {
                    sprintf(path, "%s/%s", dirname, name[i].file.d_name);
                }
                else
                {
                    sprintf(path, "%s%s", dirname, name[i].file.d_name);
                }
                printf("%s:\n", path);
                ls(path);
            }
        }
    }
    free(name);
    closedir(dir);
}
