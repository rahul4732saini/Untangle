#include <ncurses.h>
#include <string.h>

#include "typedefs.h"

#include "interface/tui/consts.h"
#include "interface/tui/typedefs.h"

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
        printw("%s", footers[i]);
    }
}
