#include "headers.h"
#include "proc.h"
#include "prompt.h"

void removejob (long long int x) {
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

void fexit() {
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	// if (pid < 0) {
	// 	printf("Failed\n");
	// 	return;
	// }
	// if (WIFEXITED(status)) {
	// 	for (int i = 0; i < jobs; i++) {
	// 		printf("done\n");
	// 		if (jobpid[i] == pid) {
	// 			printf("\n%s exited normally with pid %lld\n", jobnames[i], jobpid[i]);
	// 			char host[100000], user[100000];
	// 			gethostname(host, 100000); getlogin_r(user, 100000);
	// 			prompt(host, user);
	// 			fflush(stdout);
	// 		removejob(i);
	// 		}
	// 	}
	// }
	if (pid >= 0) {
		for (int i = 0; i < jobs; i++) {
			if (WIFEXITED(status)) {
				if (jobinfo[i].pid == pid) {
					if (WEXITSTATUS(status)) {
						printf("\n%s exited abnormally with pid %lld\n", jobinfo[i].name, jobinfo[i].pid);
					}
					else {
						printf("\n%s exited normally with pid %lld\n", jobinfo[i].name, jobinfo[i].pid);
					}
					char host[100000], user[100000];
					gethostname(host, 100000); getlogin_r(user, 100000);
					prompt(host, user);
					fflush(stdout);
					removejob(i);
				}
			}
		}
	}
	else {
		return;
	}
}