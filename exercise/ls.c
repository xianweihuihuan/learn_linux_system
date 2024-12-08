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
void ls(char *filename)
{
    DIR *dir = opendir(filename);
    if (dir == NULL)
    {
        perror("open this dir failed");
        exit(EXIT_FAILURE);
    }
    struct dirent *rnm;
    fm *name = (fm *)malloc(10 * sizeof(fm));
    if (name == NULL)
    {
        perror("malloc failed");
        exit(EXIT_SUCCESS);
    }
    int alloc = 10;
    int sz = 0;
    while ((rnm = readdir(dir)) != NULL)
    {
        if (sz == alloc)
        {
            int newn = 2 * alloc;
            fm *jj = (fm *)realloc(name, newn * sizeof(fm));
            if (jj == NULL)
            {
                perror("realloc failed");
                exit(EXIT_FAILURE);
            }
            name = jj;
            alloc = newn;
        }
        name[sz].file = *rnm;
        int k = lstat(name[sz].file.d_name, &name[sz].stat);
        if (k == -1)
        {
            perror("obtain failed");
            exit(EXIT_FAILURE);
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

                char path[MAX_PATH];
                snprintf(path, MAX_PATH, "%s/%s", filename, name[i].file.d_name);
                printf("%s:\n", path);
                int lss = chdir(path);
                if (lss == -1)
                {
                    perror("failed chdir");
                    exit(EXIT_FAILURE);
                }
                ls(path);
            }
        }
    }
    free(name);
    closedir(dir);
}
