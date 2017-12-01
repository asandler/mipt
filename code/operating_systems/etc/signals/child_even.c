#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigfunc(int sigtype) {
    static int cur = 2;

    if (sigtype == SIGUSR1) {
        printf("%d\n", cur);
        fflush(stdout);
        cur += 2;
    }
}

int main() {
    fprintf(stderr, "Child 1 executed\n");

    if (signal(SIGUSR1, sigfunc) == SIG_ERR) {
        exit(EXIT_FAILURE);
    }

    kill(getppid(), SIGUSR1);

    while (1) {
        pause();
    }

    return 0;
}
