#include "my_shell.hpp"

int main()
{
    extern std::string infile;
    extern std::string outfile;
    Env_Init();
    while (true)
    {
        Print_commandline();
        char commandline[COMMAND_SIZE];
        if(!getcommand(commandline,COMMAND_SIZE)){
            continue;
        }
        checkredir(commandline);
        checkfifo(commandline);
        //fifotest();
        //checktest();
        infile.clear();
        outfile.clear();
        
    }
//     for(int i = 0;environ[i];i++){
//         std::cout<<environ[i]<<std::endl;
//     }
}