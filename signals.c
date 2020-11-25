#include "headers.h"
#include "signals.h"
#include "prompt.h"

void ctrlz () {
    if (running.pid == processid) {
        signal(SIGTSTP, ctrlz);
        printf("\n");
        char host[10000], user[10000];
        gethostname(host, 10000);
        getlogin_r(user, 10000);
        prompt(host, user);
        fflush(stdout);
        return;
    }
    else {
        kill(running.pid, SIGTTIN);
        kill(running.pid, SIGTSTP);
        jobinfo[jobs].pid = running.pid;
        strcpy(jobinfo[jobs].name, running.name);
        jobs++;
        running.pid = processid;
        strcpy(running.name, "shell\0");
        signal(SIGTSTP, ctrlz);
        return;
    }
}

void ctrlc () {
    if (getpid() == processid) {
        signal(SIGINT, ctrlc);
        printf("\n");
        char host[10000], user[10000];
        gethostname(host, 10000);
        getlogin_r(user, 10000);
        prompt(host, user);
        fflush(stdout);
        return;
    }
    else if (running.pid != processid) {
        kill(running.pid, SIGINT);
        running.pid = processid;
        strcpy(running.name, "shell\0");
        signal(SIGINT, ctrlc);
        return;
    }
}