#include <ncurses.h>
#include "interface/tui/consts.h"

/**
 * @brief Initializes and configures the standard TUI screen.
 */
void setup(void)
{
    initscr();

    noecho();
    cbreak();
    start_color();

    // Color pairs initialization.
    init_pair(COLOR_SELECT, COLOR_BLACK, COLOR_WHITE);

    // Reduced ESC key delay to 100ms to avoid performance issues.
    ESCDELAY = 100;

    curs_set(0);
    keypad(stdscr, TRUE);
}

/**
 * @brief Fress the resources and exits the TUI environment.
 */
void clean(void)
{
    endwin();
}
