#include"my_stdio.h"

void func1()
{
    MY_FILE* yyy = my_fopen("test1.txt","w");
    const char* ttt = "hello world\n";
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fclose(yyy);
}
void func3()
{
    MY_FILE* yyy = my_fopen("test1.txt","w");
    const char* ttt = "hello bit\n";
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fclose(yyy);
}
void func2()
{
    MY_FILE* yyy = my_fopen("test2.txt","a");
    const char* ttt = "hello world\n";
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fclose(yyy);
}
void func4()
{
    MY_FILE* yyy = my_fopen("test2.txt","a");
    const char* ttt = "hello bit\n";
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fwrite(yyy,ttt,strlen(ttt));
    my_fclose(yyy);
}
int main(){
    func1();
    func2();
    func3();
    func4();
    return 0;
    
}

