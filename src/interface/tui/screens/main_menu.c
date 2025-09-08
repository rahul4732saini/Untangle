#include <ncurses.h>
#include <string.h>

#include "consts.h"
#include "typedefs.h"

#include "interface/tui/typedefs.h"
#include "interface/tui/shared.h"
#include "interface/tui/consts.h"

/**
 * @brief Displays the footer strings comprising general details about
 * the software.
 *
 * @param scr_dim Pointer to the Dimension struct comprising the
 * screen dimensions.
 */
static void show_footer(Dimension *scr_dim)
{
    index_t row = scr_dim->height - footers_len - 1;

    for (index_t i = 0; i < footers_len; ++i, ++row)
    {
        move(row, (scr_dim->width - strlen(footers[i])) / 2);
        addstr(footers[i]);
    }

    refresh();
}

/**
 * @brief Displays the specified menu heading on top on the
 * associated menu window.
 *
 * @param heading Text to be displayed on top on the menu window.
 * @param menu_start_y Vertical starting position of the menu.
 * @param scr_dim Pointer to the Dimension struct comprising the
 * screen dimensions.
 */
static inline void show_menu_heading(
    const char *heading, size_t menu_start_y, Dimension *scr_dim)
{
    move(menu_start_y - 1, (scr_dim->width - strlen(heading)) / 2);
    addstr(heading);
}

/**
 * @brief Initializes the main menu window and displays its static
 * contents on the TUI screen.
 *
 * @param wctx Pointer to the WinContext struct comprising the window data.
 * @param scr_dim Pointer to the Dimension struct comprising the
 * screen dimensions.
 */
void init_main_menu(WinContext *wctx, Dimension *scr_dim)
{
    Dimension *dim = wctx->dim;

    dim->height = main_menu_items_len + 2;
    dim->width = main_menu_width;

    dim->start_y = (scr_dim->height - dim->height + title_bar_height) / 2;
    dim->start_x = (scr_dim->width - dim->width) / 2;

    wctx->win = init_window(dim);
    box(wctx->win, 0, 0);

    // Draws the static content on the screen.
    show_footer(scr_dim);
    show_menu_heading(main_menu_title, dim->start_y, scr_dim);
    show_title_bar(main_menu_top_bar_title, scr_dim);

    wrefresh(wctx->win);
}

/**
 * @brief Displays the main menu window on the TUI screen.
 *
 * @details Displays the main menu window with the menu items
 * also highlighting the selected item marking the selection.
 *
 * @param wctx Pointer to the WinContext struct comprising the window data.
 * @param select Index of the selected menu item.
 */
void show_main_menu(WinContext *wctx, index_t select)
{
    WINDOW *win = wctx->win;

    // Left cutoff is stored as a 32-bit signed integer to meet
    // padding variable requirements in variadic formatting.

    span_t left_cutoff;
    span_t length, width = main_menu_width - 2;

    for (index_t i = 0; i < main_menu_items_len; ++i)
    {
        // Extracts the string length for cutoff calculation.
        length = strlen(main_menu_items[i]);
        left_cutoff = (width - length) / 2;

        // Configures the current background and foreground
        // color based on the index of the menu item.

        if (i == select)
            wattron(win, COLOR_PAIR(COLOR_SELECT));

        wmove(win, i + 1, 1);

        wprintw(win, "%*s", left_cutoff, "");
        wprintw(win, "%s", main_menu_items[i]);
        wprintw(win, "%*s", width - length - left_cutoff, "");

        if (i == select)
            wattroff(win, COLOR_PAIR(COLOR_SELECT));
    }

    wrefresh(win);
}
