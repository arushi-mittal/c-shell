#include "headers.h"
#include "kjobs.h"

void removejobs (long long int x) {
	if (x >= 0) {
		for (long long i = x; i < jobs - 1; i++) {
			jobinfo[i].pid = jobinfo[i + 1].pid;
			strcpy(jobinfo[i].name, jobinfo[i + 1].name);
		}
		jobs--;
	}
	else {
		perror("Error");
	}
}

void removekjob (int pid) {
    for (int i = 0; i < jobs; i++) {
        if (jobinfo[i].pid == pid) {
            jobinfo[i].pid = 0;
            jobinfo[i].name[0] = '\0';
            removejobs(i);
            break;
        }
    }
}

void kjobs (char * command) {
    pid_t pid;
    int signal;
    command = strtok(NULL, " ");
    if (command) {
        pid = atoi(command);
    }
    else {
        printf("No process ID given\n");
        return;
    }
    command = strtok(NULL, " ");
    if (command) {
        signal = atoi(command);
    }
    else {
        printf("No kill signal given\n");
        return;
    }
    kill(pid, signal);
    removekjob(pid);
}