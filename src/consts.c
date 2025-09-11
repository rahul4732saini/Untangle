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

// Default bucket count in the hash table for storing data.
const len_t default_hash_table_size = 30;

const char *main_menu_items[] = {
    "Solve Problems",
    "Extensions",
    "Quit",
};

const char *footers[] = {
    "Made by Rahul Saini",
    "Visit https://www.github.com/rahul4732saini/Untangle for more info.",
};

const len_t main_menu_items_len = 3;
const len_t footers_len = 2;

// Text to be displayed in the title bar for different menu screens.
const char *main_menu_title = "Untangle";
const char *domain_menu_title = "Domain Menu";
const char *field_menu_title = "Field Menu";
const char *problem_menu_title = "Problem Menu";
const char *extension_menu_title = "Extensions";

// Exclusive window title for the main menu.
const char *main_menu_window_title = "Main Menu";

// Placeholder to be displayed in the search box within the application
// interface if no other text is to be displayed.
const char *search_box_placeholder = "Search...";
