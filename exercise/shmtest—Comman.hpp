#include <sys/shm.h>
#include <sys/ipc.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

enum user_type
{
    CREATER,
    USER,
};

class shm
{
protected:
    void key(const std::string &pathname, int projid)
    {
        key_t tmp = ftok(pathname.c_str(), projid);
        if (tmp < 0)
        {
            ERR_EXIT("Creat key");
        }
        _key = tmp;
    }

    void Create()
    {
        _shmid = shmget(_key, _size, IPC_CREAT | IPC_EXCL | 0664);
        if (_shmid < 0)
        {
            ERR_EXIT("Create shm");
        }
    }

    void Get()
    {
        _shmid = shmget(_key, _size, IPC_CREAT);
    }

    void Attach()
    {
        _start_mem = shmat(_shmid, nullptr, 0);
        if (_start_mem == nullptr)
        {
            ERR_EXIT("shmat");
        }
    }

    void Detach()
    {
        shmdt(_start_mem);
    }

    void deletshm()
    {
        int n = shmctl(_shmid, IPC_RMID, nullptr);
        if (n < 0)
        {
            ERR_EXIT("Rm shm");
        }
    }

public:
    shm(user_type tt, int size, const std::string &pathname = ".", int projid = 0x66)
        : _usertype(tt), _size(size)
    {
        key(pathname, projid);
        if (tt == CREATER)
        {
            Create();
        }
        else if (tt == USER)
        {
            Get();
        }
        Attach();
    }

    void *getshm()
    {
        return _start_mem;
    }

    ~shm()
    {
        Detach();
        if (_usertype == CREATER)
        {
            deletshm();
        }
    }

protected:
    user_type _usertype;
    int _shmid;
    key_t _key;
    void *_start_mem;
    int _size;
};

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
            ERR_EXIT("make fifo");
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
            ERR_EXIT("open fifo(read)");
        }
        else
        {
            _fd = fd;
        }
    }

    bool readfromfifo(int fd)
    {
        char buffer[1024];
        int num = read(_fd, buffer, sizeof(buffer));
        if (num == 0)
        {
            return false;
        }
        else if (num < 0)
        {
            ERR_EXIT("read");
        }
        else
        {
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
            ERR_EXIT("open fifo(write)");
        }
        else
        {
            _fd = fd;
        }
    }

    void writetofifo(char *buffer, int n)
    {
        int num = write(_fd, buffer, n);
        if (num < 0)
        {
            ERR_EXIT("write");
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