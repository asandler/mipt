#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define DIF_BASES 1
#define OUT_OF_MEMORY 2
#define DIV_ZERO 3

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef unsigned char small;

typedef struct {
        small *data; 
        size_t length, alloc_size, base;
        char sign;
} bigint;

int sum(bigint *a, bigint *b, bigint *answer);

void *safe_realloc(void *memory, size_t size) {
        memory = realloc(memory, size);
        if (memory == NULL) {
                fprintf(stderr, "long arifmetics: safe_realloc(): Out of memory");
                exit(OUT_OF_MEMORY);
        }
        return memory;
}

void add_to_number(bigint *n, small c) {
        if (n->alloc_size == 0) {
                n->data = safe_realloc(n->data, sizeof(small));
                n->alloc_size = 1;
        }
        if (n->length == n->alloc_size) {
                n->data = safe_realloc(n->data, n->alloc_size * 2 * sizeof(small));
                n->alloc_size *= 2;
        }
        n->data[n->length++] = c;
}

bigint *init_number(size_t base) {
        bigint *t = NULL;
        t = safe_realloc(t, sizeof(bigint));
        t->data = NULL;
        t->alloc_size = t->sign = t->length = 0;
        t->base = base;
        return t;
}

void free_number(bigint *n) {
        free(n->data);
        free(n);
}

void reverse(bigint *n) {
        size_t i;
        small t;
        for (i = 0; i < n->length / 2; i++) {
                t = n->data[i];
                n->data[i] = n->data[n->length - i - 1];
                n->data[n->length - i - 1] = t;
        }
}

void print_number(bigint *n) {
        int i;
        if (n->sign == 0 || n->length == 0) {
                printf("0\n");
                return;
        }
        if (n->sign < 0)
                printf("-");
        for (i = n->length - 1; i >= 0; i--)
                printf("%d", n->data[i]);
        printf("\n");
}

void read_number(bigint *n) {
        int c = getchar();
        n->length = n->sign = 0;
        if (c == '-') {
                n->sign = -1;
                c = getchar();
        } else
                if  (c == '+')
                        c = getchar();
        while (c == '0')
                c = getchar();
        while ('0' <= c && c <= '9') {
                add_to_number(n, (small)(c - '0'));
                c = getchar();
        }
        if (!n->length)
                n->sign = 0;
        if (n->length && !n->sign)
                n->sign = 1;
        reverse(n);
        while (c != '\n' && c != EOF)
                c = getchar();
}

char compare(bigint *a, bigint *b) {
        int t;
        if (a->sign != b->sign)
                return (a->sign > b->sign) ? 1 : -1;
        if (a->length != b->length)
                return (a->length > b->length) ? a->sign : -a->sign;
        for (t = a->length - 1; t >= 0; t--)
                if (a->data[t] != b->data[t])
                        return (a->data[t] > b->data[t]) ? a->sign : -a->sign;
        return 0;
}

void copy_number(bigint *target, bigint *source) {
        int i;
        if (target == source)
                return;
        target->length = 0;
        target->base = source->base;
        target->sign = source->sign;
        for (i = 0; i < source->length; i++)
                add_to_number(target, source->data[i]);
        target->length = source->length;
}

void mult_on_small(bigint *a, size_t s, bigint *answer) {
        int memory = 0, i, tempsum;
        bigint *temp;
        if (s == 0) {
                answer->sign = answer->length = 0;
                return;
        }
        temp = init_number(a->base);
        temp->sign = 1;
        for (i = 0; i < a->length; i++) {
                tempsum = a->data[i] * s + memory;
                add_to_number(temp, tempsum % a->base);
                memory = tempsum / a->base;
        }
        if (memory)
                add_to_number(temp, (small) memory);
        copy_number(answer, temp);
        free_number(temp);
}

int mult(bigint *a, bigint *b, bigint *answer) {
        int i;
        bigint *temp, *ans;
        if (a->base != b->base) {
                fprintf(stderr, "long ariphmetics: mult(): Different bases\n");
                return DIF_BASES;
        }
        if (a->sign == 0 || b->sign == 0) {
                answer->length = answer->sign = 0;
                return OK;
        }
        ans = init_number(a->base);
        temp = init_number(a->base);
        for (i = b->length - 1; i >= 0; i--) {
                mult_on_small(ans, a->base, ans);
                mult_on_small(a, b->data[i], temp);
                sum(ans, temp, ans);
        }
        if (a->sign != b->sign)
                ans->sign = -1;
        else
                ans->sign = 1;
        copy_number(answer, ans);
        free_number(temp);
        free_number(ans);
        return OK;
}

