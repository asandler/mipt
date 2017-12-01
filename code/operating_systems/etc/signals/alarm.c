#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ioctl.h>


int sigcount = 0;
struct itimerval timer;

void sigfunc(int sigtype) {
        if (sigtype == SIGINT) {
                sigcount++;
        }
        if (sigtype == SIGALRM) {
                printf("\nDon't sleep!\n");
        }
        if (sigtype == SIGUSR1) {
                srand(time(0));
                exit(rand() % 100);
        }
}

void set_timer(int sec, int usec) {
        timer.it_interval.tv_sec = sec;
        timer.it_interval.tv_usec = usec;
        timer.it_value.tv_sec = sec;
        timer.it_value.tv_usec = usec;
        setitimer(ITIMER_REAL, &timer, NULL);
}

int main() {
        int pid, status;

        if ((pid = fork()) < 0) {
                perror("vanya2");
                exit(EXIT_FAILURE);
        }
        if (pid > 0) {
                if (signal(SIGINT, sigfunc) == SIG_ERR) {
                        fprintf(stderr, "can't handle a signal SIGINT\n");
                        exit(EXIT_FAILURE);
                }
                if (signal(SIGALRM, sigfunc) == SIG_ERR) {
                        fprintf(stderr, "can't handle a signal SIGINT\n");
                        exit(EXIT_FAILURE);
                }
                set_timer(10, 0);
                while (sigcount < 5) {
                        pause();
                }
                kill(pid, SIGUSR1);
                wait(&status);
                printf("\n%d\n", WEXITSTATUS(status));
        } else {
                setsid();
                if (signal(SIGUSR1, sigfunc) == SIG_ERR) {
                        fprintf(stderr, "can't handle a signal SIGINT\n");
                        exit(EXIT_FAILURE);
                }
                while (1) {
                        pause();
                }
        }
        return 0;
}
