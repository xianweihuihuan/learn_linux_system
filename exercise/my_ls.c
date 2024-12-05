#include "my_ls.h"

int mode[1000] = {0};
void stackinit(stack *st)
{
    st->file = NULL;
    st->alloc = 0;
    st->sz = 0;
}
void boarden(stack *st)
{
    int newn = (st->alloc == 0) ? 4 : st->alloc * 2;
    st->file = (char **)realloc(st->file, sizeof(char *));
    st->alloc = newn;
    for (int i = st->sz; i < st->alloc; i++)
    {
        st->file[i] = (char *)malloc(100);
    }
}
void stackpush(stack *st, const char *file)
{
    if (st->sz == st->alloc)
    {
        boarden(st);
    }
    strcpy(st->file[st->sz], file);
    st->sz += 1;
}
void stackpop(stack *st)
{
    st->sz -= 1;
}
bool stackempty(stack *st)
{
    return (st->sz == 0);
}
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
void ls_none(int argc, char *argv[])
{
    int index = 0;
    char ndir[1024];
    char *tmp = getcwd(ndir, 1024);
    struct dirent file[10000];
    if (!tmp)
    {
        fprintf(stderr, "Usage: %s<directorry>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    DIR *dir = opendir(tmp);
    if (!dir)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    struct dirent *tmpf;
    while ((tmpf = readdir(dir)) != NULL)
    {
        if (tmpf->d_name[0] == '.')
        {
            continue;
        }
        file[index++] = *tmpf;
    }
    qsort(file, index, sizeof(struct dirent), &sort1);
    for (int i = 0; i < index; i++)
    {
        struct stat tmps;
        stat(file[i].d_name, &tmps);
        if (S_ISDIR(tmps.st_mode))
        {
            printf("\033[0m\033[1;36m%s\033[m/  ", file[i].d_name);
        }
        if (S_ISREG(tmps.st_mode))
        {
            printf("\033[0m\033[1;33m%s  \033[m", file[i].d_name);
        }
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
void ls_normal(struct fm *name, long count)
{
    if (mode['a'] == 0)
    {
        int tmpp = 0;
        if (mode['s'] != 0)
        {
            int size = 0;
            for (int i = 0; i < count; i++)
            {
                if (name[i].file.d_name[0] == '.')
                {
                    continue;
                }
                size += name[i].stat.st_blocks;
            }
            printf("总计 %d\n", size / 2);
        }
        for (int i = 0; i < count; i++)
        {
            if (name[i].file.d_name[0] == '.')
            {
                continue;
            }
            if (mode['i'] != 0)
            {
                printf("%7ld ", name[i].stat.st_ino);
            }
            if (mode['s'] != 0)
            {
                printf("%-3ld ", name[i].stat.st_blocks / 2);
            }
            if (name[i].file.d_type == 4)
            {
                printf("\033[0m\033[1;36m%-20s\033[m", name[i].file.d_name);
                tmpp++;
            }
            if ((name[i].file.d_type == 8))
            {
                printf("\033[0m\033[1;33m%-20s\033[m", name[i].file.d_name);
                tmpp++;
            }
            if (name[i].file.d_type == 10)
            {
                printf("\033[0m\033[1;32m%-20s\033[m", name[i].file.d_name);
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
        int tmpp = 0;
        if (mode['s'] != 0)
        {
            int size = 0;
            for (int i = 0; i < count; i++)
            {
                size += name[i].stat.st_blocks;
            }
            printf("总计 %d\n", size / 2);
        }
        for (int i = 0; i < count; i++)
        {
            if (mode['i'] != 0)
            {
                printf("%7ld ", name[i].stat.st_ino);
            }
            if (mode['s'] != 0)
            {
                printf("%-3ld", name[i].stat.st_blocks / 2);
            }

            if (name[i].file.d_type == 4)
            {
                printf("\033[0m\033[1;36m%-20s\033[m", name[i].file.d_name);
                tmpp++;
            }
            if ((name[i].file.d_type == 8))
            {
                printf("\033[0m\033[1;33m%-20s\033[m", name[i].file.d_name);
                tmpp++;
            }
            if (name[i].file.d_type == 10)
            {
                printf("\033[0m\033[1;32m%-20s\033[m", name[i].file.d_name);
                tmpp++;
            }
            if (tmpp == 5)
            {
                printf("\n");
                tmpp = 0;
            }
        }
    }
}
void ls_l(struct fm *name, long count)
{
    static char *perm[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    if (mode['s'] != 0)
    {
        int tmp = 0;
        for (int i = 0; i < count; i++)
        {
            if (mode['a'] == 0 && name[i].file.d_name[0] == '.')
            {
                continue;
            }
            tmp += name[i].stat.st_blocks;
        }
        printf("总计 %d\n", tmp / 2);
    }
    for (int i = 0; i < count; i++)
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
        if (name[i].file.d_type == 4)
        {
            printf("d");
        }
        else
        {
            printf("-");
        }
        for (int j = 3; j; --j)
        {
            printf("%3s", perm[(name[i].stat.st_mode & mask) >> (j - 1) * N_BITS]);
            mask >>= N_BITS;
        }
        printf(" ");
        printf("%ld ", name[i].stat.st_nlink);
        struct passwd *aaa = getpwuid(name[i].stat.st_uid);
        printf("%s ", aaa->pw_name);
        struct group *bbb = getgrgid(name[i].stat.st_gid);
        printf("%s ", bbb->gr_name);
        printf("%10ld ", name[i].stat.st_size);
        struct tm *ccc = localtime(&(name[i].stat.st_mtime));
        printf("%2d月 %2d %-2d:%-2d ", ccc->tm_mon + 1, ccc->tm_mday, ccc->tm_hour, ccc->tm_min);
        if (name[i].file.d_type == 4)
        {
            printf("\033[0m\033[1;36m%-20s\033[m\n", name[i].file.d_name);
        }
        if ((name[i].file.d_type == 8))
        {
            printf("\033[0m\033[1;33m%-20s\033[m\n", name[i].file.d_name);
        }
        if (name[i].file.d_type == 10)
        {
            printf("\033[0m\033[1;32m%-20s\033[m\n", name[i].file.d_name);
        }
    }
}
void ls_R(struct fm *name, long count)
{
    ;
}
void ls(char **filename, int i)
{
    DIR *dir;
    chdir(filename[i]);
    char tmpd[100];
    char *tmptmp = getcwd(tmpd, 100);
    dir = opendir(tmpd);
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
    free(name);
    name = NULL;
    closedir(dir);
}