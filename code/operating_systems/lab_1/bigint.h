#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define OK 0
#define DIF_BASES 1
#define OUT_OF_MEMORY 2
#define DIV_ZERO 3

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef unsigned char small;

typedef struct {
    small* data;
    size_t length, alloc_size, base;
    char sign;
} bigint;

int sum(bigint* a, bigint* b, bigint* answer);
void free_number(bigint* n);

int add_to_number(bigint* n, small c) {
    if (n->alloc_size == 0) {
        n->data = realloc(n->data, sizeof(small));
        if (n->data != NULL)
            n->alloc_size = 1;
        else
            return OUT_OF_MEMORY;
    }
    if (n->length == n->alloc_size) {
        n->data = realloc(n->data, n->alloc_size * 2 * sizeof(small));
        if (n->data != NULL)
            n->alloc_size *= 2;
        else
            return OUT_OF_MEMORY;
    }
    n->data[n->length++] = c;
    return OK;
}

void init_number(bigint* t, size_t base) {
    t->data = NULL;
    t->alloc_size = t->sign = t->length = 0;
    t->base = base;
}

void init_numbers(int n, ...) {
    va_list argList;
    size_t base;
    va_start(argList, n);
    base = va_arg(argList, size_t);
    while (n--)
        init_number(va_arg(argList, bigint*), base);
    va_end(argList);
}

void free_number(bigint* n) {
    free(n->data);
}

void free_numbers(int n, ...) {
    va_list argList;
    va_start(argList, n);
    while (n--)
        free_number(va_arg(argList, bigint*));
    va_end(argList);
}

int copy_number(bigint* target, bigint* source) {
    int i;
    if (target == source)
        return OK;
    target->length = 0;
    target->base = source->base;
    target->sign = source->sign;
    for (i = 0; i < source->length; i++)
        if (add_to_number(target, source->data[i]) != OK)
            return OUT_OF_MEMORY;
    target->length = source->length;
    return OK;
}

void reverse(bigint* n) {
    size_t i;
    small t;
    for (i = 0; i < n->length / 2; i++) {
        t = n->data[i];
        n->data[i] = n->data[n->length - i - 1];
        n->data[n->length - i - 1] = t;
    }
}

