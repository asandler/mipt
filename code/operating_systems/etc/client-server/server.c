#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_char(int fd) {
    char buf[1];
    int t;
    t = read(fd, buf, 1);
    if (t == -1 || t == 0)
        return EOF;
    return buf[0];
}

void read_from_FIFO(char* fifo, char* target) {
    int i = 0, fd;
    int c;
    printf("curR: %s ", fifo);
    fflush(stdout);
    fd = open(fifo, O_RDONLY);
    while ((c = read_char(fd)) != '\n' && c != EOF) {
        target[i++] = (char) c;
        printf("%c", c);
        fflush(stdout);
    }
    close(fd);
}

void write_to_FIFO(char* fifo, char* source) {
    int fd = open(fifo, O_WRONLY);
    source[strlen(source)] = '\n';
    write(fd, source, strlen(source));
    close(fd);
}

int main(int argc, char** argv) {
    char move[500], curR[500], curW[500];
    int i;

    strcpy(curR, argv[1]);
    strcpy(curW, argv[2]);

    if (mkfifo(argv[1], 0777) == -1) {
        printf("Can't create FIFO\n");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(argv[2], 0777) == -1) {
        fprintf(stderr, "Can't create FIFO\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 9; i++) {
        read_from_FIFO(curR, move);
        printf("%s\n", move);
        fflush(stdout);
        write_to_FIFO(curW, move);
        if (strcmp(move, "p") == 0) {
            exit(EXIT_FAILURE);
        }
        if (strcmp(curR, argv[1]) == 0) {
            strcpy(curR, argv[2]);
            strcpy(curW, argv[1]);
        } else {
            strcpy(curR, argv[1]);
            strcpy(curW, argv[2]);
        }
    }
    write_to_FIFO(curW, "g\n");
    write_to_FIFO(curR, "g\n");
    return 0;
}
