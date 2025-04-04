#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <vector>

#define COMMAND_SIZE 1024
#define ARGCSZ 100
#define ENVSZ 100
#define NONE_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3
#define Tail "\033[m"
#define Cyan "\033[0m\033[1;36m"
#define Yellow "\033[0m\033[1;33m"
#define Green "\033[0m\033[1;32m"

void Print_commandline();
bool getcommand(char *in, size_t sz);
void checkredir(char *in);
void checkfifo(char *in);
void check(char *in);
bool checkselfexcute();
void excute();