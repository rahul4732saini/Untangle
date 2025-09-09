/**
 * @file handlers.c
 * @brief Defines screen handlers functions for managing the TUI environment.
 */

#include <ncurses.h>

#include "consts.h"
#include "shared.h"
#include "typedefs.h"

#include "interface/tui/typedefs.h"
#include "interface/tui/consts.h"
#include "interface/tui/handlers.h"

#include "interface/tui/screens/main_menu.h"

// Screen handlers associated with the main menu options.
static const handler_t main_menu_handlers[] = {
    HDL_DOMAIN_MENU,
    HDL_EXTENSION_MENU,
    HDL_PREV,
};

/**
 * @brief Handles the main menu interface.
 *
 * @details Displays the main menu window and handles user input for
 * triggering the actions associated with the main menu buttons.
 */
handler_t handle_main_menu(Dimension *scr_dim, void **data)
{
    SessionData *sdata = (SessionData *)*data;

    WinContext wctx;
    Dimension dim;

    wctx.dim = &dim;
    init_main_menu(&wctx, scr_dim);

    input_t input = 0;
    index_t select = 0;

    // Displays the main menu window until the RETURN key is pressed
    // signifying a menu button press.
    do
    {
        switch (input)
        {
        case KEY_UP:
            --select;
            break;

        case KEY_DOWN:
            ++select;
            break;

        case KEY_RESIZE:
            return HDL_SELF;
        }

        // Updates the selection and displays the menu.
        select = (select + main_menu_items_len) % main_menu_items_len;
        show_main_menu(&wctx, select);

    } while ((input = getch()) != ASCII_LF);

    if (!select)
        *data = sdata->domains;

    // Ignores the case where the exit button is pressed as the data
    // passed is ignored.
    else
        *data = sdata->plugins;

    return main_menu_handlers[select];
}