int diff(bigint *a, bigint *b, bigint *answer) {
        size_t i, j;
        bigint *temp, *ans;
        if (a->base != b->base) {
                fprintf(stderr, "long ariphmetics: diff(): Different bases\n");
                return DIF_BASES;
        }
        if (a->sign == 0) {
                copy_number(answer, b);
                answer->sign *= -1;
                return OK;
        }
        if (b->sign == 0) {
                copy_number(answer, a);
                return OK;
        }
        switch (compare(a, b)) {
        case 0:
                answer->sign = answer->length = 0;
                return OK;
        case 1:
                if (a->sign != b->sign) {
                        temp = init_number(a->base);
                        if (a->sign > 0) {
                                copy_number(temp, b);
                                temp->sign = 1;
                                sum(a, temp, answer);
                        } else {
                                copy_number(temp, a);
                                temp->sign = 1;
                                sum(temp, b, answer);
                                answer->sign = -1;
                        }
                        free_number(temp);
                        return OK;
                }
                temp = init_number(a->base);
                ans = init_number(a->base);
                if (a->sign == -1) {
                        copy_number(temp, a);
                        temp->sign *= -1;
                        copy_number(ans, b);
                        ans->sign *= -1;
                        diff(ans, temp, answer);
                        free_number(ans);
                        free_number(temp);
                        return OK;
                }
                copy_number(temp, a);
                ans->sign = 1;
                ans->length = 0;
                for (i = 0; i < temp->length; i++) {
                        if (i >= b->length)
                                add_to_number(ans, temp->data[i]);
                        else {
                                if (temp->data[i] < b->data[i]) {
                                        j = i;
                                        do {
                                                temp->data[j] += (i == j) ? a->base : a->base - 1;
                                                j++;
                                        } while (j < temp->length && temp->data[j] == 0);
                                        temp->data[j]--;
                                }
                                add_to_number(ans, temp->data[i] - b->data[i]);
                        }
                }
                while (!ans->data[ans->length - 1])
                        ans->length--;
                copy_number(answer, ans);
                free_number(ans);
                free_number(temp);
                return OK;
        case -1:
                diff(b, a, answer);
                answer->sign = -1;
                return OK;
        }
        return OK;
}

int sum(bigint *a, bigint *b, bigint *answer) {
        size_t i, c, memory = 0, mlen = max(a->length, b->length);
        bigint* temp;
        if (a->base != b->base) {
                fprintf(stderr, "long ariphmetics: sum(): Different bases\n");
                return DIF_BASES;
        }
        if (a->sign == 0) {
                copy_number(answer, b);
                return OK;
        }
        if (b->sign == 0) {
                copy_number(answer, a);
                return OK;
        }
        if (a->sign != b->sign) {
                temp = init_number(a->base);
                copy_number(temp, a);
                temp->sign *= -1;
                if (compare(b, temp) == 0) {
                        answer->length = answer->sign = 0;
                        free_number(temp);
                        return 0;
                }
                if (a->sign > 0) {
                        copy_number(temp, b);
                        temp->sign *= -1;
                        diff(a, temp, answer);
                } else {
                        diff(b, temp, answer);
                }
                free_number(temp);
                return OK;
        }
        temp = init_number(a->base);
        temp->base = a->base;
        temp->sign = a->sign;
        for (i = 0; i < mlen; i++) {
                c = memory;
                if (i < a->length)
                        c += a->data[i];
                if (i < b->length)
                        c += b->data[i];
                add_to_number(temp, c % answer->base);
                memory = c / answer->base;
        }
        if (memory)
                add_to_number(temp, memory);
        copy_number(answer, temp);
        free_number(temp);
        return OK;
}

int divis(bigint *a, bigint *b, bigint *answer) {
        int i, j;
        bigint *temp, *ans, *temp2;
        if (a->base != b->base) {
                fprintf(stderr, "long ariphmetics: div(): Different bases");
                return DIF_BASES;
        }
        if (b->sign == 0 || b->length == 0) {
                fprintf(stderr, "long ariphmetics: div(): Division by zero");
                return DIV_ZERO;
        }
        if (a->sign == 0 || a->length == 0) {
                answer->sign = answer->length = 0;
                return OK;
        }
        temp = init_number(a->base);
        temp2 = init_number(a->base);
        ans = init_number(a->base);
        i = a->length;
        copy_number(temp2, b);
        temp->sign = temp2->sign = 1;
        while (i > 0 && compare(temp, temp2) < 0) {
                i--;
                reverse(temp);
                add_to_number(temp, a->data[i]);
                reverse(temp);
        }
        if (compare(temp, temp2) < 0)
                ans->sign = ans->length = 0;
        else
                if (a->sign != b->sign)
                        ans->sign = -1;
                else
                        ans->sign = 1;
        while (i >= 0) {
                for (j = 0; j < a->base && compare(temp, temp2) >= 0; j++) {
                        diff(temp, temp2, temp);
                }
                add_to_number(ans, j);
                mult_on_small(temp, a->base, temp);
                if (temp->length == 0 || temp->sign == 0) {
                        temp->sign = temp->length = 1;
                }
                i--;
                if (i < 0)
                        break;
                temp->data[0] = a->data[i];
        }
        reverse(ans);
        if (a->sign != b->sign) {
                temp->sign = 1;
                temp->length = 0;
                add_to_number(temp, 1);
                diff(ans, temp, ans);
        }
        copy_number(answer, ans);
        free_number(ans);
        free_number(temp);
        free_number(temp2);
        return OK;
}

