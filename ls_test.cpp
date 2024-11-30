#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

#define LEFT "["
#define RIGHT "]"
#define LABLE "#"
#define DELIM " \t"
#define LINE_SIZE 1024
#define ARGC_SIZE 32
#define EXIT_CODE 44

#define NONE -1
#define IN_RDIR     0
#define OUT_RDIR    1
#define APPEND_RDIR 2

int lastcode = 0;
int quit = 0;
extern char **environ;
char commandline[LINE_SIZE];
char *argv[ARGC_SIZE];
char pwd[LINE_SIZE];
char *rdirfilename = NULL;
int rdir = NONE;

// 自定义环境变量表
char myenv[LINE_SIZE];
// 自定义本地变量表


const char *getusername()
{
    return getenv("USER");
}

const char *gethostname1()
{
    return getenv("HOSTNAME");
}

void getpwd()
{
    getcwd(pwd, sizeof(pwd));
}

void check_redir(char *cmd)
{

    // ls -al -n
    // ls -al -n >/</>> filename.txt
    char *pos = cmd;
    while(*pos)
    {
        if(*pos == '>')
        {
            if(*(pos+1) == '>')
            {
                *pos++ = '\0';
                *pos++ = '\0';
                while(isspace(*pos)) pos++;
                rdirfilename = pos;
                rdir=APPEND_RDIR;
                break;
            }
            else{
                *pos = '\0';
                pos++;
                while(isspace(*pos)) pos++;
                rdirfilename = pos;
                rdir=OUT_RDIR;
                break;
            }
        }
        else if(*pos == '<')
        {
            *pos = '\0'; // ls -a -l -n < filename.txt
            pos++;
            while(isspace(*pos)) pos++;
            rdirfilename = pos;
            rdir=IN_RDIR;
            break;
        }
        else{
            //do nothing
        }
        pos++;
    }
}


void interact(char *cline, int size)
{
    getpwd();
    printf(LEFT"%s@%s %s"RIGHT""LABLE" ", getusername(), gethostname1(), pwd);
    char *s = fgets(cline, size, stdin);
    assert(s);
    (void)s;
    // "abcd\n\0"
    cline[strlen(cline)-1] = '\0';

    //ls -a -l > myfile.txt
    check_redir(cline);
}

int splitstring(char cline[], char *_argv[])
{
    int i = 0;
    argv[i++] = strtok(cline, DELIM);
    while(_argv[i++] = strtok(NULL, DELIM)); // 故意写的=
    return i - 1;
}

void NormalExcute(char *_argv[])
{
    pid_t id = fork();
    if(id < 0){
        perror("fork");
        return;
    }
    else if(id == 0){
        int fd = 0;

        // 后面我们做了重定向的工作，后面我们在进行程序替换的时候，难道不影响吗？？？
        if(rdir == IN_RDIR)
        {
            fd = open(rdirfilename, O_RDONLY);
            dup2(fd, 0);
        }
        else if(rdir == OUT_RDIR)
        {
            fd = open(rdirfilename, O_CREAT|O_WRONLY|O_TRUNC, 0666);
            dup2(fd, 1);
        }
        else if(rdir == APPEND_RDIR)
        {
            fd = open(rdirfilename, O_CREAT|O_WRONLY|O_APPEND, 0666);
            dup2(fd, 1);
        }
        //让子进程执行命令
        //execvpe(_argv[0], _argv, environ);
        execvp(_argv[0], _argv);
        exit(EXIT_CODE);
    }
    else{
        int status = 0;
        pid_t rid = waitpid(id, &status, 0);
        if(rid == id) 
        {
            lastcode = WEXITSTATUS(status);
        }
    }
}

int buildCommand(char *_argv[], int _argc)
{
    if(_argc == 2 && strcmp(_argv[0], "cd") == 0){
        chdir(argv[1]);
        getpwd();
        sprintf(getenv("PWD"), "%s", pwd);
        return 1;
    }
    else if(_argc == 2 && strcmp(_argv[0], "export") == 0){
        strcpy(myenv, _argv[1]);
        putenv(myenv);
        return 1;
    }
    else if(_argc == 2 && strcmp(_argv[0], "echo") == 0){
        if(strcmp(_argv[1], "$?") == 0)
        {
            printf("%d\n", lastcode);
            lastcode=0;
        }
        else if(*_argv[1] == '$'){
            char *val = getenv(_argv[1]+1);
            if(val) printf("%s\n", val);
        }
        else{
            printf("%s\n", _argv[1]);
        }

        return 1;
    }

    // 特殊处理一下ls
    if(strcmp(_argv[0], "ls") == 0)
    {
        _argv[_argc++] = "--color";
        _argv[_argc] = NULL;
    }
    return 0;
}

int main()
{
    while(!quit){
        // 1.
        rdirfilename = NULL;
        rdir = NONE;
        // 2. 交互问题,获取命令行, ls -a -l > myfile / ls -a -l >> myfile / cat < file.txt
        interact(commandline, sizeof(commandline));

        // commandline -> "ls -a -l -n\0" -> "ls" "-a" "-l" "-n"
        // 3. 子串分割的问题，解析命令行
        int argc = splitstring(commandline, argv);
        if(argc == 0) continue;

        // 4. 指令的判断 
        // debug
        //for(int i = 0; argv[i]; i++) printf("[%d]: %s\n", i, argv[i]);
        //内键命令，本质就是一个shell内部的一个函数
        int n = buildCommand(argv, argc);

        // 5. 普通命令的执行
        if(!n) NormalExcute(argv);
    }
    return 0;
}
