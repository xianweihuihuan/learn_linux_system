#include "Pool_thread.hpp"
#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include "mutex.hpp"
#include "cond.hpp"
#include <memory>
#include "Log.hpp"
#include "mutex.hpp"
#include "cond.hpp"
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
#include <functional>
#include <queue>
#include <memory>
#define Tail "\033[m"
#define Cyan "\033[0m\033[1;36m"
#define Yellow "\033[0m\033[1;33m"
#define Green "\033[0m\033[1;32m"

namespace Xianwei
{

    using Pthread_t = std::shared_ptr<pthread>;
    struct SearchConfig
    {
        SearchConfig(const std::string root_path,
                     const std::string file_type,
                     int max_concurrency,
                     int max_depth,
                     bool skip_hidden,
                     std::vector<std::string> skip_path)
            : _root_path(root_path),
              _file_type(file_type),
              _max_concurrency(max_concurrency),
              _max_depth(max_depth),
              _skip_hidden(skip_hidden)
        {
            for (const auto &x : skip_path)
            {
                _skip_paths.push_back(x);
            }
        }
        SearchConfig()
        {
        }
        SearchConfig &operator=(const SearchConfig &st)
        {
            _root_path = st._root_path;
            _file_type = st._file_type;
            _max_concurrency = st._max_concurrency;
            _max_depth = st._max_depth;
            _skip_hidden = st._skip_hidden;
            _skip_paths = st._skip_paths;
            return *this;
        }

        std::string _root_path;               // 要搜索的根目录
        std::string _file_type;               // 要搜索的文件类型，如 ".txt"、".cpp" 等
        int _max_concurrency;                 // 最大并发数
        int _max_depth;                       // 最大搜索深度
        bool _skip_hidden;                    // 是否跳过隐藏文件或目录
        std::vector<std::string> _skip_paths; // 要跳过的目录或文件的路径
    };
    class thread_pool
    {
    private:
        bool Isexit(const SearchConfig &st, const std::string name)
        {
            for (const auto &x : st._skip_paths)
            {
                if (name == x)
                {
                    return false;
                }
            }
            return true;
        }
        void Search(const SearchConfig &st)
        {
            std::string path = st._root_path;
            DIR *dir = opendir(path.c_str());
            if (dir == nullptr)
            {
                LOG(ERROR) << "opendir error-" << path;
                return;
            }
            dirent *rnm;
            while ((rnm = readdir(dir)) != nullptr)
            {

                std::string name = rnm->d_name;
                std::string filename = path;

                if (filename[filename.size() - 1] != '/')
                {
                    filename += '/';
                }
                filename += name;
                if (name == "." || name == "..")
                {
                    continue;
                }
                if (st._skip_hidden && name[0] == '.')
                {
                    continue;
                }
                if (!Isexit(st, name))
                {
                    continue;
                }
                // std::cout<<filename<<std::endl;
                if (name.find(st._file_type) != -1)
                {
                    std::cout << Cyan << filename << Tail << std::endl;
                }
                if (rnm->d_type == 4)
                {
                    if (st._max_depth == 1)
                    {
                        break;
                    }
                    SearchConfig next(filename, st._file_type, st._max_concurrency, st._max_depth - 1, st._skip_hidden, st._skip_paths);
                    Enter(next);
                    // std::cout<<filename<<std::endl;
                }
            }
            closedir(dir);
        }

        bool Isempty()
        {
            return _tq.empty();
        }

        void Task()
        {
            while (true)
            {
                SearchConfig t;
                {
                    mutexmaster st(_lock);
                    while (Isempty() && _isrunning)
                    {
                        _waitnums++;
                        _cd.wait(_lock);
                        _waitnums--;
                    }
                    if (Isempty() && !_isrunning)
                    {
                        break;
                    }
                    t = _tq.front();
                    _tq.pop();
                }
                Search(t);
            }
        }

    public:
        thread_pool(int num)
            : _num(num),
              _isrunning(false),
              _waitnums(0)
        {
            for (int i = 0; i < _num; i++)
            {
                _thread.push_back(std::make_shared<pthread>(std::bind(&thread_pool::Task, this)));
            }
        }

        void Enter(const SearchConfig &task)
        {
            mutexmaster mtx(_lock);
            _tq.push(std::move(task));
            if (_waitnums > 0)
            {
                _cd.Notice();
            }
        }

        void Start()
        {
            if (_isrunning)
            {
                return;
            }
            _isrunning = true;
            for (const auto &x : _thread)
            {
                x->Start();
            }
        }

        void Stop()
        {
            mutexmaster mtx(_lock);
            if (_isrunning)
            {
                _isrunning = false;
            }
            if (!Isempty())
            {
                _cd.Noticeall();
            }
        }

        void Join()
        {
            for (const auto &x : _thread)
            {
                x->join();
            }
        }

    private:
        std::queue<SearchConfig> _tq;
        std::vector<Pthread_t> _thread;
        mutex _lock;
        cond _cd;
        int _num;
        bool _isrunning;
        int _waitnums;
    };

}
