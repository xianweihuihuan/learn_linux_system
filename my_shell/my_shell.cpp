#include "my_shell.hpp"

char cwd[1024];
char *g_env[ENVSZ];
int g_envs = 0;
std::string infile;
std::string outfile;
bool inred = false;
bool outred = false;
bool appendred = false;
char* fifocommand[100];
int fifosz = 0;
char* g_argv[100];
int g_args;


void Env_Init()
{
    extern char **environ;
    memset(g_env, 0, sizeof(g_env));
    for (int i = 0; environ[i]; i++)
    {
        g_env[i] = (char *)malloc(strlen(environ[i]) + 1);
        strcpy(g_env[i], environ[i]);
        g_envs++;
    }
    g_env[g_envs++] = "HH";
    g_env[g_envs] = nullptr;
    for (int i = 0; g_env[i]; i++)
    {
        putenv(g_env[i]);
    }
    environ = g_env;
}

void Print_commandline()
{
    char *user = getenv("USER");
    char *pwd = getcwd(cwd, sizeof(cwd));
    if (pwd == NULL)
    {
        perror("get cwd failed\n");
        exit(EXIT_FAILURE);
    }
    std::string dir = cwd;
    std::string tmp;
    if (dir != "/")
    {
        auto pos = dir.rfind("/");
        if (pos == std::string::npos)
        {
            perror("BUG");
            exit(EXIT_FAILURE);
        }
        tmp = dir.substr(pos + 1);
    }
    else
    {
        tmp = dir;
    }
    std::cout << "[" << user << "@~/" << tmp << "]$:";
}

bool getcommand(char* in, size_t sz)
{
    char *c = fgets(in, sz, stdin);
    if (c == nullptr)
    {
        return false;
    }
    in[strlen(in) + 1] = '\0';
    if (strlen(in) == 0)
    {
        return false;
    }
    return true;
}

void Skip_space(char*in, int &end)
{
    while (isspace(in[end]))
    {
        end++;
    }
}

void obtainrefile(std::string &file, char* in, int pos)
{
    while (in[pos] != '\0' && in[pos] != ' ')
    {
        char tmp = in[pos];
        in[pos] = ' ';
        file += tmp;
        pos++;
    }
}

void checkredir(char* in)
{
    int start = 0;
    int end = strlen(in) - 1;
    while (start < end)
    {
        if (in[end] == '<')
        {
            in[end++] = ' ';
            Skip_space(in, end);
            inred = true;
            obtainrefile(infile, in, end);
            //std::cout << infile << std::endl;
        }
        else if (in[end] == '>')
        {
            if (in[end - 1] == '>')
            {
                appendred = true;
                in[end - 1] = ' ';
            }
            else
            {
                outred = true;
            }
            in[end++] = 0;
            Skip_space(in, end);
            obtainrefile(outfile, in, end);
            //std::cout << outfile << std::endl;
        }
        else
        {
            end--;
        }
    }
}

void checkfifo(char* in){
    fifosz = 0;
    fifocommand[fifosz++] = strtok(in,"|");
    while((bool)(fifocommand[fifosz++] = strtok(nullptr,"|")));
    fifosz--;
}

void fifotest(){
    std::cout<<fifosz<<std::endl;
    for(int i = 0;i<fifosz;i++){
        std::cout<<fifocommand[i]<<std::endl;
    }
}

void check(char* in){
    g_args = 0;
    g_argv[g_args++] = strtok(in," ");
    while((bool)(g_argv[g_args++] = strtok(nullptr," ")));
    g_args--;
}

// void checktest(){
//     for(int i = 0;i<fifosz;i++){
//         check(fifocommand[i]);
//         for(int j = 0;j<g_envs;j++){
//             std::cout<<g_env[j]<<std::endl;
//         }
//     }
// }
bool my_cd(){
    if(g_args == 1){
        perror("输入的参数太少");
        return false;
    }
    if(g_args > 2){
        perror("输入的参数太多");
        return false;
    }
    char pwd[100];
    char* tmp = getcwd(pwd,sizeof(pwd));
    if(tmp==nullptr){
        perror("获取当前路径失败");
        return false;
    }
    if(strncmp(g_argv[1],"-",1)==0){
        //std::cout<<"-"<<std::endl;
        char* oldpwd = getenv("OLDPWD");
        std::cout<<oldpwd;
        if(oldpwd == nullptr){
            perror("找不到上次目录");
            return false;
        }
        int num = chdir(oldpwd);
        if(num == -1){
            perror("进入上次目录失败");
            return false;
        }
    }else{
        int num = chdir(g_argv[1]);
        if(num == -1){
            perror("进入该目录失败");
            return false;
        }
    }
    setenv("OLDPWD",pwd,1);
    return true;
}

bool checkselfexcute(){
    if(strstr(fifocommand[0],"cd")){
        if(fifosz != 1){
            perror("操作错误");
            return false;
        }
        check(fifocommand[0]);
        my_cd();
        //std::cout<<"my_cd"<<std::endl;
    }
    return true;
}


