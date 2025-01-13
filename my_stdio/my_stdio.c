#include "my_stdio.h"

MY_FILE *BuyFile(int fd, int flag)
{
    MY_FILE *f = (MY_FILE *)malloc(sizeof(MY_FILE));
    if (f == NULL)
    {
        const char *error = "文件结构体创建失败";
        write(2, error, strlen(error));
        exit(EXIT_FAILURE);
    }
    f->bufferlen = 0;
    f->fileno = fd;
    f->flag = flag;
    f->flush_method = LINE_FLUSH;
    memset(f->outbuffer, 0, MAX);
    return f;
}
MY_FILE *my_fopen(const char *file, const char *mod)
{
    int fd = -1;
    int flag = 0;
    if (strcmp(mod, "w") == 0)
    {
        flag = O_CREAT | O_WRONLY | O_TRUNC;
    }
    else if (strcmp(mod, "r") == 0)
    {
        flag = O_RDONLY;
    }
    else if (strcmp(mod, "a") == 0)
    {
        flag = O_CREAT | O_WRONLY | O_APPEND;
    }
    else if (strcmp(mod, "w+") == 0)
    {
        flag = O_CREAT | O_RDWR | O_TRUNC;
    }
    else if (strcmp(mod, "r+") == 0)
    {
        flag = O_RDWR | O_TRUNC;
    }
    else if (strcmp(mod, "w+") == 0)
    {
        flag = O_CREAT | O_RDWR | O_TRUNC;
    }
    else if (strcmp(mod, "a+") == 0)
    {
        flag = O_CREAT | O_RDWR | O_APPEND;
    }
    else
    {
        const char *error = "没有这个选项";
        write(2, error, strlen(error));
        exit(EXIT_FAILURE);
    }
    fd = open(file, flag, 0666);
    if (fd == -1)
    {
        const char *error = "打开文件失败";
        exit(EXIT_FAILURE);
    }
    MY_FILE *f = BuyFile(fd, flag);
    return f;
}
void my_fclose(MY_FILE *fd)
{
    my_flush(fd);
    close(fd->fileno);
}
int my_fwrite(MY_FILE *fd, void *str, int len)
{
    memcpy(fd->outbuffer, str, len);
    fd->bufferlen += len;
    if (fd->outbuffer[fd->bufferlen - 1] == '\n' && (fd->flush_method & LINE_FLUSH))
    {
        my_flush(fd);
    }
    return 0;
}
void my_flush(MY_FILE *fd)
{
    int n = write(fd->fileno, fd->outbuffer, fd->bufferlen);
    fd->bufferlen = 0;
    memset(fd->outbuffer, 0, MAX);
}

