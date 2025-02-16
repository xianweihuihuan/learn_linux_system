#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<fcntl.h>
#include<vector>

#define COMMAND_SIZE 1024
#define ARGCSZ 100
#define ENVSZ 100
#define NONE_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3


void Env_Init();
void Print_commandline();
bool getcommand(char* in,size_t sz);
void checkredir(char* in);
void checkfifo(char* in);
void fifotest();
void check(char* in);
void checktest();
bool checkselfexcute();
void excute();