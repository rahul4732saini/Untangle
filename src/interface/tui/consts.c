#include "interface/tui/typedefs.h"

const span_t title_bar_height = 2;

// The following window widths also include the side borders.
const span_t main_menu_width = 24;
const span_t search_box_width = 60;

const char *go_back_button_text = "Go Back [Esc]";

// Starting coordinates of different components on the TUI screen.
const pos_t go_back_button_start_y = title_bar_height;
const pos_t go_back_button_start_x = 1;
const pos_t search_box_start_y = go_back_button_start_y + 1;

const char *domain_menu_title = "Select a Domain";