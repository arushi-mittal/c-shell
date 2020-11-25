#include "headers.h"
#include "nightswatch.h"

void interrupt (int t) {
    char head[100000], info[10000];
    FILE * f = fopen("/proc/interrupts", "r");
    if (f != NULL) {
        fgets(head, 10000, f);
        printf("%s", head);
        fclose(f);
    }
    else {
        perror("Error");
    }
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    while (1) {
        FILE * f = fopen("/proc/interrupts", "r");
        if (f) {
            for (int i = 0; i < 3; i++) {
                fgets(info, 100000, f);  
            }
            info[3] = ' '; info[4] = ' '; info[5] = ' '; info[95] = '\0';
            printf("\n%s", info);
            fclose(f);
            char buf[2];
            int r = read(0, buf, 2);
            if (r > 0) {
                if (buf[0] == 'q') {
                    printf("\n\n\n");
                    fcntl(0, F_SETFL, fcntl(0, F_GETFL) & ~O_NONBLOCK);
                    fflush(stdout); 
                    return;
                }
            }
            sleep(t);
        }
        else {
            perror("Error");
        }
   }
}

void nw (char * commands) {
    int seconds = 5, flag = 0;
    commands = strtok(NULL,  " ");
    while (commands != NULL) {
        if (strcmp(commands, "-n") == 0) {
            commands = strtok(NULL,  " ");
            if (commands) {
                seconds = atoi(commands);
            }
        }
        if (commands != NULL) {
            if (strcmp(commands, "interrupt") == 0) {
                flag = 1;
            }
            commands = strtok(NULL, " ");
        }
    }
    if (flag) {
        interrupt(seconds);
    }
}