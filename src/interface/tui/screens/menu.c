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

    dim->start_y = (scr_dim->height - dim->height) / 2;
    dim->start_x = (scr_dim->width - dim->width) / 2;

    wctx->win = init_window(dim);

    // Draws the static content on the screen.
    box(wctx->win, 0, 0);
    show_footer(scr_dim);
    show_title_bar(main_menu_top_bar_title, scr_dim);

    wrefresh(wctx->win);
}
