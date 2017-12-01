#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char game[3][3] = {
    {'-','-','-'},
    {'-','-','-'},
    {'-','-','-'}
};

int won(char c) {
    if (game[0][0] == c && game[1][0] == c && game[2][0] == c)
        return 1;
    if (game[0][1] == c && game[1][1] == c && game[2][1] == c)
        return 1;
    if (game[0][2] == c && game[1][2] == c && game[2][2] == c)
        return 1;
    if (game[0][0] == c && game[0][1] == c && game[0][2] == c)
        return 1;
    if (game[1][0] == c && game[1][1] == c && game[1][2] == c)
        return 1;
    if (game[2][0] == c && game[2][1] == c && game[2][2] == c)
        return 1;
    if (game[0][0] == c && game[1][1] == c && game[2][2] == c)
        return 1;
    if (game[0][2] == c && game[1][1] == c && game[2][0] == c)
        return 1;
    return 0;

}

void set_move(char* move) {
    game[move[0] - '1'][move[2] - '1'] = move[4];
}

int empty(int i, int j) {
    return (game[i][j] == '-');
}

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
    fd = open(fifo, O_RDONLY);
    while ((c = read_char(fd)) != '\n' && c != EOF)
        target[i++] = (char) c;
    close(fd);
}

void write_to_FIFO(char* fifo, char* source) {
    int fd = open(fifo, O_WRONLY);
    write(fd, source, strlen(source));
    write(fd, "\n", 1);
    close(fd);
}


char* check(char* move) {
    if (strlen(move) > 5) {
        printf("0");
        fflush(stdout);
        return "p";
    }
    if (move[0] < '1' || move[0] > '3') {
        printf("1");
        fflush(stdout);
        return "p";
    }
    if (move[1] != '_') {
        printf("2");
        fflush(stdout);
        return "p";
    }
    if (move[2] < '1' || move[2] > '3') {
        printf("3");
        fflush(stdout);
        return "p";
    }
    if (move[3] != '_') {
        printf("4");
        fflush(stdout);
        return "p";
    }
    if (move[4] != 'X' && move[4] != 'O') {
        printf("5");
        fflush(stdout);
        return "p";
    }
    if (!empty(move[0] - '1', move[2] - '1')) {
        printf("6");
        fflush(stdout);
        return "p";
    }
    return move;
}

void draw() {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++)
            printf("%c", game[i][j]);
        printf("\n");
    }
}

void PrintEndOfGame(char* move) {
    if (strcmp(move, "p") == 0) {
        fprintf(stderr, "The other client has fallen with presentation error. The game will be terminated\n");
    } else {
        if (won('X')) {
            printf("First won\n");
        } else {
            if (won('O')) {
                printf("Second won\n");
            } else {
                printf("Round draw\n");
            }
        }
    }
}

int main(int argc, char** argv) {
    char move[500];
    int k;

    k = (int) (argv[2][0] - '0');
    printf("%i\n", k);
    fflush(stdout);

    while (1) {
        if (k == 1) {
            scanf("%s", move);
            if (strcmp(check(move), move)) {
                fprintf(stderr, "Presentation error\n");
                write_to_FIFO(argv[1], check(move));
                exit(EXIT_FAILURE);
            } else {
                write_to_FIFO(argv[1], move);
                set_move(move);
                draw();
            }
            read_from_FIFO(argv[1], move);
            if (strcmp(check(move), move)) {
                /*
                   write, who won and exit
                 */
                PrintEndOfGame(move);
                exit(EXIT_SUCCESS);
            } else {
                set_move(move);
                draw();
            }
        } else {
            read_from_FIFO(argv[1], move);
            if (strcmp(check(move), move)) {
                /*
                   write, who won and exit
                 */
                PrintEndOfGame(move);
                exit(EXIT_SUCCESS);
            } else {
                set_move(move);
                draw();
            }
            scanf("%s", move);
            if (strcmp(check(move), move)) {
                fprintf(stderr, "Presentation error\n");
                write_to_FIFO(argv[1], check(move));
                exit(EXIT_FAILURE);
            } else {
                write_to_FIFO(argv[1], move);
                set_move(move);
                draw();
            }
        }
    }
    return 0;
}
