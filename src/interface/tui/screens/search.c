#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "typedefs.h"

#include "interface/tui/consts.h"
#include "interface/tui/typedefs.h"

/**
 * @brief Displays the search button on the TUI screen.
 */
void show_search_button(pos_t start_y, Dimension *scr_dim, bool selected)
{
    // Stores the color pair for displaying the text on the screen.
    integer_t color = selected ? COLOR_SELECT : COLOR_DEFAULT;

    attron(COLOR_PAIR(color));

    move(start_y, (scr_dim->width - strlen(search_button_text) - 2) / 2);
    printw(" %s ", search_button_text);

    attron(COLOR_PAIR(color));
    refresh();
}
