#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<sys/wait.h>
#include<signal.h>
#include<sys/resource.h>
#include<fcntl.h>

typedef struct joblist {
    long long int pid;
    char name[10000];
}joblist;

joblist jobinfo[100000];
joblist running;

long long jobs, processid;
char hdn[100000];
char un[100000], hn[100000];
char old[10000];
bool changed;