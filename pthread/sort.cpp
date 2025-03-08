#include <iostream>
#include <vector>
#include <ctime>
#include <threads.h>
#include <memory>
#include "pthread.hpp"

std::vector<int> nums(100000, 0);
struct in
{
    std::vector<int> ret;
    int _val;
};

void selectnums(in &tmp)
{
    for (auto x : nums)
    {
        if (x == tmp._val)
        {
            tmp.ret.emplace_back(tmp._val);
        }
    }
}

int main()
{
    srand(time(nullptr));
    for (auto &x : nums)
    {
        x = rand() % 10 + 1;
    }
    std::shared_ptr<in[]> st(new in[10]);
    for (int i = 0; i < 10; i++)
    {
        st[i]._val = i + 1;
    }
    for (int i = 0; i < 10; i++)
    {
        Xianwei::pthread<in &> pth(st[i], selectnums);
        pth.Start();
        pth.join();
    }
    int len = 0;
    for (int i = 0; i < 10; i++)
    {
        for (const auto &x : st[i].ret)
        {
            nums[len++] = x;
        }
    }
    for (const auto &x : nums)
    {
        std::cout << x << " ";
    }
}
