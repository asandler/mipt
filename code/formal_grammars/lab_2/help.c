#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "help.h"

char* copy_to(int n, ...) {
        va_list argList;
        char *next, *answer;
        answer = (char*) malloc(1);
        answer[0] = 0;
        va_start(argList, n);
        while (n--) {
                next = va_arg(argList, char*);
                answer = (char*) realloc(answer, strlen(answer) + strlen(next) + 5);
                strcat(answer, next);
        }
        va_end(argList);
        return answer;
}

void freee(int n, ...) {
        va_list argList;
        void *next;
        va_start(argList, n);
        while (n--) {
                next = va_arg(argList, void*);
                free(next);
        }
        va_end(argList);
}
