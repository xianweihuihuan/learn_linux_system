#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define STYLE '='
#define SIZE 101
typedef void (*p)(const char*,double,double);
void progress(const char*tips,double total,double already);
void download(double total,p cb);