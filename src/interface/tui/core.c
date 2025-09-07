#include <ncurses.h>

/**
 * @brief Initializes and configures the standard TUI screen.
 */
void setup(void)
{
    initscr();

    noecho();
    cbreak();
    start_color();

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
