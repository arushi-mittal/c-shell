#include "headers.h"
#include "execute.h"

void fg(char ** args) {
    pid_t pid, wpid;
    int status;
    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) < 0) {
            printf("Error: No such command\n");
        }
        exit(0);
    } 
    else if (pid < 0) {
        printf("Error: No such command");
        return;
    } 
    else {
        strcpy(running.name, args[0]);
        running.pid = pid;
        int status;
        waitpid(pid, &status, WUNTRACED);
        fflush(stdout);
    }
}


void bg (char ** command) {
    int fd, status, f;
    f = fork();
    if (f == 0) {
        setpgid(0, 0);
        fd = execvp(command[0], command);
        if (fd == -1) {
            perror("Error");
            return;
        }
        fflush(stdout);
        exit(0);
    }
    else if (f > 0) {
        jobinfo[jobs].pid = f;
        strcpy(jobinfo[jobs].name, command[0]);
        jobs++; 
    }
    else {
        perror("Error");
    }
}

void execute (char * commands) {
    char temp[10000];
    strcpy(temp, commands);
    int i = 0;
    char * commandslist[50];
    while (commands) {
        commandslist[i] = commands;
        i++;
        commands = strtok(NULL, " ");
    }
    commandslist[i] = NULL;
    if (strcmp(commandslist[i - 1], "&") == 0) {
        bg(commandslist);
    }
    else {
        fg(commandslist);
    }
}