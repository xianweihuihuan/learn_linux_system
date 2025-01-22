#include "ProcessPool.hpp"

int main()
{
    int num = 0;
    std::cin >> num;
    while (num == 0)
    {
        std::cout << "请重新输入：";
        std::cin >> num;
    }
    ProcessPool t(num);
    int cnt = 0;
    std::cin >> cnt;
    t.Start();
    while (cnt--)
    {
        t.Run();
        sleep(1);
        std::cout << cnt << std::endl;
        std::cout << std::endl;
    }
    t.End();
    return 0;
}