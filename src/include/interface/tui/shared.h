#ifndef _TUI_SHARED_H
#define _TUI_SHARED_H

#include <ncurses.h>
#include "interface/tui/typedefs.h"

extern WINDOW *init_window(Dimension *dim);
extern void show_title_bar(const char *heading, Dimension *scr_dim);

#endif
