#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

using Taskfunc = std::function<void()>;
using Releasefunc = std::function<void()>;
class Task {
 public:
  Task(uint64_t tid, uint32_t timeout, Taskfunc tcb)
      : _tid(tid), _timeout(timeout), _task_cb(tcb),_cancel(false) {}

  ~Task() {
    if (_cancel == false) {
      _task_cb();
    }
    _destory_cb();
  }

  void SetRelease(Releasefunc dcb) { _destory_cb = dcb; }

  uint32_t Timeout() { return _timeout; }

  void Cancel() { _cancel = true; }

 private:
  uint64_t _tid;
  uint32_t _timeout;
  bool _cancel;
  Taskfunc _task_cb;
  Releasefunc _destory_cb;
};

class TimerWheel {
 private:
  void Remove(uint64_t tid) {
    auto it = _timers.find(tid);
    if (it != _timers.end()) {
      _timers.erase(it);
    }
  }

 public:
  TimerWheel(int sz = 60) : _capacity(sz), _tick(0), _wheel(_capacity) {}

  void AddTask(uint64_t tid, uint32_t timeout,const Taskfunc& tcb) {
    Taskptr pt(new Task(tid, timeout, tcb));
    pt->SetRelease(std::bind(&TimerWheel::Remove, this, tid));
    int pos = (_tick + timeout) % _capacity;
    _wheel[pos].push_back(pt);
    _timers[tid] = Weakptr(pt);
  }

  void FlushTask(uint64_t tid) {
    auto it = _timers.find(tid);
    if (it != _timers.end()) {
      Taskptr pt = it->second.lock();
      uint32_t timeout = pt->Timeout();
      int pos = (_tick + timeout) % _capacity;
      _wheel[pos].emplace_back(pt);
    }
  }

  void CancelTime(uint64_t tid) {
    auto it = _timers.find(tid);
    if (it != _timers.end()) {
      Taskptr pt = it->second.lock();
      pt->Cancel();
    }
  }

  void Run() {
    _tick = (_tick + 1) % _capacity;
    _wheel[_tick].clear();
  }

 private:
  using Taskptr = std::shared_ptr<Task>;
  using Weakptr = std::weak_ptr<Task>;
  int _tick;
  int _capacity;
  std::vector<std::vector<Taskptr>> _wheel;
  std::unordered_map<uint64_t, Weakptr> _timers;
};