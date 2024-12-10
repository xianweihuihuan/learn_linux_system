#include"progress.h"

void progress(const char*tips,double total,double already){
    char buff[SIZE];
    memset(buff,0,sizeof(buff));
    const char* lable = "|/-\\";
    int len = strlen(lable);
    static int index = 0;
    double rate = already*100.0/total;
    int sz = (int)(already*100/total);
    for(int i = 0;i<sz;i++){
        buff[i] = STYLE;
    }
    printf("%s[%-100s][%.1f%%][%c]\r",tips,buff,rate,lable[index%len]);
    fflush(stdout);
    index++;
    index%=len;
}
double speed[10] = {1.0,0.5,0.2,0.3,2.0,0.1,0.05,0.02,0.01,0.001};
void download(double total,p cb){
    srand((unsigned)time(NULL));
    double already = 0.0;
    while(already<=total){
        cb("下载中",total,already);
        if(already>=total){
            break;
        }
        usleep(5000);
        int random = rand()%10;
        already+= speed[random];
        if(already>=total){
            already = total;
        }
    }
    printf("\n");
}