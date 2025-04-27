#include "timewheel.hpp"
#include <iostream>
#include <thread>

void Test(){
  std::cout << "1111111111111111111111111111\n";
}
int main(){
  TimerWheel pt;
  std::function<void()> ll = &Test;
  int i = 1;
  pt.AddTask(10, 20, ll);
  while(true){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pt.Run();
    std::cout << "等待了一秒" <<i++<< std::endl;
  }
}