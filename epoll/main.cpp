#include "epollsever.hpp"

int main(int argc,char* argv[]){
    int port =std::stoi(argv[1]);
    Xianwei::epollserver st(port);
    st.init();
    st.loop();
}