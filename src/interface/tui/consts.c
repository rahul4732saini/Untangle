#include "typedefs.h"
#include "interface/tui/typedefs.h"

// Size of the stack to store the data associated with each TUI
// screen interface.
const len_t screen_stack_size = 30;

// Warning message to be displayed on the screen if the screen
// dimensions are too small to render the interface.
const char *scr_dim_warning = "Current screen dimensions are unsupported.\n\
Please resize the window to a larger size to continue.";

// Heights of different components on the TUI screen.
const span_t title_bar_height = 2;
const span_t go_back_button_height = 1;
const span_t search_box_height = 3;

// The following window widths also include the side borders.
const span_t main_menu_width = 24;
const span_t search_box_width = 50;
const span_t sub_menu_width = 60;

const span_t search_box_padding = 1;

const char *go_back_button_text = "Go Back [Esc]";

// Starting coordinates of different components on the TUI screen.
const pos_t go_back_button_start_y = title_bar_height;
const pos_t go_back_button_start_x = 1;
const pos_t search_box_start_y = go_back_button_start_y + go_back_button_height;
const pos_t sub_menu_start_y = search_box_start_y + search_box_height;
