#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "consts.h"
#include "typedefs.h"

#define DEFAULT_PATH_BUFF_SIZE 256
#define PATH_BUFF_INC_STEP 8

const char *lib_file_suffix = ".so";

/**
 * @brief Extracts the absolute path to the plugin directory.
 */
static char *get_plugin_dir(void)
{
    len_t size = DEFAULT_PATH_BUFF_SIZE;
    char *buff = (char *)malloc(size);

    // Expands the buffer until it is too small to contain the
    // absolute path to the current directory.
    while (!(getcwd(buff, size)))
    {
        size += PATH_BUFF_INC_STEP;
        buff = (char *)realloc(buff, size);
    }

    // Expands the buffer to also add the plugin directory path at the
    // end, and 1 more charcter for the '/' before the plugin directory
    // path.
    size += strlen(plugin_dir) + 1;
    buff = (char *)realloc(buff, size);

    strcat(buff, "/");
    strcat(buff, plugin_dir);

    return buff;
}
