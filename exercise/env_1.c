#include<stdio.h>

int main(int argc,char *argv[],char *env[]){
    (void)argc;
    (void)argv;
    for(int i = 0;env[i];i++){
        printf("env[%d]->%s\n",i,env[i]);
    }
}