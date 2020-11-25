#include "headers.h"
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
#include "shell.h"

int main () {
	jobs = 0;
	changed = 0;
	long long int i = 0;
	size_t size = 1000000, homedirsize, currentdirsize, inputsize;
	getcwd(hdn, 1000000);
	char semicolon[2] = ";", space[2] = " ";
	char login[size], hostname[size], homedir[size], currentdir[size], input[size], dirname[size];
	char * ptr = input, * commands, * commandslist[10000];
	gethostname(hostname, size);
	getlogin_r(login, size);
	getcwd(homedir, size);
	processid = getpid();
	homedirsize = strlen(homedir);
	running.pid = processid;
	strcpy(running.name, "shell\0");
	strcpy(old, hdn);
	while (1) {
		signal(SIGCHLD, fexit);
		signal(SIGINT, ctrlc);
		signal(SIGTSTP, ctrlz);
		prompt(hostname, login);
		inputsize = getline(&ptr, &size, stdin);
		input[inputsize - 1] = '\0';
		commands = strtok(input, semicolon);
		i = 0;
		while (commands) {
			commandslist[i] = commands;
			i++;
			commands = strtok(NULL, semicolon);
		}
		for (long long j = 0; j < i; j++) {
			loop(homedir, commandslist[j]);
		}
	}
}