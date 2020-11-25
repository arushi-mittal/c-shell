#include "headers.h"
#include "shell.h"
#include "echo.h"
#include "cd.h"
#include "pwd.h"
#include "ls.h"
#include "pinfo.h"
#include "execute.h"
#include "proc.h"
#include "nightswatch.h"
#include "env.h"
#include "prompt.h"
#include "state.h"
#include "kjobs.h"
#include "overkill.h"
#include "jobs.h"
#include "signals.h"
 
int redir (char * commands) {
   if (strstr(commands, "<") && strstr(commands, ">>")) {
       return 1;
   }
   else if (strstr(commands, ">>")) {
       return 2;
   }
   else if (strstr(commands, "<") && strstr(commands, ">")) {
       return 3;
   }
   else if (strstr(commands, "<")) {
       return 4;
   }
   else if (strstr(commands, ">")) {
       return 5;
   }
   else {
       return 0;
   }
}
 
void red (char * commands) {
   int count = 0, f, s, fpin, fpout;
   int re = redir(commands);
   char * ip[10000], * op[10000];
   char * info[10000];
   char * opf, * ipf;
   op[0] = commands;
   if (re == 0) {
       return;
   }
   if (re != 4 && re != 0) {
       op[0] = strtok(commands, ">");
       op[1] = strtok(NULL, ">");
       if (op[1] == NULL) {
           printf("Invalid output file\n");
           return;
       }
       opf = strtok(op[1], " ");
   }
   ip[0] = op[0];
   if (re == 1 || re == 3 || re == 4) {
       ip[0] = strtok(ip[0], "<");
       ip[1] = strtok(NULL, "<");
       if (ip[1] == NULL) {
           printf("No input file given\n");
           return;
       }
       ipf = strtok(ip[1], " ");
       if (access(ipf, F_OK) == -1) {
           printf("Invalid input file\n");
           return;
       }
   }
   ip[0] = strtok(ip[0], " ");
   while (ip[0]) {
       info[count] = ip[0];
       count++;
       ip[0] = strtok(NULL, " ");
   }
   info[count] = NULL;
   if (re != 4 && re != 0) {
       if (opf == NULL) {
           printf("No output file given\n");
           return;
       }
   }
   f = fork();
   if (f == 0) {
       if (re == 1 || re == 3 || re  == 4) {
           if (fpin = open(ipf, O_RDONLY) >= 0) {
               dup2(fpin, 0);
               close(fpin);
           }
           else {
               perror("Error");
           }
       }
       if (re != 4 && re > 0) {
           if (re == 1 || re == 2) {
               fpout = open(opf, O_WRONLY | O_APPEND | O_CREAT, 0644);
           }
           else if (re == 3 || re == 5) {
               fpout = open(opf, O_WRONLY | O_TRUNC | O_CREAT, 0644);
           }
           if (fpout < 0) {
               printf("Unable to access output file\n");
               return;
           }
               dup2(fpout, 1);
               close(fpout);
       }
       if (execvp(info[0], info) == -1) {
           printf("No such command\n");
           return;
       }
       exit(0);
   }
   else if (f > 0) {
       return;
   }
   else {
       printf("Fork failed\n");
       return;
   }
   return;
}
 
int checkpipe (char * commands) {
   if (strstr(commands, "|")) {
       return 1;
   }
   return 0;
}
 
 
void pipes (char * commands) {
   int count = 0;
   char * args[1000];
   char * tok = strtok(commands, "|");
   while (tok != NULL) {
       args[count] = tok;
       count++;
       tok = strtok(NULL, "|");
   }
   for (int i = 0; i < count; i++) {
       int pid, change, pipearr[2];
       pipe(pipearr);
       pid = fork();
       if (pid == 0) {
           dup2(change, 0);
           if (args[i + 1] != NULL) {
               dup2(pipearr[1], 1);
           }
           close(pipearr[0]);
           loop(hdn, args[i]);
           exit(0);
       }
       else if (pid > 0) {
           wait(NULL);
           close(pipearr[1]);
           change = pipearr[0];
       }
       else {
           perror("Error: Fork Failed\n");
       }
   }
}
 
void loop (char * homedir, char * commandslist) {
   char space[2] = " ", semicolon[2] = ";";
   size_t size = 100000;
   char currentdir[size];
   if (checkpipe(commandslist)) {
       pipes(commandslist);
       return;
   }
   if (redir(commandslist)) {
       red(commandslist);
       printf("Done\n");
       fflush(stdout);
       return;
   }
   char * commands = strtok(commandslist, " ");
   if (strcmp(commands, "echo") == 0) {
       commands = strtok(NULL, space);
       echo(commands);
   }
   else if (strcmp(commands, "pwd") == 0) {
       pwd();
   }
   else if (strcmp(commands, "cd") == 0) {
       commands = strtok(NULL, space);
       cd(commands, homedir);
   }
   else if (strcmp(commands, "ls") == 0) {
       commands = strtok(NULL, " ");
       ls(commands, homedir);
   }
   else if (strcmp(commands, "pinfo") == 0) {
       commands = strtok(NULL, " ");
       pinfo(commands, homedir);
   }
   else if (strcmp(commands, "quit") == 0) {
       overkill();
       exit(EXIT_SUCCESS);
   }
   else if (strcmp(commands, "nightswatch") == 0) {
       nw(commands);
   }
   else if (strcmp(commands, "setenv") == 0) {
       commands = strtok(NULL, " ");
       char * name = commands;
       commands = strtok(NULL, " ");
       char * val = commands;
        if (name == NULL || val == NULL) {
           printf("Incorrect number of arguments\n");
        }
        else {
            set(name, val);
        }
   }
   else if (strcmp(commands, "unsetenv") == 0) {
       commands = strtok(NULL, " ");
       char * name = commands;
        if (name == NULL) {
            printf("Name not provided\n");
        }
        else {
            unset(name);
        }
   }
   else if (strcmp(commands, "bg") == 0) {
       back(commands);
   }
   else if (strcmp(commands, "fg") == 0) {
       fore(commands);
   }
   else if (strcmp(commands, "kjob") == 0) {
       kjobs(commands);
   }
   else if (strcmp(commands, "overkill") == 0) {
       overkill();
   }  
   else if (strcmp(commands, "jobs") == 0) {
       job();
   }
   else {
       execute(commands);
   }
}