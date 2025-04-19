#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>
#include <memory>
#include <future>
#include <functional>

namespace Xianwei
{
  using func_t = std::function<void()>;
  class thread_pool
  {
  public:
    thread_pool(int num)
        : _num(num),
          _stop(false)
    {
      for (int i = 0; i < _num; ++i)
      {
        _thread.push_back(std::thread([this]()
                                      {
          while(true) {
            func_t T;
            {
              std::unique_lock<std::mutex> lk(_lock);
              _cond.wait(lk,[this](){return this->_stop||!(this->_task.empty());});
              if(_task.empty()&& _stop){return;}
              T = std::move(_task.front());
              _task.pop();
            }
            T();
          } }));
      }
    }

    template <class Fn, typename... Args>
    auto Enter(Fn &&f, Args &&...args) -> std::future<typename std::result_of<Fn(Args...)>::type>
    {
      using ret_type = typename std::result_of<Fn(Args...)>::type;
      auto task = std::make_shared<std::packaged_task<ret_type()>>(std::bind(std::forward<Fn>(f), std::forward<Args>(args)...));
      std::future<ret_type> ret = task->get_future();
      {
        std::lock_guard<std::mutex> mtx(_lock);
        _task.emplace([task](){ (*task)(); });
      }
      _cond.notify_one();
      return ret;
    }

    void Stop() {
      _stop = true;
    }

    void Join() {
      for(auto& x : _thread) {
        x.join();
      }
    }

    ~thread_pool() {
      Stop();
      _cond.notify_all();
      Join();
    }

  private:
    std::vector<std::thread> _thread;
    std::queue<func_t> _task;
    std::mutex _lock;
    std::condition_variable _cond;
    bool _stop;
    int _num;
  };
}