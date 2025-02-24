#include "my_shell.hpp"

char cwd[1024];
char *g_env[ENVSZ];
int g_envs = 0;
std::string infile;
std::string outfile;
bool inred = false;
bool outred = false;
bool appendred = false;
char *fifocommand[100];
int fifosz = 0;
char *g_argv[100];
int g_args;
bool back = false;



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
    std::cout << "[" << Green << user << Tail << Cyan "@~/" << tmp << Tail "]" << Yellow << "$:" << Tail;
}

bool getcommand(char *in, size_t sz)
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
    in[strlen(in) - 1] = '\0';
    while (in[i] == ' ' && i < strlen(in))
    {
        for (int j = 0; j < strlen(in); j++)
        {
            in[j] = in[j + 1];
        }
    }
    if (strlen(in) == 0)
    {
        return false;
    }
    return true;
}

void Skip_space(char *in, int &end)
{
    while (isspace(in[end]))
    {
        end++;
    }
}

void obtainrefile(std::string &file, char *in, int pos)
{
    while (in[pos] != '\0' && in[pos] != ' ')
    {
        char tmp = in[pos];
        in[pos] = ' ';
        file += tmp;
        pos++;
    }
}

void checkredir(char *in)
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
            in[end] = ' ';
            end++;
            Skip_space(in, end);
            obtainrefile(outfile, in, end);
        }
        else
        {
            end--;
        }
    }
}

void checkfifo(char *in)
{
    fifosz = 0;
    fifocommand[fifosz++] = strtok(in, "|");
    while ((bool)(fifocommand[fifosz++] = strtok(nullptr, "|")))
        ;
    fifosz--;
}



void check(char *in)
{
    for (int i = 0; i <= strlen(in); i++)
    {
        if (in[i] == '&')
        {
            in[i] = ' ';
            back = true;
        }
    }
    g_args = 0;
    g_argv[g_args++] = strtok(in, " ");
    while ((bool)(g_argv[g_args++] = strtok(NULL, " ")))
        ;
    g_args--;
}

bool my_cd()
{
    if (g_args > 2)
    {
        perror("too much parameter");
        return false;
    }
    char *dir = g_argv[1];
    char pwd[100];
    char *tmp = getcwd(pwd, sizeof(pwd));
    if (tmp == nullptr)
    {
        perror("get the current path fail");
        return false;
    }
    if (g_args == 1 || (strcmp(dir, "~") == 0))
    {
        char *home = getenv("HOME");
        if (home == nullptr)
        {
            perror("get homedir fail");
            return false;
        }
        int num = chdir(home);
        if (num == -1)
        {
            perror("chdir failed");
            return false;
        }
    }
    else if (strcmp(dir, "-") == 0)
    {

        char *oldpwd = getenv("OLDPWD");
        if (oldpwd == nullptr)
        {
            perror("get last pwd fail");
            return false;
        }
        int num = chdir(oldpwd);
        if (num == -1)
        {
            perror("chdir fail");
            return false;
        }
    }
    else
    {
        int num = chdir(dir);
        if (num == -1)
        {
            perror("chdir failed");
            return false;
        }
    }
    setenv("OLDPWD", pwd, 1);
    return true;
}

bool checkselfexcute()
{
    if (strstr(fifocommand[0], "cd"))
    {
        if (fifosz != 1)
        {
            perror("操作错误");
            return false;
        }
        check(fifocommand[0]);
        my_cd();
        return true;
    }
    return false;
}

void _excute(int sz)
{
    if (sz == fifosz - 1)
    {
        // printf("11111111111");
        check(fifocommand[sz]);
        if (back)
        {
            int iid = fork();
            if (iid == 0)
            {
                int tmp = execvp(g_argv[0], g_argv);
                if (tmp == -1)
                {
                    perror("invalid command");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                exit(EXIT_SUCCESS);
            }
        }
        else
        {
            int tmp = execvp(g_argv[0], g_argv);
            if (tmp == -1)
            {
                perror("invalid command");
                exit(EXIT_FAILURE);
            }
        }
        return;
    }
    else
    {
        int fd[2] = {0};
        int tmp = pipe(fd);
        if (tmp == -1)
        {
            perror("create pipe fail");
            return;
        }
        // printf("111111111111111\n");
        pid_t id = fork();
        if (id == 0)
        {
            close(fd[0]);
            dup2(fd[1], 1);
            check(fifocommand[sz]);
            if (back)
            {
                int iid = fork();
                if (iid == 0)
                {
                    int tmp = execvp(g_argv[0], g_argv);
                    if (tmp == -1)
                    {
                        perror("invalid command");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else
            {
                if (id == 0)
                {
                    close(fd[0]);
                    dup2(fd[1], 1);
                    // check(fifocommand[sz]);
                    int tmp = execvp(g_argv[0], g_argv);
                    if (tmp == -1)
                    {
                        perror("invalid command");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    close(fd[1]);
                    dup2(fd[0], 0);
                    waitpid(id, NULL, 0);
                    _excute(++sz);
                }
            }
        }
        else
        {
            close(fd[1]);
            dup2(fd[0], 0);
            waitpid(id, NULL, 0);
            _excute(++sz);
        }
    }
}

void dorefile()
{
    if (inred)
    {
        int num = open(infile.c_str(), O_RDONLY);
        dup2(num, 0);
        close(num);
    }
    if (outred)
    {
        int num = open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(num, 1);
        close(num);
    }
    else if (appendred)
    {
        int num = open(outfile.c_str(), O_APPEND | O_WRONLY | O_CREAT, 0644);
        dup2(num, 1);
        close(num);
    }
}

void excute()
{
    int pid = fork();
    if (pid == 0)
    {
        dorefile();
        _excute(0);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}
