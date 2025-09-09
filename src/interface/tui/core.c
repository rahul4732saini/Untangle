#include <ncurses.h>

#include "typedefs.h"
#include "shared.h"

#include "interface/tui/typedefs.h"
#include "interface/tui/consts.h"
#include "interface/tui/handlers.h"

// Pointers to TUI screen handler functions associated with each screen identifier.
static const handler_t (*handlers[])(Dimension *, void **) = {
    handle_main_menu,
    handle_domain_menu,
};

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
    init_pair(COLOR_DISABLED, COLOR_GREY, COLOR_BLACK);
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
