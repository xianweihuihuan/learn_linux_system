#include "cond.hpp"
#include "mutex.hpp"
#include "pthread.hpp"
#include <queue>

namespace Xianwei
{
    template <class T>
    class blockqueue
    {
    private:
        bool Isempty()
        {
            return _q.empty();
        }

        bool Isfull()
        {
            return _q.size == _cap;
        }

    public:
        blockqueue(int cap = 10)
            : _cap(cap)
        {
        }

        void Equeue(const T &in)
        {
            mutexmaster(_mtx);
            while (Isfull())
            {
                _p_cond.wait(_mtx);
            }
            _q.push(in);
            _c_cond.Notice();
        }

        void Pop(T *out)
        {
            mutexmaster(_mtx);
            while (Isempty())
            {
                _c_cond.wait(_mtx);
            }
            *out = _q.front();
            _q.pop();
            _p_cond.Notice();
        }

        ~blockqueue()
        {
        }

    private:
        std::queue<T> _q;
        int _cap;
        mutex _mtx;
        cond _p_cond;
        cond _c_cond;
    };
}