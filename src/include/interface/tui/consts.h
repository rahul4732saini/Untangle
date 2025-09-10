#ifndef _TUI_CONSTS_H
#define _TUI_CONSTS_H

#include "typedefs.h"
#include "interface/tui/typedefs.h"

// ncurses TUI color pair identifiers.
#define COLOR_DEFAULT 0
#define COLOR_SELECT 1
#define COLOR_DISABLED 2

// Extended ncurses color index
#define COLOR_GREY 8

// ASCII code associated with the corresponding keyboard keys.
#define ASCII_LF 10
#define ASCII_ESC 27

// Minimum screen dimensions to be supported to run in the TUI environment.
#define MIN_HEIGHT 80
#define MIN_WIDTH 25

extern const len_t screen_stack_size;

extern const span_t title_bar_height;
extern const span_t go_back_button_height;
extern const span_t search_box_height;

extern const span_t main_menu_width;
extern const span_t search_box_width;
extern const span_t sub_menu_width;

extern const span_t search_box_padding;

extern const char *go_back_button_text;

extern const pos_t go_back_button_start_y;
extern const pos_t go_back_button_start_x;
extern const pos_t search_box_start_y;
extern const pos_t sub_menu_start_y;

#endif
