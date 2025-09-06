#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "consts.h"
#include "typedefs.h"

/**
 * @brief Extracts the absolute path to the plugin directory.
 */
static char *get_plugin_dir(void)
{
    char *buff = (char *)malloc(MAX_PATH + strlen(plugin_dir) + 2);
    len_t length = GetCurrentDirectory(MAX_PATH, buff);

    // Adds the relative path to the plugin directory at the end.
    sprintf(buff + length, "\\%s\\", plugin_dir);
    return buff;
}
