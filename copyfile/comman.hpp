#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include<cstring>

#define EXIT_ERR(m)         \
    do                      \
    {                       \
        std::cerr << m;     \
        exit(EXIT_FAILURE); \
    } while (0);

class namedfifo
{
public:
    namedfifo(const std::string &path = ".", const std::string &file = "fifo")
    {
        umask(0);
        _file = (path + "/" + file);
        int tmp = mkfifo(_file.c_str(), 0664);
        if (tmp < 0)
        {
            EXIT_ERR("make fifo")
        }
    }
    std::string fifoname()
    {
        return _file;
    }
    ~namedfifo()
    {
        unlink(_file.c_str());
    }

private:
    std::string _file;
};

class readfifo
{
public:
    readfifo(const std::string &path = ".", const std::string &file = "fifo")
    {
        _file = (path + "/" + file);
        int fd = open(_file.c_str(), O_RDONLY);
        if (fd < 0)
        {
            EXIT_ERR("open fifo(read)")
        }
        else
        {
            _fd = fd;
        }
    }
    bool readfromfifo(int fd)
    {
        char buffer[1024];
        int num = read(_fd, buffer, sizeof(buffer));\
        //std::cout<<buffer;
        //std::cout<<num<<"\n";
        if (num == 0)
        {
            return false;
        }
        else if (num < 0)
        {
            EXIT_ERR("read");
        }
        else
        {
            write(fd,buffer,strlen(buffer));
            return true;
        }
    }
    ~readfifo()
    {
        close(_fd);
    }

private:
    std::string _file;
    int _fd;
};

class writefifo
{
public:
    writefifo(const std::string &path = ".", const std::string &file = "fifo")
    {
        _file = (path + "/" + file);
        int fd = open(_file.c_str(), O_APPEND | O_WRONLY);
        if (fd < 0)
        {
            EXIT_ERR("open fifo(write)")
        }
        else
        {
            _fd = fd;
        }
    }
    void writetofifo(char *buffer,int n)
    {
        int num = write(_fd, buffer, n);
        if (num < 0)
        {
            EXIT_ERR("write")
        }
    }
    ~writefifo()
    {
        close(_fd);
    }

private:
    std::string _file;
    int _fd;
};