#include "ls.h"

int mode[200] = {0};
void stackinit(stack *st)
{
    st->file = NULL;
    st->alloc = 0;
    st->sz = 0;
}
void boarden(stack *st)
{
    int newn = (st->alloc == 0) ? 4 : st->alloc * 2;
    st->file = (char **)realloc(st->file, newn * sizeof(char *));
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
char *stacktop(stack *st)
{
    return st->file[st->sz - 1];
}
void stackdestory(stack *st)
{
    for (int i = 0; i < st->alloc; i++)
    {
        free(st->file[i]);
    }
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

void ls_n()
{
    char tmp[200];
    getcwd(tmp, 200);
    DIR *dir;
    dir = opendir(tmp);
    fm *name = (fm *)malloc(10 * sizeof(fm));
    struct dirent *rnm;
    int alloc = 10;
    int sz = 0;
    while ((rnm = readdir(dir)) != NULL)
    {
        if (sz == alloc)
        {
            int newn = 2 * alloc;
            name = (fm *)realloc(name, newn * sizeof(fm));
            alloc = newn;
        }
        name[sz].file = *rnm;
        stat(rnm->d_name, &name[sz].stat);
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
        printf("\n");
    }
    free(name);
    closedir(dir);
}
void ls_m(char *filename, char *filename1)
{
    printf("%s:\n", filename1);
    chdir(filename);
    DIR *dir;
    dir = opendir(filename);
    fm *name = (fm *)malloc(10 * sizeof(fm));
    struct dirent *rnm;
    int alloc = 10;
    int sz = 0;
    while ((rnm = readdir(dir)) != NULL)
    {
        if (sz == alloc)
        {
            int newn = 2 * alloc;
            name = (fm *)realloc(name, newn * sizeof(fm));
            alloc = newn;
        }
        name[sz].file = *rnm;
        stat(rnm->d_name, &name[sz].stat);
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
            if (mode['a'] == 0 && name[i].file.d_name[0] == '.')
            {
                continue;
            }
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
            printf("%3ld ", name[i].stat.st_nlink);
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
        printf("\n");
    }
    free(name);
    closedir(dir);
}

void ls_Rn()
{
    while(1){
        char ori[200];
        getcwd(ori,200);
        ls_m(ori,ori);
    }
}

void ls_Rm(char *filename, char *filename1)
{
    ls_m(filename, filename1);
    stack st;
    stackinit(&st);
    chdir(filename);
    DIR *dir;
    dir = opendir(filename);
    fm *name = (fm *)malloc(10 * sizeof(fm));
    struct dirent *rnm;
    int alloc = 10;
    int sz = 0;
    while ((rnm = readdir(dir)) != NULL)
    {
        if (sz == alloc)
        {
            int newn = 2 * alloc;
            name = (fm *)realloc(name, newn * sizeof(fm));
            alloc = newn;
        }
        name[sz].file = *rnm;
        stat(rnm->d_name, &name[sz].stat);
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
    for (int i = sz - 1; i >= 0; i--)
    {
        if (strcmp(name[i].file.d_name, ".") == 0 || strcmp(name[i].file.d_name, "..") == 0)
        {
            continue;
        }
        if (name[i].file.d_type == 10)
        {
            continue;
        }
        if (name[i].file.d_type == 4)
        {
            chdir(name[i].file.d_name);
            char tptm[200];
            getcwd(tptm, 200);
            stackpush(&st, tptm);
            chdir(filename);
        }
    }
    free(name);
    closedir(dir);
    while (!(stackempty(&st)))
    {
        chdir(stacktop(&st));
        char pp[200];
        getcwd(pp, 200);
        ls_m(pp, pp);
        DIR *DIr;
        DIr = opendir(pp);
        fm *name = (fm *)malloc(10 * sizeof(fm));
        alloc = 10;
        sz = 0;
        stackpop(&st);
        while ((rnm = readdir(DIr)) != NULL)
        {
            if (sz == alloc)
            {
                int newn = 2 * alloc;
                name = (fm *)realloc(name, newn * sizeof(fm));
                alloc = newn;
            }
            name[sz].file = *rnm;
            stat(rnm->d_name, &name[sz].stat);
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

        for (int i = sz - 1; i >= 0; i--)
        {

            if (strcmp(name[i].file.d_name, ".") == 0 || strcmp(name[i].file.d_name, "..") == 0)
            {
                continue;
            }
            if (name[i].file.d_type == 10)
            {
                continue;
            }
            if (name[i].file.d_type == 4)
            {
                chdir(name[i].file.d_name);
                char tptm[200];
                getcwd(tptm, 200);
                stackpush(&st, tptm);
                chdir(pp);
            }
        }
        closedir(DIr);
        free(name);
    }
    stackdestory(&st);
}