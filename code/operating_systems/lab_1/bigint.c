#include "bigint.h"

int main() {
        bigint t, s;
        int r;
        init_numbers(2, 10, &t, &s);
        read_number(&s);
        read_number(&t);
        mod(&s, &t, &s);
        print_number(&s);
        free_numbers(2, &t, &s);
        return 0;
}
