#include "ants.h"

#include <ctime>
#include <unistd.h>

int main() {
    srand((unsigned int) time(0));
    init_stdio();

    Field& F = Field::Instance();
    F.initialize(16, 32);

    do {
        F.redraw();
        F.pause();
        F.newDay();
    } while (!F.end());

    getch();
    endwin();

    return 0;
}
