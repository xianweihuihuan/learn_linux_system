#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <limits.h>

#define N_BITS 3

struct fm
{
    struct dirent file;
    struct stat stat;
};
typedef struct stack
{
    char *file[100];
    int alloc;
    int sz;
} stack;
void ls_none(int argc, char *argv[]);
int sort1(const void *a, const void *b);
int sort_time(const void *a, const void *b);
void ls_normal(struct fm *name, long count);
void ls_l(struct fm *name, long count);
void ls_R(struct fm *name, long count);
void ls(char **filename, int i);