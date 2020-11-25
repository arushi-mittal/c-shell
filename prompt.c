#include "headers.h"
#include "prompt.h"

void prompt (char * host, char * user) {
    long long int i;
    size_t size = 1000000, currentdirsize, inputsize, homedirsize = strlen(hdn);
    char currentdir[size], dirname[size];
    getcwd(currentdir, size);
    currentdirsize = strlen(currentdir);
    if (strstr(currentdir, hdn)) {
        strcpy(dirname, "~");
        for (i = homedirsize; i < currentdirsize; i++) {
            dirname[i - homedirsize + 1] = currentdir[i];
            
        }
        dirname[i - homedirsize + 1] = '\0';
    }
    else {
        strcpy(dirname, currentdir);
    }
    printf("\r<\033[1;32m%s@%s\033[0m:\033[1;34m%s\033[0m>", user, host, dirname);
}