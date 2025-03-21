#include "sem.hpp"
#include <vector>
#include "mutex.hpp"
#include <iostream>
#include <pthread.h>

namespace Xianwei
{
    template <class T>
    class Ringbuffer
    {
    public:
        Ringbuffer(int cap)
            : _cap(cap), _ring(cap), _p_step(0), _c_step(0), _spacesem(cap), _datasem(0)
        {
        }

        void Equeue(const T &in)
        {
            _spacesem.P();
            {
                mutexmaster(_p_mtx);
                _ring[_p_step++] = in;
                _p_step %= _cap;
            }
            _datasem.V();
        }

        void Pop(T *out)
        {
            _datasem.P();
            {
                mutexmaster(_c_mtx);
                *out = ring[_c_step++];
                _c_step %= _cap;
            }
            _spacesem.V();
        }

        ~Ringbuffer()
        {
        }

    private:
        std::vector<T> _ring;
        int _cap;
        int _p_step;
        int _c_step;
        sem _datasem;
        sem _spacesem;
        mutex _p_mtx;
        mutex _c_mtx;
    };
}