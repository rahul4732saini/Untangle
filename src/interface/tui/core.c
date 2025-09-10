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
    handle_field_menu,
};

/**
 * @brief Initializes and configures the standard TUI screen.
 */
static void setup(void)
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
static void clean(void)
{
    endwin();
}

/**
 * @brief Updates the screen dimensions and waits if they are un-supported.
 * @param scr_dim Pointer to the Dimension struct comprising the
 * screen dimensions.
 */
static void handle_screen_dimensions(Dimension *scr_dim)
{
    // Updates the struct with the new screen dimensions.
    *scr_dim = (Dimension){
        .height = getmaxy(stdscr),
        .width = getmaxx(stdscr),
    };

    // Displays a warning message if the screen dimensions are unsupported.
    if (scr_dim->height < MIN_HEIGHT || scr_dim->width < MIN_WIDTH)
    {
        clear();

        mvaddstr(0, 0, scr_dim_warning);
        refresh();

        // Waits until the screen dimensions are changed.
        while (getch() != KEY_RESIZE)
            ;
    }
}

/**
 * @brief Sets up and handles the TUI environment.
 */
void mainloop(SessionData *sdata)
{
    setup();

    Dimension scr_dim;
    handler_t handle_stack[screen_stack_size];
    void *data_stack[screen_stack_size];

    index_t len = 0;  // Length of the stacks
    handler_t handle; // Temporarily stores the identifier return by the handler.

    // Registers main-menu data onto the stack for execution.
    handle_stack[len] = HDL_MAIN_MENU;
    data_stack[len++] = sdata;

    while (true)
    {
        handle_screen_dimensions(&scr_dim);

        // Copies the current interface data to the next frame, and passes a pointer to
        // that to avoid any changes in the original while also automatically placing the
        // next interface data onto the stack.
        data_stack[len] = data_stack[len - 1];
        handle = handlers[handle_stack[len - 1]](&scr_dim, data_stack + len);

        if (handle == HDL_SELF)
            continue;

        else if (handle == HDL_PREV)
        {
            // Exits if there are no more interface entries on the stack.
            if (len == 1)
                break;

            --len;
            continue;
        }

        handle_stack[len++] = handle;
    }

    clean();
}
