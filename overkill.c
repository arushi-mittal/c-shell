#include "headers.h"
#include "overkill.h"

void overkill () {
    for (long long int i = 0; i < jobs; i++) {
        kill(jobinfo[i].pid, SIGKILL);
    }
    while (jobs--) {
        jobinfo[jobs].pid = 0;
        jobinfo[jobs].name[0] = '\0';
    }
    jobs = 0;
}