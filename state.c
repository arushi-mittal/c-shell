#include "headers.h"
#include "state.h"
#include "signals.h"

void removefrombg (int pid) {
    jobs--;
    for (int i = pid; i < jobs; i++) {
        jobinfo[i].pid = jobinfo[i + 1].pid;
        strcpy(jobinfo[i].name, jobinfo[i + 1].name);
    }
}

void back (char * command) {
    command = strtok(NULL, " ");
    pid_t pid;
    if (command == NULL) {
        printf("No job number given\n");
    }
    else {
        pid = atoi(command);
        if (pid > jobs || pid < 1) {
            printf("Invalid job number\n");
        }
        else {
            kill(jobinfo[pid - 1].pid, SIGTTIN);
            kill(jobinfo[pid - 1].pid, SIGCONT);
            running.pid = processid;
            strcpy(running.name, "shell");
        }
    }
    return;
}

void fore (char * command) {
    command = strtok(NULL, " ");
    pid_t pid;
    if (command == NULL) {
        printf("No job number given\n");
    }
    else {
        pid = atoi(command);
        if (pid > jobs || pid < 1) {
            printf("Invalid job number\n");
        }
        else {
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU,SIG_IGN);
            tcsetpgrp(STDIN_FILENO, jobinfo[pid - 1].pid);
            strcpy(running.name, jobinfo[pid - 1].name);
            running.pid = jobinfo[pid - 1].pid;
            kill(jobinfo[pid - 1].pid, SIGCONT);
            removefrombg(pid - 1);
            waitpid(-1, NULL, WUNTRACED);
            signal(SIGTSTP, ctrlz);
            tcsetpgrp(STDIN_FILENO,getpgrp());
            signal(SIGTTIN,SIG_DFL);
            signal(SIGTTOU,SIG_DFL);
        }
    }
}