void print_number(bigint* n) {
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

int read_number(bigint* k) {
    bigint n;
    init_numbers(1, k->base, &n);
    int c = getchar();
    if (c == '-') {
        n.sign = -1;
        c = getchar();
    } else if (c == '+')
        c = getchar();
    while (c == '0')
        c = getchar();
    while ('0' <= c && c <= '9') {
        if (add_to_number(&n, (small)(c - '0')) != OK) {
            free_number(&n);
            return OUT_OF_MEMORY;
        }
        c = getchar();
    }
    if (!n.length)
        n.sign = 0;
    if (n.length && !n.sign)
        n.sign = 1;
    reverse(&n);
    if (copy_number(k, &n) != OK) {
        free_number(&n);
        return OUT_OF_MEMORY;
    }
    while (c != '\n' && c != EOF)
        c = getchar();
    free_number(&n);
    return OK;
}

char compare(bigint* a, bigint* b) {
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

int mult_on_small(bigint* a, size_t s, bigint* answer) {
    int memory = 0, i, tempsum;
    bigint temp;
    if (s == 0) {
        answer->sign = answer->length = 0;
        return OK;
    }
    init_numbers(1, a->base, &temp);
    temp.sign = 1;
    for (i = 0; i < a->length; i++) {
        tempsum = a->data[i] * s + memory;
        if (add_to_number(&temp, tempsum % a->base) != OK) {
            free_number(&temp);
            return OUT_OF_MEMORY;
        }
        memory = tempsum / a->base;
    }
    if (memory)
        if (add_to_number(&temp, (small)memory) != OK) {
            free_number(&temp);
            return OUT_OF_MEMORY;
        }
    if (copy_number(answer, &temp) != OK) {
        free_number(&temp);
        return OUT_OF_MEMORY;
    }
    free_number(&temp);
    return OK;
}

int mult(bigint* a, bigint* b, bigint* answer) {
    int i, t;
    bigint temp, ans;
    if (a->base != b->base)
        return DIF_BASES;
    if (a->sign == 0 || b->sign == 0) {
        answer->length = answer->sign = 0;
        return OK;
    }
    init_numbers(2, a->base, &ans, &temp);
    for (i = b->length - 1; i >= 0; i--) {
        if (mult_on_small(&ans, a->base, &ans) != OK) {
            free_numbers(2, &temp, &ans);
            return OUT_OF_MEMORY;
        }
        if (mult_on_small(a, b->data[i], &temp) != OK) {
            free_numbers(2, &temp, &ans);
            return OUT_OF_MEMORY;
        }
        t = sum(&ans, &temp, &ans);
        if (t != OK) {
            free_numbers(2, &temp, &ans);
            return t;
        }
    }
    if (a->sign != b->sign)
        ans.sign = -1;
    else
        ans.sign = 1;
    if (copy_number(answer, &ans) != OK) {
        free_numbers(2, &temp, &ans);
        return OUT_OF_MEMORY;
    }
    free_numbers(2, &temp, &ans);
    return OK;
}

int diff(bigint* a, bigint* b, bigint* answer) {
    size_t i, j;
    int t;
    bigint temp, ans;
    if (a->base != b->base)
        return DIF_BASES;
    if (a->sign == 0) {
        if (copy_number(answer, b) != OK)
            return OUT_OF_MEMORY;
        answer->sign *= -1;
        return OK;
    }
    if (b->sign == 0) {
        if (copy_number(answer, a) != OK)
            return OUT_OF_MEMORY;
        return OK;
    }
    switch (compare(a, b)) {
        case 0:
            answer->sign = answer->length = 0;
            return OK;
        case 1:
            if (a->sign != b->sign) {
                init_numbers(1, a->base, &temp);
                if (a->sign > 0) {
                    if (copy_number(&temp, b) != OK) {
                        free_number(&temp);
                        return OUT_OF_MEMORY;
                    }
                    temp.sign = 1;
                    t = sum(a, &temp, answer);
                    if (t != OK) {
                        free_number(&temp);
                        return t;
                    }
                } else {
                    if (copy_number(&temp, a) != OK) {
                        free_number(&temp);
                        return OUT_OF_MEMORY;
                    }
                    temp.sign = 1;
                    t = sum(&temp, b, answer);
                    if (t != OK) {
                        free_number(&temp);
                        return t;
                    }
                    answer->sign = -1;
                }
                free_number(&temp);
                return OK;
            }
            init_numbers(2, a->base, &ans, &temp);
            if (a->sign == -1) {
                if (copy_number(&temp, a) != OK) {
                    free_numbers(2, &temp, &ans);
                    return OUT_OF_MEMORY;
                }
                temp.sign *= -1;
                if (copy_number(&ans, b) != OK) {
                    free_numbers(2, &temp, &ans);
                    return OUT_OF_MEMORY;
                }
                ans.sign *= -1;
                t = diff(&ans, &temp, answer);
                if (t != OK) {
                    free_numbers(2, &temp, &ans);
                    return t;
                }
                free_numbers(2, &temp, &ans);
                return OK;
            }
            if (copy_number(&temp, a) != OK) {
                free_numbers(2, &temp, &ans);
                return OUT_OF_MEMORY;
            }
            ans.sign = 1;
            ans.length = 0;
            for (i = 0; i < temp.length; i++) {
                if (i >= b->length) {
                    if (add_to_number(&ans, temp.data[i]) != OK) {
                        free_numbers(2, &temp, &ans);
                        return OUT_OF_MEMORY;
                    }
                } else {
                    if (temp.data[i] < b->data[i]) {
                        j = i;
                        do {
                            temp.data[j] += (i == j) ? a->base : a->base - 1;
                            j++;
                        } while (j < temp.length && temp.data[j] == 0);
                        temp.data[j]--;
                    }
                    if (add_to_number(&ans, temp.data[i] - b->data[i]) != OK) {
                        free_numbers(2, &temp, &ans);
                        return OUT_OF_MEMORY;
                    }
                }
            }
            while (!ans.data[ans.length - 1])
                ans.length--;
            if (copy_number(answer, &ans) != OK) {
                free_numbers(2, &temp, &ans);
                return OUT_OF_MEMORY;
            }
            free_numbers(2, &temp, &ans);
            return OK;
        case -1:
            t = diff(b, a, answer);
            if (t != OK) {
                free_numbers(2, &temp, &ans);
                return t;
            }
            answer->sign = -1;
            return OK;
    }
    return OK;
}

int sum(bigint* a, bigint* b, bigint* answer) {
    size_t i, c, memory = 0, mlen = max(a->length, b->length);
    int t;
    bigint temp;
    if (a->base != b->base)
        return DIF_BASES;
    if (a->sign == 0) {
        if (copy_number(answer, b) != OK)
            return OUT_OF_MEMORY;
        return OK;
    }
    if (b->sign == 0) {
        if (copy_number(answer, a) != OK)
            return OUT_OF_MEMORY;
        return OK;
    }
    init_numbers(1, a->base, &temp);
    if (a->sign != b->sign) {
        if (copy_number(&temp, a) != OK) {
            free_numbers(1, &temp);
            return OUT_OF_MEMORY;
        }
        temp.sign *= -1;
        if (compare(b, &temp) == 0) {
            answer->length = answer->sign = 0;
            free_numbers(1, &temp);
            return 0;
        }
        if (a->sign > 0) {
            if (copy_number(&temp, b) != OK) {
                free_numbers(1, &temp);
                return OUT_OF_MEMORY;
            }
            temp.sign *= -1;
            t = diff(a, &temp, answer);
            if (t != OK) {
                free_numbers(1, &temp);
                return t;
            }
        } else {
            t = diff(b, &temp, answer);
            if (t != OK) {
                free_numbers(1, &temp);
                return t;
            }
        }
        free_number(&temp);
        return OK;
    }
    temp.base = a->base;
    temp.sign = a->sign;
    for (i = 0; i < mlen; i++) {
        c = memory;
        if (i < a->length)
            c += a->data[i];
        if (i < b->length)
            c += b->data[i];
        if (add_to_number(&temp, c % answer->base) != OK) {
            free_number(&temp);
            return OUT_OF_MEMORY;
        }
        memory = c / answer->base;
    }
    if (memory)
        if (add_to_number(&temp, memory) != OK) {
            free_number(&temp);
            return OUT_OF_MEMORY;
        }
    if (copy_number(answer, &temp) != OK) {
        free_number(&temp);
        return OUT_OF_MEMORY;
    }
    free_number(&temp);
    return OK;
}

int divis(bigint* a, bigint* b, bigint* answer) {
    int i, j, t;
    bigint temp, ans, temp2;
    if (a->base != b->base) {
        return DIF_BASES;
    }
    if (b->sign == 0 || b->length == 0) {
        return DIV_ZERO;
    }
    if (a->sign == 0 || a->length == 0) {
        answer->sign = answer->length = 0;
        return OK;
    }
    init_numbers(3, a->base, &temp, &temp2, &ans);
    i = a->length;
    if (copy_number(&temp2, b) != OK) {
        free_numbers(3, &temp, &temp2, &ans);
        return OUT_OF_MEMORY;
    }
    temp.sign = temp2.sign = 1;
    while (i > 0 && compare(&temp, &temp2) < 0) {
        i--;
        reverse(&temp);
        if (add_to_number(&temp, a->data[i]) != OK) {
            free_numbers(3, &temp, &temp2, &ans);
            return OUT_OF_MEMORY;
        }
        reverse(&temp);
    }
    if (compare(&temp, &temp2) < 0)
        ans.sign = ans.length = 0;
    else if (a->sign != b->sign)
        ans.sign = -1;
    else
        ans.sign = 1;
    while (i >= 0) {
        for (j = 0; j < a->base && compare(&temp, &temp2) >= 0; j++) {
            t = diff(&temp, &temp2, &temp);
            if (t != OK) {
                free_numbers(3, &temp, &temp2, &ans);
                return OUT_OF_MEMORY;
            }
        }
        if (add_to_number(&ans, j) != OK) {
            free_numbers(3, &temp, &temp2, &ans);
            return OUT_OF_MEMORY;
        }
        t = mult_on_small(&temp, a->base, &temp);
        if (t != OK) {
            free_numbers(3, &temp, &temp2, &ans);
            return OUT_OF_MEMORY;
        }
        if (temp.length == 0 || temp.sign == 0) {
            temp.sign = temp.length = 1;
        }
        i--;
        if (i < 0)
            break;
        temp.data[0] = a->data[i];
    }
    reverse(&ans);
    if (a->sign != b->sign) {
        temp.sign = 1;
        temp.length = 0;
        if (add_to_number(&temp, 1) != OK) {
            free_numbers(3, &temp, &temp2, &ans);
            return OUT_OF_MEMORY;
        }
        t = diff(&ans, &temp, &ans);
        if (t != OK) {
            free_numbers(3, &temp, &temp2, &ans);
            return OUT_OF_MEMORY;
        }
    }
    if (copy_number(answer, &ans) != OK) {
        free_numbers(3, &temp, &temp2, &ans);
        return OUT_OF_MEMORY;
    }
    free_numbers(3, &ans, &temp, &temp2);
    return OK;
}

int mod(bigint* a, bigint* b, bigint* answer) {
    int t;
    bigint temp, temp2;
    if (a->base != b->base)
        return DIF_BASES;
    if (b->sign == 0 || b->length == 0)
        return DIV_ZERO;
    if (a->sign == 0) {
        answer->sign = answer->length = 0;
        return OK;
    }
    init_numbers(2, a->base, &temp, &temp2);
    if (a->sign != b->sign) {
        if (copy_number(&temp, a) != OK) {
            free_numbers(2, &temp, &temp2);
            return OUT_OF_MEMORY;
        }
        if (copy_number(&temp2, b) != OK) {
            free_numbers(2, &temp, &temp2);
            return OUT_OF_MEMORY;
        }
        temp.sign = temp2.sign = 1;
        t = mod(&temp, &temp2, &temp);
        if (t != OK) {
            free_numbers(2, &temp, &temp2);
            return t;
        }
        temp.sign = 1;
        t = diff(&temp2, &temp, &temp);
        if (t != OK) {
            free_numbers(2, &temp, &temp2);
            return t;
        }
        temp.sign = b->sign;
        if (copy_number(answer, &temp) != OK) {
            free_numbers(2, &temp, &temp2);
            return OUT_OF_MEMORY;
        }
        free_numbers(2, &temp, &temp2);
    } else {
        if (a->sign < 0) {
            if (copy_number(&temp, a) != OK) {
                free_numbers(2, &temp, &temp2);
                return OUT_OF_MEMORY;
            }
            if (copy_number(&temp2, b) != OK) {
                free_numbers(2, &temp, &temp2);
                return OUT_OF_MEMORY;
            }
            temp.sign = temp2.sign = 1;
            t = mod(&temp, &temp2, &temp);
            if (t != OK) {
                free_numbers(2, &temp, &temp2);
                return t;
            }
            temp.sign = -1;
            if (copy_number(answer, &temp) != OK) {
                free_numbers(2, &temp, &temp2);
                return OUT_OF_MEMORY;
            }
            free_numbers(2, &temp, &temp2);
        } else {
            t = divis(a, b, &temp);
            t = (t == OK) ? mult(&temp, b, &temp) : t;
            t = (t == OK) ? diff(a, &temp, &temp) : t;
            temp.sign = a->sign;
            if (copy_number(answer, &temp) != OK) {
                free_numbers(2, &temp, &temp2);
                return OUT_OF_MEMORY;
            }
            free_numbers(2, &temp, &temp2);
            return OK;
        }
    }
    return OK;
}

int _dec_to_256(bigint* a, bigint* answer) {
    int i, digit, t;
    bigint temp, _256, ans, temp2;
    if (a->base != 10)
        return DIF_BASES;
    init_numbers(3, 10, &temp, &temp2, &_256);
    init_numbers(1, 256, &ans);
    temp.sign = temp2.sign = _256.sign = 1;
    if (add_to_number(&_256, (small)6) != OK) {
        free_numbers(4, &ans, &temp, &temp2, &_256);
        return OUT_OF_MEMORY;
    }
    if (add_to_number(&_256, (small)5) != OK) {
        free_numbers(4, &ans, &temp, &temp2, &_256);
        return OUT_OF_MEMORY;
    }
    if (add_to_number(&_256, (small)2) != OK) {
        free_numbers(4, &ans, &temp, &temp2, &_256);
        return OUT_OF_MEMORY;
    }
    if (copy_number(&temp, a) != OK) {
        free_numbers(4, &ans, &temp, &temp2, &_256);
        return OUT_OF_MEMORY;
    }
    while (temp.sign && temp.length) {
        t = mod(&temp, &_256, &temp2);
        t = (t == OK) ? divis(&temp, &_256, &temp) : t;
        if (t != OK) {
            free_numbers(4, &ans, &temp, &temp2, &_256);
            return OUT_OF_MEMORY;
        }
        digit = 0;
        for (i = temp2.length - 1; i >= 0; i--)
            digit = digit * 10 + temp2.data[i];
        if (add_to_number(&ans, (small)digit) != OK) {
            free_numbers(4, &ans, &temp, &temp2, &_256);
            return OUT_OF_MEMORY;
        }
    }
    ans.sign = a->sign;
    if (copy_number(answer, &ans) != OK) {
        free_numbers(4, &ans, &temp, &temp2, &_256);
        return OUT_OF_MEMORY;
    }
    free_numbers(4, &_256, &ans, &temp, &temp2);
    return OK;
}

int _256_to_dec(bigint* a, bigint* answer) {
    int i, digit, t;
    small k;
    bigint ans, temp2, _256;
    if (a->base != 256)
        return DIF_BASES;
    init_numbers(3, 10, &_256, &ans, &temp2);
    ans.sign = _256.sign = temp2.sign = 1;
    t = add_to_number(&_256, 6);
    t = (t == OK) ? add_to_number(&_256, 5) : t;
    t = (t == OK) ? add_to_number(&_256, 2) : t;
    if (t != OK) {
        free_numbers(3, &ans, &_256, &temp2);
        return OUT_OF_MEMORY;
    }
    for (i = a->length - 1; i >= 0; i--) {
        t = mult(&ans, &_256, &ans);
        if (t != OK) {
            free_numbers(3, &ans, &_256, &temp2);
            return OUT_OF_MEMORY;
        }
        temp2.length = 0;
        k = a->data[i];
        if (k > 99) {
            t = add_to_number(&temp2, (k % 100) % 10);
            t = (t == OK) ? add_to_number(&temp2, (k % 100) / 10) : t;
            t = (t == OK) ? add_to_number(&temp2, k / 100) : t;
        } else {
            if (k > 9) {
                t = (t == OK) ? add_to_number(&temp2, k % 10) : t;
                t = (t == OK) ? add_to_number(&temp2, k / 10) : t;
            } else {
                t = (t == OK) ? add_to_number(&temp2, k) : t;
            }
        }
        t = (t == OK) ? sum(&ans, &temp2, &ans) : t;
        if (t != OK) {
            free_numbers(3, &ans, &_256, &temp2);
            return OUT_OF_MEMORY;
        }
    }
    ans.sign = a->sign;
    if (copy_number(answer, &ans) != OK) {
        free_numbers(3, &ans, &_256, &temp2);
        return OUT_OF_MEMORY;
    }
    free_numbers(3, &ans, &_256, &temp2);
    return OK;
}
