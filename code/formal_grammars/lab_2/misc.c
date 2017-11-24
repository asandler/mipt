#include <stdlib.h>
#include <stdio.h>
#include "oberon.tab.h"

int pow_int(int x, int n) {
    if (n < 0) {
        printf("Runtime error: negative power.\n");
        exit(1);
    }
    if (n == 0)
        return 1;
    else
        return x * pow_int(x, n - 1);
}

extern FILE* yyin;
extern YYSTYPE yylval;

int main(int argc, char** argv) {
    FILE* handle;
    if (argc != 2) {
        printf("Usage: %s program\n", argv[0]);
        exit(1);
    }
    if (!(handle = fopen(argv[1], "r"))) {
        printf("Error: cannot open file\n");
        exit(1);
    }
    yyin = handle;
    yylval.string = malloc(1);
    yylval.string[0] = 0;
    yyparse();
}

void yyerror(char* s) {
    printf("Parser error: %s\n", s);
    free(yylval.string);
    exit(1);
}
