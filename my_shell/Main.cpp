#include "my_shell.hpp"

int main()
{
    extern std::string infile;
    extern std::string outfile;
    //Env_Init();
    signal(SIGINT, SIG_IGN);
    std::cout<<Cyan<<"Welcome to user my shell"<<Tail<<std::endl;
    while (true)
    {
        Print_commandline();
        char commandline[COMMAND_SIZE];
        if (!getcommand(commandline, COMMAND_SIZE))
        {
            continue;
        }
        //printf("%s\n",commandline);
        if (strcmp(commandline, "exit")==0)
        {
            break;
        }
        checkredir(commandline);
        checkfifo(commandline);
        // fifotest();
        // checktest();
        if (checkselfexcute())
        {
            //printf("1111111111\n");
            infile.clear();
            outfile.clear();
            continue;
        }
        //printf("1111111111\n");
        excute();
        infile.clear();
        outfile.clear();
    }
    
    //     for(int i = 0;environ[i];i++){
    //         std::cout<<environ[i]<<std::endl;
    //     }
}