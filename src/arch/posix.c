/**
 * @file posix.c
 * @brief Defines platform-dependent functionalities for POSIX-compliant systems.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <sys/stat.h>

#include "consts.h"
#include "typedefs.h"
#include "shared.h"
#include "structs.h"

#define DEFAULT_PATH_BUFF_SIZE 256
#define PATH_BUFF_INC_STEP 16

static const char *lib_file_suffix = ".so";
static const len_t lib_file_suffix_len = 3;

// Library open mode to load the symbols lazily and load them
// locally to avoid any conflicts and also ensure performance.
static const int lib_open_mode = RTLD_LAZY | RTLD_LOCAL;

static void **handlers = NULL;
static PluginsData plugins;

static len_t plugin_count;

/**
 * @brief Verifies whether the path exists and points to a valid directory.
 */
static bool verify_directory(char *path)
{
    struct stat dir_stat;

    if (!stat(path, &dir_stat))
        return false;

    return S_ISDIR(dir_stat.st_mode);
}

/**
 * @brief Validates the specified directory entry for a plugin file.
 */
static bool is_valid_plugin_file(struct dirent *entry)
{
    if (entry->d_type != DT_REG)
        return false;

    char *match = strstr(entry->d_name, lib_file_suffix);

    // Only returns true if a match was found and there are no
    // characters present ahead of the expected suffix.
    return match && !match[lib_file_suffix_len];
}

/**
 * @brief Extracts the absolute path to the plugin directory.
 */
static char *get_plugin_dir(void)
{
    len_t size = DEFAULT_PATH_BUFF_SIZE;
    char *buff = (char *)malloc(size);

    // Expands the buffer until it is big enough to store the absolute
    // path to the current directory.
    while (!(getcwd(buff, size)))
    {
        size += PATH_BUFF_INC_STEP;
        buff = (char *)realloc(buff, size);
    }

    // Expands the buffer to also add the plugin directory path at the
    // end, and 1 more character for the '/' before the plugin directory
    // path.
    size += strlen(plugin_dir) + 1;
    buff = (char *)realloc(buff, size);

    strcat(buff, "/");
    strcat(buff, plugin_dir);

    return buff;
}

/**
 * @brief Extracts the number of plugin files within the specified directory.
 */
static len_t get_plugin_count(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;

    len_t ctr = 0;

    while ((entry = readdir(dir)))
        ctr += is_valid_plugin_file(entry);

    closedir(dir);
    return ctr;
}

/**
 * @brief Loads plugin data from the specified library file and saves it.
 *
 * @details Loads the specified dynamic library file and saves the handler
 * and plugin data at the given index in the associated arrays.
 *
 * @param path Absolute path to the plugin library file.
 * @param inx Index to use for storing the data in the arrays.
 *
 * @return A boolean true if the process was successful, and false if
 * there was an error.
 */
static bool load_plugin(char *path, index_t inx)
{
    void *handler = dlopen(path, lib_open_mode);

    // Fails if the library cannot be opened successfully.
    if (!handler)
        return false;

    Domains *domains = (Domains *)dlsym(handler, plugin_domains_var);
    char **name = (char **)dlsym(handler, plugin_name_var);

    // Fails if the target symbols cannot be loaded from the library.
    if (!domains || !name)
    {
        dlclose(handler);
        return false;
    }

    len_t plugin_name_len = strlen(name);

    // Reduces the length of the buffer if the name is longer than the
    // maximum allowance.
    if (plugin_name_len > max_plugin_name_len)
        plugin_name_len = max_plugin_name_len;

    // Uses a separate buffer to store the plugin name to ensure
    // accessibility even if the library is closed during runtime.
    char *plugin_name = (char *)malloc(plugin_name_len);

    // Copies the plugin name to the buffer.
    strncpy(plugin_name, name, plugin_name_len);
    plugin_name[plugin_name_len - 1] = 0;

    // Stores the library handler and the plugin data.
    handlers[inx] = handler;
    plugins.plugins[inx] = (PluginData){
        .domains = domains,
        .name = plugin_name,
        .enabled = true,
    };

    return true;
}

/**
 * @brief Extracts plugin data from the plugin libraries.
 */
PluginsData *get_plugins(void)
{
    // Avoid re-loading the libraries and extracting the functions if
    // already done.
    if (handlers)
        return &plugins;

    char *dir_path = get_plugin_dir();

    // Returns if the plugin directory does not exist.
    if (!verify_directory(dir_path))
        return NULL;

    len_t file_cnt = get_plugin_count(dir_path);
    len_t dir_path_len = strlen(dir_path);

    // Allocates memory for storing the library handlers and plugin data.
    handlers = malloc(file_cnt * sizeof(void *));
    plugins.plugins = (PluginData *)malloc(file_cnt * sizeof(PluginData));

    // Keeps track of the current index in the handlers and functions array.
    len_t ctr = 0;

    DIR *dir = opendir(dir_path);
    struct dirent *entry;

    while ((entry = readdir(dir)))
    {
        if (!is_valid_plugin_file(entry))
            continue;

        // Extracts and saves the absolute path to the plugin library to avoid
        // loading a library from the global environment if the name conflicts.
        char buff[dir_path_len + strlen(entry->d_name) + 1];
        strcpy(buff, dir_path);
        strcat(buff, entry->d_name);

        // Updates the counter if the library is successfully loaded.
        if (load_plugin(buff, ctr))
            ++ctr;
    }

    // Saves the counter for future usage.
    plugins.size = plugin_count = ctr;

    free(dir_path);
    closedir(dir);

    return &plugins;
}

/**
 * @details Closes the plugin libraries and frees up heap memory.
 */
void clean_plugins(void)
{
    // Skips the clean process if it has already been done.
    if (!handlers)
        return;

    // Frees the memory allocated for the plugin names and closes all
    // the dynamically loaded libraries.
    for (len_t i = 0; i < plugin_count; ++i)
    {
        free(plugins.plugins[i].name);
        dlclose(handlers[i]);
    }

    free(handlers);
    free(plugins.plugins);

    // Resets the variables to their defaults to mark them as un-initialized.
    handlers = NULL;
    plugin_count = plugins.size = 0;
}
