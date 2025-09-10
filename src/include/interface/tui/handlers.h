#ifndef _TUI_HANDLERS_H
#define _TUI_HANDLERS_H

#include "interface/tui/typedefs.h"

// TUI screen handler identifiers
#define HDL_PREV -2
#define HDL_SELF -1
#define HDL_MAIN_MENU 0
#define HDL_DOMAIN_MENU 1
#define HDL_FIELD_MENU 2
#define HDL_PROBLEM_MENU 3
#define HDL_EXTENSION_MENU 4
#define HDL_PROBLEM_SOLVER 5

handler_t handle_main_menu(Dimension *scr_dim, void **data);
handler_t handle_domain_menu(Dimension *scr_dim, void **data);
handler_t handle_field_menu(Dimension *scr_dim, void **data);
handler_t handle_problem_menu(Dimension *scr_dim, void **data);

#endif
