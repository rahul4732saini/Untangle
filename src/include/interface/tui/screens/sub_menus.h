#ifndef _TUI_SCR_SUB_MENUS_H
#define _TUI_SCR_SUB_MENUS_H

#include <stdbool.h>
#include "interface/tui/typedefs.h"

extern void show_go_back_button(bool selected);
extern void show_sub_menu_screen(char *title, Dimension *scr_dim);
extern void init_search_window(WinContext *wctx, Dimension *scr_dim);
extern void show_search_window(WinContext *wctx, char *text, bool selected);
extern void init_sub_menu_window(WinContext *wctx, Dimension *scr_dim);

#endif
