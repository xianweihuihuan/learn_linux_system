#include "Reactor.hpp"
#include "Listener.hpp"


int main(int argc,char* argv[]){
    std::unique_ptr<Xianwei::reactor> st = std::make_unique<Xianwei::reactor>();
    uint16_t port = std::stoi(argv[1]);
    auto cn = std::make_shared<Xianwei::Listener>(port);
    st->insertcn(cn);
    st->Loop();
}