#include "pthread.hpp"

void fun1(int tmp)
{
    while (1)
    {
        sleep(1);
        std::cout << "I get a number :" << tmp << std::endl;
    }
}

int main()
{
    Xianwei::pthread<int> st(5, fun1);
    st.Start();
    while(1){
        sleep(1);
        std::cout<<"I am thread-main, I'm running"<<std::endl;
    }
    st.join();
}