#include "my_shell.hpp"

int main()
{
    extern std::string infile;
    extern std::string outfile;
    signal(SIGINT, SIG_IGN);
    std::cout << Cyan << "Welcome to user my shell" << Tail << std::endl;
    while (true)
    {
        Print_commandline();
        char commandline[COMMAND_SIZE];
        if (!getcommand(commandline, COMMAND_SIZE))
        {
            continue;
        }
        if (strcmp(commandline, "exit") == 0)
        {
            break;
        }
        checkredir(commandline);
        checkfifo(commandline);
        if (checkselfexcute())
        {
            infile.clear();
            outfile.clear();
            continue;
        }
        excute();
        infile.clear();
        outfile.clear();
    }
}