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
#include <stdbool.h>

#define N_BITS 3

struct fm
{
    struct dirent file;
    struct stat stat;
};
typedef struct stack
{
    char **file;
    int alloc;
    int sz;
} stack;
void stackinit(stack*st);
void ls_none(int argc, char *argv[]);
int sort1(const void *a, const void *b);
int sort_time(const void *a, const void *b);
void ls_normal(struct fm *name, long count);
void ls_l(struct fm *name, long count);
void ls_R(struct fm *name, long count);
void ls(char **filename, int i);
void boarden(stack *st);
void stackpush(stack *st, const char *file);
void stackpop(stack *st);
bool stackempty(stack *st);