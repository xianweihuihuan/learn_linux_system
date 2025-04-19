#include "thread_pool.hpp"
#include <iostream>
int j = 1;

int ttt(int i){
  std::cout<<"线程"<<std::this_thread::get_id()<<"开始执行第"<<j++<<"个任务"<<std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout<<"线程"<<std::this_thread::get_id()<<"执行完毕"<<std::endl;
  return i*i;
}

int main(){
  Xianwei::thread_pool pool(5);
  std::vector<std::future<int>> ret;
  for(int i = 1;i<=10;++i) {
    auto tmp = pool.Enter(ttt,i);
    ret.emplace_back(std::move(tmp));
  }
  std::this_thread::sleep_for(std::chrono::seconds(20));
  for(auto& x : ret) {
    std::cout<<x.get()<<" ";
  }
}