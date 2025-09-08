#include <ncurses.h>
#include <stdbool.h>

#include "typedefs.h"

#include "interface/tui/consts.h"
#include "interface/tui/typedefs.h"

/**
 * @brief Displays the 'Go Back' button on the TUI screen.
 *
 * @details Displays the button at a pre-defined location on the screen
 * while also highlighting it if the corresponding argument is specified.
 *
 * @param selected Whether to highlight the button.
 */
void show_go_back_button(bool selected)
{
    integer_t color = selected ? COLOR_SELECT : COLOR_DEFAULT;

    attron(COLOR_PAIR(color));

    move(go_back_button_start_y, go_back_button_start_x);
    printw(" %s ", go_back_button_text);

    attroff(COLOR_PAIR(color));
    refresh();
}
