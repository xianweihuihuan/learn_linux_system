#include<stdio.h>
#include<stdlib.h>


int main(){
    printf("PATH = %s\n",getenv("PATH"));
    printf("USER = %s\n",getenv("USER"));
    printf("HOME = %s\n",getenv("HOME"));
    printf("LOGNAME = %s\n",getenv("LOGNAME"));
    printf("HOSTNAME = %s\n",getenv("HOSTNAME"));
    printf("PWD = %s\n",getenv("PWD"));
    printf("OLDPWD = %s\n",getenv("OLDPWD"));
    printf("SHELL = %s\n",getenv("SHELL"));

}