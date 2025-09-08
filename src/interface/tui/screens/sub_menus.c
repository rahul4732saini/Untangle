#include <ncurses.h>
#include <stdbool.h>

#include "typedefs.h"

#include "interface/tui/consts.h"
#include "interface/tui/typedefs.h"
#include "interface/tui/shared.h"

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

/**
 * @brief Draws the static content of the Domain Menu TUI screen.
 * @param scr_dim Pointer to the Dimension struct comprising the
 * screen dimensions.
 */
void show_domain_menu_screen(Dimension *scr_dim)
{
    clear();

    show_title_bar(domain_menu_title, scr_dim);
    show_go_back_button(false);
}

/**
 * @brief Initializes and configures the search box window.
 */
void init_search_window(WinContext *wctx, Dimension *scr_dim)
{
    Dimension *dim = wctx->dim;

    *dim = (Dimension){
        .height = 3,
        .width = search_box_width,
        .start_y = search_box_start_y,
        .start_x = (scr_dim->width - search_box_width) / 2,
    };

    wctx->win = init_window(dim);
    box(wctx->win, 0, 0);

    wrefresh(wctx->win);
}

/**
 * @brief Displays the search box along with the specified text if not NULL.
 */
void show_search_window(WinContext *wctx, char *text, bool selected)
{
    WINDOW *win = wctx->win;
    integer_t color = selected ? COLOR_DEFAULT : COLOR_DISABLED;

    wattron(win, COLOR_PAIR(color));

    // Clears the search box of any remaining characters.
    wmove(win, 1, 1);
    printw("%*s", search_box_width - 2, "");

    wmove(win, 1, 1);

    // Always prints the specified text is not NULL.
    if (text)
    {
        len_t len = strlen(text);

        // Only prints the last characters from the string that can
        // fit in the search box if its length exceeds the limit.
        if (len < search_box_width - 3)
            waddstr(win, text);

        waddstr(win, text + len - search_box_width + 3);
    }

    // Displays the placeholder when the box is not selected and there is
    // no other text to display.
    else if (!selected)
        waddstr(win, search_box_placeholder);

    wattroff(win, COLOR_PAIR(color));
    wrefresh(win);
}
