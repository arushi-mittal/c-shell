#include "headers.h"
#include "ls.h"

char * reformat (char * homedir, char * commands) {
        long long int x;
        char temp[100000];
        strcpy(temp, homedir);
        size_t n = strlen(commands), h = strlen(homedir);
        for (x = 1; x < n; x++) {
            temp[h + x - 1] = commands[x];
        }
        temp[h + x - 1] = '\0';
        char * tempptr = temp;
        return tempptr;
}

void noflags (char * dirname) {
    DIR * directory;
    struct dirent * current;
    if (directory = opendir(dirname)) {
    while ((current = readdir(directory)) != NULL) {
        if (current->d_name[0] != '.') {
            printf("%s\n", current->d_name);
        }
    }
    closedir(directory);
    }
    else {
        perror("Error");
    }
}

void lsall (char * dirname) {
    DIR * directory;
    struct dirent * current;
    if (directory = opendir(dirname)) {
        while ((current = readdir(directory)) != NULL) {
            printf("%s\n", current->d_name);
        }
        closedir(directory);
    }
    else {
        perror("Error");
    }
}

void lslistall (char * dirname, char * homedir) {
	char file[10000];
	struct stat st;
	struct dirent ** namelist;
	DIR * directory;
	long long n = scandir(dirname, &namelist, NULL, alphasort);
	if (n >= 0) {
		while (n--) {
			strcpy(file, dirname);
			strcat(file, "/");
			if (namelist[n]->d_name[0] == '~') {
				strcat(file, reformat(homedir, namelist[n]->d_name));
			}
			else {
				strcat(file, namelist[n]->d_name);
			}
			if (stat(file, &st) == -1) {
				perror("Error");
			}
			else {
				mode_t m = st.st_mode;
				char permissions[25] = "----------\0";
				int i = 0;
				if (S_ISDIR(m)) permissions[i] = 'd';
				i++;
				if (m & S_IRUSR) permissions[i] = 'r';
				i++;
				if (m & S_IWUSR) permissions[i] = 'w';
				i++;
				if (m & S_IXUSR) permissions[i] = 'x';
				i++;
				if (m & S_IRGRP) permissions[i] = 'r';
				i++;
				if (m & S_IWGRP) permissions[i] = 'w';
				i++;
				if (m & S_IXGRP) permissions[i] = 'x';
				i++;
				if (m & S_IROTH) permissions[i] = 'r';
				i++;
				if (m & S_IWOTH) permissions[i] = 'w';
				i++;
				if (m & S_IXOTH) permissions[i] = 'x';
				
				long long int links = st.st_nlink;
				
				struct passwd * p = getpwuid(st.st_uid);
				struct group * g = getgrgid(st.st_gid);
				char * uid = p->pw_name, * gid = g->gr_name;

				long long sz = st.st_size;

				char time[50];
				strftime(time, 50, "%h %e %H:%M", localtime(&st.st_mtime));
				
				printf("%s %3lld %10s %10s %6lld  %s %s\n", permissions, links, uid, gid, sz, time, namelist[n]->d_name);
			}
		}
	}
	else {
		perror("Error");
	}
}
void lslist (char * dirname, char * homedir) {
	char file[10000];
	struct stat st;
	struct dirent ** namelist;
	DIR * directory;
	long long n = scandir(dirname, &namelist, NULL, alphasort);
	if (n >= 0) {
		while (n--) {
			strcpy(file, dirname);
			strcat(file, "/");
			if (namelist[n]->d_name[0] == '~') {
				strcat(file, reformat(homedir, namelist[n]->d_name));
			}
			else {
				strcat(file, namelist[n]->d_name);
			}
			if (stat(file, &st) == -1) {
				perror("Error");
			}
			else {
				if (strcmp(namelist[n]->d_name, ".") == 0 || strcmp(namelist[n]->d_name, "..") == 0) {
					continue;
				}
				mode_t m = st.st_mode;
				char permissions[25] = "----------\0";
				int i = 0;
				if (S_ISDIR(m)) permissions[i] = 'd';
				i++;
				if (m & S_IRUSR) permissions[i] = 'r';
				i++;
				if (m & S_IWUSR) permissions[i] = 'w';
				i++;
				if (m & S_IXUSR) permissions[i] = 'x';
				i++;
				if (m & S_IRGRP) permissions[i] = 'r';
				i++;
				if (m & S_IWGRP) permissions[i] = 'w';
				i++;
				if (m & S_IXGRP) permissions[i] = 'x';
				i++;
				if (m & S_IROTH) permissions[i] = 'r';
				i++;
				if (m & S_IWOTH) permissions[i] = 'w';
				i++;
				if (m & S_IXOTH) permissions[i] = 'x';
				
				long long int links = st.st_nlink;
				
				struct passwd * p = getpwuid(st.st_uid);
				struct group * g = getgrgid(st.st_gid);
				char * uid = p->pw_name, * gid = g->gr_name;

				long long sz = st.st_size;

				char time[50];
				strftime(time, 50, "%h %e %H:%M", localtime(&st.st_mtime));
				
				printf("%s %3lld %10s %10s %6lld  %s %s\n", permissions, links, uid, gid, sz, time, namelist[n]->d_name);
			}
		}
	}
	else {
		perror("Error");
	}
}

