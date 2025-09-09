#ifndef _TUI_SCR_SUB_MENUS_H
#define _TUI_SCR_SUB_MENUS_H

#include <stdbool.h>

#include "shared.h"
#include "typedefs.h"
#include "structs.h"

#include "interface/tui/typedefs.h"

extern void show_go_back_button(bool selected);
extern void show_search_window(WinContext *wctx, char *text, bool selected);

extern void setup_sub_menu_screen(const char *title, Dimension *scr_dim);

extern void init_search_window(WinContext *wctx, Dimension *scr_dim);
extern void init_sub_menu_window(WinContext *wctx, Dimension *scr_dim);

extern void show_domain_menu_window(WinContext *wctx, Domains *domains, index_t select);
extern void show_field_menu_window(WinContext *wctx, Domain *domain, index_t select);
extern void show_problem_menu_window(WinContext *wctx, Field *domain, index_t select);
extern void show_extension_menu_window(WinContext *wctx, PluginsData *plugins, index_t select);

#endif
