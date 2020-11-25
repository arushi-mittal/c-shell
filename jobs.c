#include "headers.h"
#include "jobs.h"

void job () {
    char filename[100000], info[100000], status[10000];
    for (int i = 0; i < jobs; i++) {
        sprintf(filename, "/proc/%lld/stat", jobinfo[i].pid);
        FILE * f = fopen(filename, "r");
        if (f) {
            fgets(info, 100000, f);
            char * token = strtok(info, " ");
            for (int x = 0; x < 2; x++) {
                token = strtok(NULL, " ");
            }
            while (strlen(token) != 1) {
                token = strtok(NULL, " ");
            }
            if (strcmp(token, "T") == 0) {
                strcpy(status, "Stopped\0");
            }
            else if (strcmp(token, "S") == 0) {
                strcpy(status, "Running\0");
            }
            else if (strcmp(token, "R") == 0) {
                strcpy(status, "Running\0");
            }
            else if (strcmp(token, "D") == 0) {
                strcpy(status, "Stopped\0");
            }
            else if (strcmp(token, "W") == 0) {
                strcpy(status, "Paging\0");
            }
            else if (strcmp(token, "X") == 0) {
                strcpy(status, "Stopped\0");
            }
            else if (strcmp(token, "Z") == 0) {
                strcpy(status, "Zombie Process\0");
            }
        }
        printf("[%d] %s %s [%lld]\n", i + 1, status, jobinfo[i].name, jobinfo[i].pid);
    }
    if (jobs == 0) {
        printf("No jobs running\n");
    }
}