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
    int i = 0;
    in[strlen(in)-1] = '\0';
    while(in[i] == ' '&&i < strlen(in)){
        for(int j = 0;j < strlen(in);j++){
            in[j] = in[j+1];
        }
        //printf("%s\n",in);    
    } 
    //printf("%d\n",strlen(in));
    if(strlen(in)==0){
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
    while((bool)(g_argv[g_args++] = strtok(NULL," ")));
    g_args--;
}

void checktest(){
    for(int i = 0;i<fifosz;i++){
        check(fifocommand[i]);
        for(int j = 0;j<g_envs;j++){
            std::cout<<g_env[j]<<std::endl;
        }
    }
}

bool my_cd(){
    if(g_args > 2){
        printf("输入的参数太多\n");
        return false;
    }
    char* dir = g_argv[1];
    char pwd[100];
    char* tmp = getcwd(pwd,sizeof(pwd));
    if(tmp==nullptr){
        printf("获取当前路径失败\n");
        return false;
    }
    if(g_args == 1 || (strcmp(dir,"~") == 0)){
        //printf("111111111111\n");
        char* home = getenv("HOME");
        //printf("111111111111\n");
        if(home == nullptr){
            printf("获取家目录失败\n");
            return false;
        }
        int num = chdir(home);
        if(num == -1){
            printf("进入家目录失败\n");
            return false;
        }
    }
    else if(strcmp(dir,"-")==0){
        
        char* oldpwd = getenv("OLDPWD");
        //std::cout<<oldpwd;
        if(oldpwd == nullptr){
            printf("找不到上次目录\n");
            return false;
        }
        int num = chdir(oldpwd);
        if(num == -1){
            printf("进入上次目录失败\n");
            return false;
        }
    }else{
        //printf("%s",dir);
        int num = chdir(dir);
        if(num == -1){
            printf("进入该目录失败\n");
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
        //printf("1111111111111111\n");
        //printf("%d\n",g_args);
        my_cd();
        //std::cout<<"my_cd"<<std::endl;
        return true;
    }
    return false;
}

void _excute(int sz){
    if(sz == fifosz - 1){
        //printf("11111111111");
        check(fifocommand[sz]);
        execvp(g_argv[0],g_argv);
        return;
    }else{
        int fd[2] = {0};
        int tmp = pipe(fd);
        if(tmp == -1){
           printf("管道创建失败\n");
           return;    
        }
        //printf("111111111111111\n");
        pid_t id = fork();
        if(id == 0){
            close(fd[0]);
            dup2(fd[1],1);
            check(fifocommand[sz]);
            execvp(g_argv[0],g_argv);
        }else{
            close(fd[1]);
            dup2(fd[0],0);
            waitpid(id,NULL,0);
            _excute(++sz);
        }
    }
}

void dorefile(){
    if(inred){
        int num = open(infile.c_str(),O_RDONLY);
        dup2(num,0);
        close(num);
        //printf("%s\n",infile.c_str());
    }
    if(outred){
        int num = open(outfile.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
        dup2(num,1);
        close(num);
        //printf("%s\n",outfile.c_str());
    }else if(appendred){
        int num = open(outfile.c_str(),O_APPEND|O_WRONLY|O_CREAT,0644);
        dup2(num,1);
        close(num);
        //printf("%s\n",outfile.c_str());
    }
}

void excute(){
    int pid = fork();
    //printf("111111111\n");
    if(pid == 0){
        //printf("111111111\n");
        dorefile();
        _excute(0);
    }else{
        //printf("1111111111111\n");
        waitpid(pid,NULL,0);
    }
}

