#include "headers.h"
#include "pwd.h"

void pwd () {
    char dir[100000];
    getcwd(dir, 100000);
    printf("%s\n", dir);
}