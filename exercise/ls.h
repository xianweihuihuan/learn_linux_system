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

typedef struct fm
{
    struct dirent file;
    struct stat stat;
}fm;
typedef struct stack
{
    char **file;
    int alloc;
    int sz;
} stack;

void stackinit(stack*st);
void boarden(stack *st);
void stackpush(stack *st, const char *file);
void stackpop(stack *st);
bool stackempty(stack *st);
char* stacktop(stack*st);
void ls_n();
void ls_m(char* filename,char*filename1);
void ls_Rn();
void ls_Rm(char* filename,char*filename1);