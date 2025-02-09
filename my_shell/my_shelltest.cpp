#include "my_shell.hpp"

int main()
{
    extern std::string infile;
    extern std::string outfile;
    //Env_Init();
    while (true)
    {
        Print_commandline();
        char commandline[COMMAND_SIZE];
        if (!getcommand(commandline, COMMAND_SIZE))
        {
            continue;
        }
        if (strncmp(commandline, "quit",4)==0)
        {
            break;
        }
        checkredir(commandline);
        checkfifo(commandline);
        // fifotest();
        // checktest();
        if (checkselfexcute())
        {
            infile.clear();
            outfile.clear();
            continue;
        }
        infile.clear();
        outfile.clear();
    }
    //     for(int i = 0;environ[i];i++){
    //         std::cout<<environ[i]<<std::endl;
    //     }
}