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
#define MAX_PATH 1000
#define Tail "\033[m"
#define Cyan "\033[0m\033[1;36m"
#define Yellow "\033[0m\033[1;33m"
#define Green "\033[0m\033[1;32m"
typedef struct fm
{
    struct dirent file;
    struct stat stat;
} fm;
int sort1(const void *a, const void *b);
void ls(const char *filename);
