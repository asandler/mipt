#include <errno.h>
#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigfunc(int sigtype) {}

int read_digit_from_fd(int fd) {
    char buf[1];
    read(fd, buf, 1);
    if (buf[0] != '\n')
        return buf[0] - '0';
    else
        return -1;
}

int read_from_fd(int fd, int pid) {
    int answer = 0, t;
    kill(pid, SIGUSR1);
    while ((t = read_digit_from_fd(fd)) != -1) {
        answer = answer * 10 + t;
    }
    return answer;
}

int main(int argc, char** argv) {
    int fd1[2], fd2[2], pid1, pid2, n, i, fdcur, pidcur;

    if (signal(SIGUSR1, sigfunc) == SIG_ERR) {
        fprintf(stderr, "Can't handle signal\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd1) == -1) {
        fprintf(stderr, "Can't create pipe\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2) == -1) {
        fprintf(stderr, "Can't create pipe\n");
        exit(EXIT_FAILURE);
    }

    if ((pid1 = fork()) == 0) {
        dup2(fd1[1], STDOUT_FILENO);
        if (execve(argv[1], argv + 2, (char**) NULL) == -1) {
            error(1, errno, "parent");
        }
    } else {
        if ((pid2 = fork()) == 0) {
            dup2(fd2[1], STDOUT_FILENO);
            if (execve(argv[2], argv + 2, (char**) NULL) == -1) {
                error(1, errno, "parent");
            }
        } else {
            scanf("%d", &n);
            fdcur = fd1[0];
            pidcur = pid1;
            for (i = 0; i < n; i++) {
                printf("%d ", read_from_fd(fdcur, pidcur));
                fdcur = (fdcur == fd1[0]) ? fd2[0] : fd1[0];
                pidcur = (pidcur == pid1) ? pid2 : pid1;
            }
            printf("\n");
        }
    }

    return 0;
}
