#ifndef _TUI_MENU_H
#define _TUI_MENU_H

#include "typedefs.h"
#include "interface/tui/typedefs.h"

extern void setup_main_menu_screen(Dimension *scr_dim);
extern void init_main_menu(WinContext *wctx, Dimension *scr_dim);
extern void show_main_menu(WinContext *wctx, index_t select);

#endif
