#include "headers.h"
#include "env.h"

void set(char * name, char * val) {
    if (setenv(name, val, 1)) {
        perror("Error");
    }
    else if (getenv(name)) {
        printf("%s set to %s\n", name, getenv(name));
    }
}
void unset(char * name) {
    if (unsetenv(name)) {
        perror("Error");
    }
    else {
        printf("%s removed\n", name);
    }
}