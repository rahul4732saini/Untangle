#include <ncurses.h>
#include <string.h>

#include "typedefs.h"
#include "interface/tui/typedefs.h"

/**
 * @brief Initializes and configures a new ncurses TUI window.
 * @param dim Pointer to the Dimension struct comprising the window dimensions.
 * @return Pointer to the newly created window.
 */
WINDOW *init_window(Dimension *dim)
{
    WINDOW *win = newwin(
        dim->height,
        dim->width,
        dim->start_y,
        dim->start_x);

    keypad(win, TRUE);
    refresh();

    return win;
}

/**
 * @brief Displays the title bar with the specified heading at the
 * top of the screen.
 *
 * @param heading The string to be displayed as the title.
 * @param scr_dim Pointer to the Dimension struct comprising the
 * screen dimensions.
 */
void show_title_bar(char *heading, Dimension *scr_dim)
{
    move(1, 0);

    // Adds a line to add a visual separation between the title bar and
    // the remaining screen.
    for (pos_t i = 0; i < scr_dim->width; ++i)
        addch(ACS_HLINE);

    len_t heading_len = strlen(heading);

    // Displays the full title if it can be fit on the screen.
    if (heading_len <= scr_dim->width - 2)
    {
        move(0, (scr_dim->width - strlen(heading) - 1) / 2);
        addstr(heading);

        return;
    }

    // Displays the initial part of the heading and appends it with ellipsis
    // to indicate incompleteness.
    move(0, 1);
    addnstr(heading, scr_dim->width - 5);
    addstr("...");

    refresh();
}
