#include "interface/tui/typedefs.h"

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
const char *search_box_placeholder = "Search...";

// Starting coordinates of different components on the TUI screen.
const pos_t go_back_button_start_y = title_bar_height;
const pos_t go_back_button_start_x = 1;
const pos_t search_box_start_y = go_back_button_start_y + go_back_button_height;
const pos_t sub_menu_start_y = search_box_start_y + search_box_height;

const char *domain_menu_title = "Select a Domain";
