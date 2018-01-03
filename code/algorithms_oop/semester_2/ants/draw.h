#pragma once

#include <locale.h>
#include <ncurses.h>
#include <wchar.h>

static const wchar_t* EMPTY_STR  = L" ";
static const wchar_t* FOOD_STR   = L"\x2663";
static const wchar_t* GROUND_STR = L"\x2592\x2592\x2592";
static const wchar_t* ANT_STR    = L"\x263b";
static const wchar_t* QUEEN_STR  = L"\x2665";

typedef enum {
    RED_COLOR     = COLOR_PAIR(1),
    YELLOW_COLOR  = COLOR_PAIR(2),
    GREEN_COLOR   = COLOR_PAIR(3),
    WHITE_COLOR   = COLOR_PAIR(4),
    MAGENTA_COLOR = COLOR_PAIR(5)
} Color;

const Color CELL_COLOR          = GREEN_COLOR;
const Color ANT_COLOR           = YELLOW_COLOR;
const Color QUEEN_COLOR         = RED_COLOR;
const Color ANT_WITH_MEAL_COLOR = MAGENTA_COLOR;
const Color BUILDER_COLOR       = YELLOW_COLOR;
const Color ROAD_COLOR          = WHITE_COLOR;

extern WINDOW* window;

extern void put_str(size_t i, size_t j, const wchar_t* output_str, Color color);
extern void init_stdio();
