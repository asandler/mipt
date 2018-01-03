#include "draw.h"

WINDOW* window;

void put_str(size_t i, size_t j, const wchar_t* output_str, Color color) {
    attron(color);
    mvaddwstr(i, j, output_str);
}

void init_stdio() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();

    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_WHITE, -1);
    init_pair(5, COLOR_MAGENTA, -1);

    cbreak();
    window = newwin(0, 0, 0, 0);
    refresh();
    wrefresh(window);
} 
