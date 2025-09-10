/**
 * @file handlers.c
 * @brief Defines screen handlers functions for managing the TUI environment.
 */

#include <ncurses.h>

#include "consts.h"
#include "shared.h"
#include "typedefs.h"
#include "structs.h"

#include "interface/tui/typedefs.h"
#include "interface/tui/consts.h"
#include "interface/tui/handlers.h"

#include "interface/tui/screens/main_menu.h"
#include "interface/tui/screens/sub_menus.h"

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

    setup_main_menu_screen(scr_dim);
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

        case ASCII_ESC:
            return HDL_PREV;
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

/**
 * @brief Handles the domain menu interface.
 *
 * @details Displays the domain menu window and handles user input for
 * triggering the actions associated with them within the menu screen.
 */
handler_t handle_domain_menu(Dimension *scr_dim, void **data)
{
    // Creates a separate copy to be used for passing data to display functions, to
    // only provide them with the domains available in the current segment while also
    // avoiding mutations in the original data.
    Domains *domains = (Domains *)*data, domains_cpy;

    WinContext win_ctxs[2];
    Dimension dims[2];

    win_ctxs[0].dim = dims, win_ctxs[1].dim = dims + 1;

    // The title is limited to the screen width as exceeding that would
    // only hide it visually.
    char title[scr_dim->width];
    snprintf(title, scr_dim->width, "%s > %s", main_menu_window_title, domain_menu_title);

    // Sets up the sub-menu screen and initializes the windows.
    setup_sub_menu_screen(title, win_ctxs, scr_dim);

    // Excludes the vertical window borders to get the option count limit.
    len_t option_limit = win_ctxs[0].dim->height - 2;

    input_t input = 0;
    index_t segment = 0, offset = 0;

    // Displays the domain menu window until the RETURN key is pressed
    // signifying an option selection.
    do
    {
        switch (input)
        {
        case KEY_UP:
            if (offset)
                --offset;

            else if (segment)
                --segment;

            break;

        case KEY_DOWN:
            if (segment + offset == domains->domains_size - 1)
                ;

            else if (offset != option_limit - 1)
                ++offset;

            else
                ++segment;

            break;

        case KEY_RESIZE:
            return HDL_SELF;

        case ASCII_ESC:
            return HDL_PREV;
        };

        // Updates the copy based on the current segment to correctly display
        // the domain menu items.
        domains_cpy = (Domains){
            .domains = domains->domains + segment,
            .domains_size = domains->domains_size - segment,
        };

        show_domain_menu_window(win_ctxs, &domains_cpy, offset);

    } while ((input = getch()) != ASCII_LF);

    // Access the selected domain directly using the offset from the copy struct.
    *data = domains_cpy.domains[offset];

    return HDL_FIELD_MENU;
}

/**
 * @brief Handles the field menu interface.
 *
 * @details Displays the field menu window and handles user input for
 * triggering the actions associated with them within the menu screen.
 */
handler_t handle_field_menu(Dimension *scr_dim, void **data)
{
    // Creates a separate copy to be used for passing data to display functions, to
    // only provide them with the fields available in the current segment while also
    // avoiding mutations in the original data.
    Domain *domain = (Domain *)*data, domain_cpy;

    WinContext win_ctxs[2];
    Dimension dims[2];

    win_ctxs[0].dim = dims, win_ctxs[1].dim = dims + 1;

    // The title is limited to the screen width as exceeding that would
    // only hide it visually.
    char title[scr_dim->width];
    snprintf(title, scr_dim->width, ".. > %s > %s", domain->name, field_menu_title);

    // Sets up the sub-menu screen and initializes the windows.
    setup_sub_menu_screen(title, win_ctxs, scr_dim);

    // Excludes the vertical window borders to get the option count limit.
    len_t option_limit = win_ctxs[0].dim->height - 2;

    input_t input = 0;
    index_t segment = 0, offset = 0;

    // Displays the field menu window until the RETURN key is pressed
    // signifying an option selection.
    do
    {
        switch (input)
        {
        case KEY_UP:
            if (offset)
                --offset;

            else if (segment)
                --segment;

            break;

        case KEY_DOWN:
            if (segment + offset == domain->fields_size - 1)
                ;

            else if (offset != option_limit - 1)
                ++offset;

            else
                ++segment;

            break;

        case KEY_RESIZE:
            return HDL_SELF;

        case ASCII_ESC:
            return HDL_PREV;
        };

        // Updates the copy based on the current segment to correctly display
        // the field menu items.
        domain_cpy = (Domain){
            .fields = domain->fields + segment,
            .fields_size = domain->fields_size - segment,
        };

        show_field_menu_window(win_ctxs, &domain_cpy, offset);

    } while ((input = getch()) != ASCII_LF);

    // Access the selected domain directly using the offset from the copy struct.
    *data = domain_cpy.fields[offset];

    return HDL_PROBLEM_MENU;
}
