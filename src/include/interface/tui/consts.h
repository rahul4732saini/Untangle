#ifndef _TUI_CONSTS_H
#define _TUI_CONSTS_H

#include "interface/tui/typedefs.h"

// ncurses TUI color pair identifiers.
#define COLOR_DEFAULT 0
#define COLOR_SELECT 1

extern const span_t title_bar_height;
extern const span_t main_menu_width;

extern const char *go_back_button_text;

extern const pos_t go_back_button_start_y;
extern const pos_t go_back_button_start_x;

extern const char *domain_menu_title;

#endif
