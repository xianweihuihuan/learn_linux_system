#pragma once

#include <iostream>
#include <string>
#include <functional>




using task_t = std::function<void(const std::string& name)>;

void Push(const std::string& name)
{
    std::cout<<"线程["<<name<<"]正在执行一个下载任务"<<std::endl;
    //LOG(LogLevel::DEBUG) << "我是一个推送数据到服务器的一个任务, 我正在被执行" << "[" << name << "]";
}

