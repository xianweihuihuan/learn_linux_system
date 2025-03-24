#pragma once
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <memory>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <time.h>
#include "mutex.hpp"

namespace Xianwei
{
    enum Level
    {
        DEBUG = 1,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    std::string obtiantime()
    {
        time_t t = time(nullptr);
        tm tt;
        localtime_r(&t, &tt);
        char buf[1024];
        snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d %02d %02d",
                 tt.tm_year + 1900,
                 tt.tm_mon + 1,
                 tt.tm_mday,
                 tt.tm_hour,
                 tt.tm_min,
                 tt.tm_sec);
        return buf;
    }

    std::string leveltostring(Level lel)
    {
        switch (lel)
        {
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        default:
            return "None";
        }
    }

    class Logflush
    {
    public:
        virtual ~Logflush() = default;
        virtual void SyncLog(const std::string &message) = 0;
    };

    class cmdflush : public Logflush
    {
    public:
        cmdflush()
        {
        }

        ~cmdflush()
        {
        }
        void SyncLog(const std::string &message)
        {
            mutexmaster mtx(_lock);
            std::cout << message << std::endl;
        }

    private:
        mutex _lock;
    };

    class fileflush : public Logflush
    {
    public:
        fileflush(const std::string &path = "./", const std::string &file = "log.txt")
            : _path(path), _file(file)
        {
            mutexmaster mtx(_lock);
            if (std::filesystem::exists(_path))
            {
                return;
            }
            try
            {
                std::filesystem::create_directories(_path);
            }
            catch (std::filesystem::filesystem_error &e)
            {
                std::cerr << e.what() << "\n";
            }
        }

        ~fileflush()
        {
        }

        void SyncLog(const std::string &message)
        {
            mutexmaster mtx(_lock);
            std::string log = _path + _file;
            std::ofstream out(log, std::ios::app);
            if (!out.is_open())
            {
                return;
            }
            out << message << "\n";
            out.close();
        }

    private:
        std::string _path;
        std::string _file;
        mutex _lock;
    };

    class logger
    {
    public:
        logger()
        {
            _flush = std::make_shared<cmdflush>();
        }

        void enablecmd()
        {
            _flush = std::make_shared<cmdflush>();
        }

        void enablefile()
        {
            _flush = std::make_shared<fileflush>();
        }

        class logmessage
        {
        public:
            logmessage(Level lel, const std::string file, int line, logger &logger)
                : _filename(file), _line(line), _logger(logger), _pid(getpid()), _lel(lel), _time(obtiantime())
            {
                std::stringstream ssbuffer;
                ssbuffer << "[" << _time << "]"
                         << "[" << _lel << "]"
                         << "[" << _pid << "]"
                         << "[" << _filename << "]"
                         << "[" << _line << "]-";
                _mess = ssbuffer.str();
            }

            template <class T>
            logmessage &operator<<(const T &tmp)
            {
                std::stringstream ssbuffer;
                ssbuffer << tmp;
                _mess += ssbuffer.str();
                return *this;
            }

            ~logmessage()
            {
                if (_logger._flush)
                {
                    _logger._flush->SyncLog(_mess);
                }
            }

        private:
            std::string _time;
            Level _lel;
            pid_t _pid;
            std::string _filename;
            int _line;
            logger &_logger;
            std::string _mess;
        };

        logmessage operator()(Level lel, const std::string filename, int line)
        {
            return logmessage(lel, filename, line, *this);
        }

    private:
        std::shared_ptr<Logflush> _flush;
    };
    logger Logger;
#define LOG(Level) Logger(Level, __FILE__, __LINE__)
#define ENABLE_CMD() Logger.enablecmd();
#define ENABLE_FILE() Logger.enablefile();
}