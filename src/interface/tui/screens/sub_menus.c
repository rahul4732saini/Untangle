#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

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
 * @brief Draws the static contents of the sub-menu TUI screen.
 * @param scr_dim Pointer to the Dimension struct comprising the
 * screen dimensions.
 */
void show_sub_menu_screen(const char *title, Dimension *scr_dim)
{
    clear();

    show_title_bar(title, scr_dim);
    show_go_back_button(false);
}

/**
 * @brief Initializes and configures the search box window.
 */
void init_search_window(WinContext *wctx, Dimension *scr_dim)
{
    Dimension *dim = wctx->dim;

    *dim = (Dimension){
        .height = search_box_height,
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
    wmove(win, 1, search_box_padding + 1);
    printw("%*s", search_box_width - search_box_padding * 2 - 2, "");

    wmove(win, 1, 2);

    // Always prints the specified text is not NULL.
    if (text)
    {
        len_t len = strlen(text);

        if (len < search_box_width - search_box_padding * 2 - 3)
            waddstr(win, text);

        // Only prints the last characters from the string that can fit in the search
        // box along with one additional character space for the cursor if its length
        // exceeds the limit.
        waddstr(win, text + len - search_box_width + search_box_padding * 2 + 3);
    }

    // Displays the placeholder when the box is not selected and there is
    // no other text to display.
    else if (!selected)
        waddstr(win, search_box_placeholder);

    wattroff(win, COLOR_PAIR(color));
    wrefresh(win);
}

/**
 * @brief Initializes and configures the generic sub-menu window.
 */
void init_sub_menu_window(WinContext *wctx, Dimension *scr_dim)
{
    Dimension *dim = wctx->dim;

    *dim = (Dimension){
        .height = scr_dim->height - sub_menu_start_y + 1,
        .width = sub_menu_width,
        .start_y = sub_menu_start_y,
        .start_x = (scr_dim->width - sub_menu_width) / 2,
    };

    wctx->win = init_window(dim);
    box(wctx->win, 0, 0);

    wrefresh(wctx->win);
}

/**
 * @brief Extracts the name of the domain at the specified index in
 * the given domains.
 *
 * @param domains Pointer to the Domains struct comprising the domains.
 * @param inx Index of the domain whose name is to be extracted.
 */
static const char *get_domain_name(void *domains, index_t inx)
{
    return ((Domains *)domains)->domains[inx]->name;
}

/**
 * @brief Extracts the name of the field at the specified index in
 * the given domain.
 *
 * @param domains Pointer to the Domain struct comprising the fields.
 * @param inx Index of the field whose name is to be extracted.
 */
static const char *get_field_name(void *domain, index_t inx)
{
    return ((Domain *)domain)->fields[inx]->name;
}

/**
 * @brief Extracts the name of the problem at the specified index in
 * the given field.
 *
 * @param domains Pointer to the Field struct comprising the problems.
 * @param inx Index of the problem whose name is to be extracted.
 */
static const char *get_problem_name(void *field, index_t inx)
{
    return ((Field *)field)->problems[inx]->name;
}

/**
 * @brief Displays the sub-menu window along with the specified data.
 *
 * @details Displays the sub-menu window along with the data accepted as a generic
 * pointer, to allow different types of data to be specified and displayed, each entry
 * of which is accessed using the specified getter/resolver function.
 *
 * @param wctx Pointer to the WinContext struct comprising the window data.
 * @param data Generic pointer to the data to be displayed in the sub-menu.
 * @param data_size Size of the data array.
 * @param select Index of the data entry to be highlighted in the sub-menu.
 * @param getter Pointer to the function to get the name associated with each data entry.
 */
static void show_sub_menu_window(
    WinContext *wctx,
    void *data,
    len_t data_size,
    index_t select,
    const char *(*getter)(void *, index_t))
{
    WINDOW *win = wctx->win;

    // Excludes the vertical border of the window to get the option limit.
    len_t option_limit = wctx->dim->height - 2;

    // Excludes the side borders along with a padding of 1 character to get
    // the horizontal limit.
    len_t row_limit = wctx->dim->width - 4;

    for (index_t i = 0; i < option_limit && i < data_size; ++i)
    {
        if (i == select)
            wattron(win, COLOR_PAIR(COLOR_SELECT));

        // Starts with a padding of 2 characters in the row (1 for border, 1 for padding).
        wmove(win, i + 1, 2);
        waddnstr(win, getter(data, i), row_limit);

        if (i == select)
        {
            // Fills up the remaining space in the row highlighting the selection.
            wprintw(win, "%*s", row_limit - getcurx(win) + 2, "");
            wattroff(win, COLOR_PAIR(COLOR_SELECT));
        }
    }

    wrefresh(win);
}

/**
 * @brief Displays the domain sub-menu window.
 */
void show_domain_menu_window(WinContext *wctx, Domains *domains, index_t select)
{
    show_sub_menu_window(wctx, domains, domains->domains_size, select, get_domain_name);
}

/**
 * @brief Displays the field sub-menu window.
 */
void show_field_menu_window(WinContext *wctx, Domain *domain, index_t select)
{
    show_sub_menu_window(wctx, domain, domain->fields_size, select, get_field_name);
}

/**
 * @brief Displays the problem sub-menu window.
 */
void show_problem_menu_window(WinContext *wctx, Field *field, index_t select)
{
    show_sub_menu_window(wctx, field, field->problems_size, select, get_problem_name);
}
