#include "headers.h"
#include "pinfo.h"


void pinfo (char * commands, char * homedir) {
    long long int processid, memory;
    char filename[10000], state[5], info[100000], executable[100000];
    if (commands == NULL) {
        processid = getpid();
    }
    else {
        processid = atoi(commands);
    }
    sprintf(filename, "/proc/%lld/stat", processid);
    FILE * f = fopen(filename, "r");
    if (f) {
        fgets(info, 1000000, f);
        char * token = strtok(info, " ");
        for (int x = 0; x < 2; x++) {
            token = strtok(NULL, " ");
        }
        while (strlen(token) != 1) {
            token = strtok(NULL, " ");
        }
        strcpy(state, token);
        if (processid) {
            printf("pid -- %lld\n", processid);
        }
        printf("Process Status -- %s\n", state);
        for (int x = 0; x < 20; x++) {
            token = strtok(NULL, " ");
        }
        memory = atoi(token);
        printf("Memory -- %lld\n", memory);
        fclose(f);
    }
    else {
        printf("Process does not exist\n");
        return;
    }
    size_t esize;
    sprintf(filename, "/proc/%lld/exe", processid);
    f = fopen(filename, "r");
    if (f) {
        fgets(info, 1000000, f);
        esize = readlink(filename, info, 1000000);
        if (esize != -1) {
            info[esize] = '\0'; 
            if (strstr(info, homedir)) {
                char temp[100000];
                strcpy(temp, "~");
                size_t n = strlen(homedir);
                long long int x;
                for (x = n; x < esize; x++) {
                    temp[x - n + 1] = info[x];
                }
                temp[x - n + 1] = '\0';
                printf("Executable Path -- %s\n", temp);
            }
            else {
                printf("Executable Path -- %s\n", info);
            }
        }
        fclose(f);
    }
    else {
        printf("Executable Path not available\n");
        return;
    }
}