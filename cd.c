#include "headers.h"
#include "cd.h"

void cd (char * commands, char * homedir) {
    char temp1[10000];
    if (commands == NULL) {
        getcwd(temp1, 10000);
        strcpy(old, temp1);
        if(chdir(homedir)) {
            perror("Error");
        }
    }
    else if (strcmp("~", commands) == 0 || strcmp(".", commands) == 0) {
        getcwd(temp1, 10000);
        strcpy(old, temp1);
        if (chdir(homedir)) {
            perror("Error");
        }
    }
    else if (commands[0] == '~') {
        changed = 1;
        getcwd(temp1, 10000);
        strcpy(old, temp1);
        long long int x;
        char temp[100000];
        strcpy(temp, homedir);
        size_t n = strlen(commands), h = strlen(homedir);
        for (x = 1; x < n; x++) {
            temp[h + x - 1] = commands[x];
        }
        temp[h + x - 1] = '\0';
        if (chdir(temp)) {
            perror("Error");
        }
    }
    else if (strcmp(commands, "-") == 0) {
        long long int i;
        char dirname[10000];
        size_t currentdirsize = strlen(old), homedirsize = strlen(hdn);
        if (changed == 0) {
            printf("%s\n", hdn);
        }
        else if (strstr(old, hdn)) {
            strcpy(dirname, "~");
            for (i = homedirsize; i < currentdirsize; i++) {
                dirname[i - homedirsize + 1] = old[i];
                
            }
            dirname[i - homedirsize + 1] = '\0';
            printf("%s\n", dirname);
        }
        else {
            strcpy(dirname, old);
            printf("%s\n", dirname);
        }
        if (chdir(old)) {
            perror("Error");
        }
    }
    else {
        changed = 1;
        getcwd(temp1, 10000);
        strcpy(old, temp1);
        if (chdir(commands)) {
            perror("Error");
        }
    }
}