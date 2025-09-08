/**
 * @file consts.c
 * @brief Defines constant variables providing assistance to the
 * various functions defined within the project.
 */

#include "typedefs.h"

// Relative path to the plugin directory.
const char *plugin_dir = "plugin/";

// Name of the variables declared by plugin libraries, used for
// locating the associated symbols in the plugin.
const char *plugin_domains_var = "domains";
const char *plugin_name_var = "plugin_name";

// String representation of boolean values expected from the user
// for boolean inputs.
const char *bool_true = "true";
const char *bool_false = "false";

const char *main_menu_items[] = {
    "Start Solving",
    "Extensions",
    "Quit",
};

const char *footers[] = {
    "Made by Rahul Saini",
    "Visit https://www.github.com/rahul4732saini/Untangle for more info.",
};

const len_t main_menu_items_len = 3;
const len_t footers_len = 2;

const char *main_menu_top_bar_title = "Untangle";
const char *main_menu_title = "Main Menu";
