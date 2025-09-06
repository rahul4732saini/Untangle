#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "consts.h"
#include "typedefs.h"
#include "shared.h"
#include "structs.h"

#define DEFAULT_PATH_BUFF_SIZE 256
#define PATH_BUFF_INC_STEP 8

const char *lib_file_suffix = ".so";
const len_t lib_file_suffix_len = 3;

void **handlers = NULL;
PluginFunctions functions;

len_t plugin_count;

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

/**
 * @brief Extracts the number of plugin files within a directory.
 */
static len_t get_plugin_count(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;

    len_t ctr = 0;

    while ((entry = readdir(dir)))
    {
        if (entry->d_type != DT_REG)
            continue;

        char *match = strstr(entry->d_name, lib_file_suffix);

        // Skips if no match is found or there are more characters
        // present ahead of the expected suffix.
        if (match && !match[lib_file_suffix_len])
            ++ctr;
    }

    closedir(dir);
    return ctr;
}