int mod(bigint *a, bigint *b, bigint *answer) {
        bigint *temp, *temp2;
        if (a->base != b->base) {
                fprintf(stderr, "long ariphmetics: mod(): Different bases\n");
                return DIF_BASES;
        }
        if (b->sign == 0 || b->length == 0) {
                fprintf(stderr, "long ariphmetics: mod(): Division by zero\n");
                return DIV_ZERO;
        }
        if (a->sign == 0) {
                answer->sign = answer->length = 0;
                return OK;
        }
        if (a->sign != b->sign) {
                temp = init_number(a->base);
                temp2 = init_number(a->base);
                copy_number(temp, a);
                copy_number(temp2, b);
                temp->sign = temp2->sign = 1;
                mod(temp, temp2, temp);
                temp->sign = 1;
                diff(temp2, temp, temp);
                temp->sign = b->sign;
                copy_number(answer, temp);
                free_number(temp);
                free_number(temp2);
        } else {
                temp = init_number(a->base);
                if (a->sign < 0) {
                        temp2 = init_number(a->base);
                        copy_number(temp, a);
                        temp->sign = 1;
                        copy_number(temp2, b);
                        temp2->sign = 1;
                        mod(temp, temp2, temp);
                        temp->sign = -1;
                        copy_number(answer, temp);
                        free_number(temp2);
                } else {
                        divis(a, b, temp);
                        mult(temp, b, temp);
                        diff(a, temp, temp);
                        temp->sign = a->sign;
                        copy_number(answer, temp);
                        free_number(temp);
                }
        }
        return OK;
}

int _dec_to_256(bigint *a, bigint *answer) {
        int i, digit;
        bigint *temp, *_256, *ans, *temp2;
        if (a->base != 10) {
                fprintf(stderr, "long ariphmetics: _dec_to_256(): decimal base expected\n");
                return DIF_BASES;
        }
        temp = init_number(10);
        temp2 = init_number(10);
        ans = init_number(256);
        _256 = init_number(10);
        _256->sign = 1;
        add_to_number(_256, (small) 6);
        add_to_number(_256, (small) 5);
        add_to_number(_256, (small) 2);
        copy_number(temp, a);
        temp->sign = 1;
        while (temp->sign && temp->length) {
                mod(temp, _256, temp2);
                divis(temp, _256, temp);
                for (i = temp2->length - 1, digit = 0; i >= 0; i--)
                        digit = digit * 10 + temp2->data[i];
                add_to_number(ans, (small) digit);
        }
        ans->sign = a->sign;
        copy_number(answer, ans);
        free_number(_256);
        free_number(temp);
        free_number(temp2);
        free_number(ans);
        return OK;
}

int _256_to_dec(bigint *a, bigint *answer) {
        int i, digit;
        bigint *temp, *_10, *ans, *temp2;
        if (a->base != 256) {
                fprintf(stderr, "long ariphmetics: _256_to_dec(): 256-base expected\n");
                return DIF_BASES;
        }
        temp = init_number(256);
        temp2 = init_number(256);
        ans = init_number(10);
        _10 = init_number(256);
        _10->sign = 1;
        add_to_number(_10, (small) 10);
        copy_number(temp, a);
        temp->sign = 1;
        while (temp->sign && temp->length) {
                mod(temp, _10, temp2);
                divis(temp, _10, temp);
                for (i = temp2->length - 1, digit = 0; i >= 0; i--)
                        digit = digit * 10 + temp2->data[i];
                add_to_number(ans, (small) digit);
        }
        ans->sign = a->sign;
        copy_number(answer, ans);
        free_number(_10);
        free_number(temp);
        free_number(temp2);
        free_number(ans);
        return OK;
}

int main() {
        bigint *r = init_number(10);
        read_number(r);
        _dec_to_256(r, r);
        _256_to_dec(r, r);
        print_number(r);
        free_number(r);
        return 0;
}
