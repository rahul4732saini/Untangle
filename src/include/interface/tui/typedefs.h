#ifndef _TUI_STRUCTS_H
#define _TUI_STRUCTS_H

#include <ncurses.h>
#include <stdint.h>

typedef uint16_t pos_t;
typedef uint16_t span_t;

typedef struct
{
    span_t height;
    span_t width;
    pos_t start_y;
    pos_t start_x;
} Dimension;

typedef struct
{
    WINDOW *win;
    Dimension *dim;
} WinContext;

#endif
