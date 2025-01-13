#include <string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

#define MAX 1024
#define NONE_FLUSH (1 << 0)
#define LINE_FLUSH (1 << 1)
#define FULL_FLUSH (1 << 2)

typedef struct FILE_IO
{
    int fileno;          // 文件描述符
    int flag;            // 打开文件的方式
    char outbuffer[MAX]; // 语言级缓冲区
    int bufferlen;       // 目前缓冲区长度
    int flush_method;    // 刷新方式
} MY_FILE;

MY_FILE *my_fopen(const char *file, const char *mod);
void my_fclose(MY_FILE *fd);
int my_fwrite(MY_FILE *fd,void *str,int len);
void my_flush(MY_FILE *fd);
