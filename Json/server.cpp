#include "socket.hpp"
#include <memory>
#include <jsoncpp/json/json.h>
#include <sstream>

std::string test1(const std::string &out)
{
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(out, root))
    {
        int i = std::stoi(root["one"].asString());
        int j = std::stoi(root["two"].asString());
        std::string ans = std::to_string(i + j);
        return ans;
    }
    return nullptr;
}

std::string test2(const std::string &ans)
{
    Json::Value root;
    root["answer"] = ans;
    Json::FastWriter writer;
    std::string s = writer.write(root);
    return s;
}

int main(int argc, char *argv[])
{
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<Xianwei::Socket> st = std::make_unique<Xianwei::TCP_Server>();
    st->build(port);
    Xianwei::InetAddr peer;
    int newfd = st->Accept(&peer);
    Xianwei::TCP_Server stt(newfd);
    while (true)
    {
        std::string out;
        int n = stt.Recv(&out);
        if (n > 0)
        {
            std::cout << out;
        }
        if (!out.empty())
        {
            std::string ans = test1(out);
            std::cout << ans << std::endl;
            std::string se = test2(ans);
            std::cout << se;
            stt.Send(se);
        }
        out.clear();
    }
}