void ls (char * commands, char * homedir) {
    bool aflag = 0, lflag = 0;
    char * dirnames[1000];
    long long int i = 0;
    if (commands == NULL) {
        noflags(".");
    }
    else {
        while (commands) {
            if (commands[0] == '-') {
                if (strstr(commands, "a")) {
                    aflag = 1;
                }
                if (strstr(commands, "l")) {
                    lflag = 1;
                }
            }
            else {
                dirnames[i] = commands;
                i++;
            }
            commands = strtok(NULL, " ");
        }
        if (!lflag && !aflag) {
            if (i == 0) {
                noflags(".");
            }
            else if (i == 1) {
                if (dirnames[0][0] == '~') {
                    char * temp = reformat(homedir, dirnames[0]);
                    noflags(temp);
                }
                else {
                    noflags(dirnames[0]);
                }
            }
            else if (i > 1) {
                for (long long int x = 0; x < i; x++) {
                    if (dirnames[x][0] == '~') {
                        char * temp = reformat(homedir, dirnames[x]);
                        printf("\033[1;34m%s:\033[0m\n", temp);
                        noflags(temp);
                    }
                    else {
                        printf("\033[1;34m%s:\033[0m\n", dirnames[x]);
                        noflags(dirnames[x]);
                    }
                    printf("\n\n");
                }
            }
        }
        if (lflag && aflag) {
            if (i == 0) {
                lslistall(".", homedir);
            }
            else if (i == 1) {
                if (dirnames[0][0] == '~') {
                    char * temp = reformat(homedir, dirnames[0]);
                    lslistall(temp, homedir), homedir;
                }
                else {
                    lslistall(dirnames[0], homedir);
                }
            }
            else if (i > 1) {
                for (long long int x = 0; x < i; x++) {
                    if (dirnames[x][0] == '~') {
                        char * temp = reformat(homedir, dirnames[x]);
                        printf("\033[1;34m%s:\033[0m\n", temp);
                        lslistall(temp, homedir);
                    }
                    else {
                        printf("\033[1;34m%s:\033[0m\n", dirnames[x]);
                        lslistall(dirnames[x], homedir);
                    }
                    printf("\n\n");
                }
            }
        }
        if (lflag && !aflag) {
            if (i == 0) {
                lslist(".", homedir);
            }
            else if (i == 1) {
                if (dirnames[0][0] == '~') {
                    char * temp = reformat(homedir, dirnames[0]);
                    lslist(temp, homedir), homedir;
                }
                else {
                    lslist(dirnames[0], homedir);
                }
            }
            else if (i > 1) {
                for (long long int x = 0; x < i; x++) {
                    if (dirnames[x][0] == '~') {
                        char * temp = reformat(homedir, dirnames[x]);
                        printf("\033[1;34m%s:\033[0m\n", temp);
                        lslist(temp, homedir);
                    }
                    else {
                        printf("\033[1;34m%s:\033[0m\n", dirnames[x]);
                        lslist(dirnames[x], homedir);
                    }
                    printf("\n\n");
                }
            }
        }
        if (aflag && !lflag) {
            if (i == 0) {
                lsall(".");
            }
            else if (i == 1) {
                if (dirnames[0][0] == '~') {
                    char * temp = reformat(homedir, dirnames[0]);
                    lsall(temp);
                }
                else {
                    lsall(dirnames[0]);
                }
            }
            else if (i > 1) {
                for (long long int x = 0; x < i; x++) {
                    if (dirnames[x][0] == '~') {
                        char * temp = reformat(homedir, dirnames[x]);
                        printf("\033[1;34m%s:\033[0m\n", temp);
                        lsall(temp);
                    }
                    else {
                        printf("\033[1;34m%s:\033[0m\n", dirnames[x]);
                        lsall(dirnames[x]);
                    }
                    printf("\n\n");
                }
            }
        }
    }
}