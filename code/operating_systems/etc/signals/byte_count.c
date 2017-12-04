#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>

typedef unsigned long long ull;
ull statistics[26];
ull inputSize;

void print_stats(int sign) {
    ull ans = 0;
    for (int i = 'a'; i <= 'z'; ++i) {
        ans += statistics[i - 'a'];
    }

    printf("Input size: %llu\n", inputSize);
    printf("Letters count: %llu\n", ans);
    printf("Char counts:\n");
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        printf("%c: %llu\n", ch, statistics[ch - 'a']);
    }
    signal(SIGUSR1, print_stats);
}

int main() {
    signal(SIGUSR1, print_stats);
    char buf[256];
    int bytes;

    while (1) {
        if ((bytes = read(STDIN_FILENO, buf, 1)) != -1) {
            int buf_len = strlen(buf);
            inputSize += buf_len;

            for (int i = 0; i < buf_len; ++i) {
                 if (buf[i] >= 'a' && buf[i] <= 'z') {
                    ++statistics[buf[i] - 'a'];
                 }
            }
        } else {
            printf("%d\n", errno);
            error(1, errno, "main");
        }
    }

    return 0; 
}
