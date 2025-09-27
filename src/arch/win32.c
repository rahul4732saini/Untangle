/**
 * @file posix.c
 * @brief Defines platform-dependent functionalities for Windows systems.
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#include "consts.h"
#include "shared.h"
#include "typedefs.h"
#include "structs.h"

static const char *lib_file_suffix = ".dll";
static const len_t lib_file_suffix_len = 4;

HMODULE *handlers = NULL;
PluginsData plugins;

len_t plugin_count;

/**
 * @brief Converts the specified UNIX path into a valid Windows path.
 *
 * @details Converts the path by replacing instances of the forward-slash(/)
 * character with the backward-slash(\).
 *
 * @param path Pointer to the buffer comprising the path to be converted.
 */
static char *convert_unix_path(char *path)
{
    for (char *ptr = path; *ptr; ++ptr)
        if (*ptr == '/')
            *ptr = '\\';

    return path;
}

/**
 * @brief Verifies whether the specified path points to an existing directory.
 * @param path Absolute path to the directory.
 */
static bool verify_directory(char *path)
{
    DWORD attrs = GetFileAttributes(path);
    return attrs != INVALID_FILE_ATTRIBUTES && attrs & FILE_ATTRIBUTE_DIRECTORY;
}

/**
 * @brief Validates the specified directory entry for a plugin file.
 * @param entry Directory entry of the file to be validated.
 */
static bool is_valid_plugin_file(WIN32_FIND_DATA *fs_entry)
{
    if (fs_entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        return false;

    char *match = strstr(fs_entry->cFileName, lib_file_suffix);

    // Only returns true if a match was found and there are no
    // characters present ahead of the expected suffix.
    return match && !match[lib_file_suffix_len];
}

/**
 * @brief Extracts the absolute path to the plugin directory.
 * @details Extracts and returns directory path, or NULL in case of an error.
 */
static char *get_plugin_dir(void)
{
    char *buff = (char *)malloc(MAX_PATH + strlen(plugin_dir) + 1);
    len_t length = GetCurrentDirectory(MAX_PATH, buff);

    // Fails and returns if there was an error while extracting the current directory path.
    if (!length)
    {
        free(buff);
        return NULL;
    }

    // Adds the relative path to the plugin directory at the end and
    // converts it into a Windows compatible path.
    sprintf(buff + length, "\\%s", plugin_dir);
    convert_unix_path(buff + length);

    return buff;
}

/**
 * @brief Extracts the number of plugin files within the specified directory.
 *
 * @details Extracts and returns the count of files present within the specified
 * directory that adhere to the plugin file naming scheme.
 *
 * @param path Absolute path to the plugin directory.
 */
static len_t get_plugin_count(char *path)
{
    // Creates a separate buffer to store the directory path and the
    // pattern specification to include all the file and directories.
    char buff[strlen(path) + 1];
    sprintf(buff, "%s*", path);

    WIN32_FIND_DATA fs_entry;
    HANDLE hFind = FindFirstFile(buff, &fs_entry);

    len_t ctr = 0;

    do
    {
        ctr += is_valid_plugin_file(&fs_entry);

    } while (FindNextFile(hFind, &fs_entry));

    FindClose(hFind);
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
    HMODULE lib = LoadLibrary(path);

    // Fails if the library cannot be opened successfully.
    if (!lib)
        return false;

    Domains *domains = (Domains *)GetProcAddress(lib, plugin_domains_var);
    char **name = (char **)GetProcAddress(lib, plugin_name_var);

    // Fails if the target symbols cannot be loaded from the library.
    if (!domains || !name)
    {
        FreeLibrary(lib);
        return false;
    }

    len_t plugin_name_len = strlen(name) + 1;

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

    // Allocates heap memory for a separate buffer to store the
    // library file path along with the plugin data.
    char *file_path = (char *)malloc(strlen(path) + 1);
    strcpy(file_path, path);

    // Stores the library handler and the plugin data.
    handlers[inx] = lib;
    plugins.plugins[inx] = (PluginData){
        .name = plugin_name,
        .path = file_path,
        .domains = domains,
        .enabled = true,
    };

    return true;
}

/**
 * @brief Extracts plugin data from the plugin libraries.
 */
PluginsData *get_plugins(void)
{
    if (handlers)
        return &plugins;

    char *dir_path = get_plugin_dir();

    len_t file_cnt = get_plugin_count(dir_path);
    len_t dir_path_len = strlen(dir_path);

    // Creates a separate buffer to store the directory path and the
    // pattern specification to include all the file and directories.
    char pattern[dir_path_len + 1];
    sprintf(pattern, "%s*", dir_path);

    // Allocates memory for storing the library handlers and plugin data.
    handlers = (HMODULE *)malloc(file_cnt * sizeof(HMODULE));
    plugins.plugins = (PluginData *)malloc(file_cnt * sizeof(PluginData));

    len_t ctr = 0;

    WIN32_FIND_DATA fs_entry;
    HANDLE hFind = FindFirstFile(pattern, &fs_entry);

    do
    {
        if (!is_valid_plugin_file(&fs_entry))
            continue;

        // Creates a separate buffer to store the absolute path of the library file.
        char buff[dir_path_len + strlen(fs_entry.cFileName) + 1];
        strcpy(buff, dir_path);
        strcat(buff, fs_entry.cFileName);

        // Updates the counter if the library is successfully loaded.
        if (load_plugin(buff, ctr))
            ++ctr;

    } while (FindNextFile(hFind, &fs_entry));

    // Saves the counter for future usage.
    plugins.size = plugin_count = ctr;

    FindClose(hFind);
    free(dir_path);

    return &plugins;
}

/**
 * @brief Toggles the availability of the plugin stored at the given index.
 *
 * @details If enabled, disables the plugin closing the library and resetting
 * the associated member variables. Whereas if disabled, loads the library into
 * memory, resolves target symbols, and updates the plugin data.
 *
 * @param inx Index of the plugin in the plugins array to be toggled.
 * @return Boolean value indicating whether the operation was a success.
 */
bool toggle_plugin(index_t inx)
{
    if (inx >= plugin_count)
        return false;

    PluginData *plugin = plugins.plugins + inx;

    // Disables the plugin if currently enabled.
    if (plugin->enabled)
    {
        FreeLibrary(handlers[inx]);

        // Resets the variables upon library closure.
        plugin->domains = NULL;
        plugin->enabled = false;

        return true;
    }

    HMODULE lib = LoadLibrary(plugin->path);

    // Fails if the library cannot be opened successfully.
    if (!lib)
        return false;

    Domains *domains = GetProcAddress(lib, plugin_domains_var);

    // Fails if the target symbol cannot be loaded from the library.
    if (!domains)
    {
        FreeLibrary(lib);
        return false;
    }

    // Adds the library handler to the handlers array and updates plugin data.
    handlers[inx] = lib;
    plugin->domains = domains;
    plugin->enabled = true;

    return true;
}

/**
 * @details Closes the plugin libraries and frees up heap memory.
 */
void clean_plugins(void)
{
    // Skips the clean process if it has already been done.
    if (!handlers)
        return;

    // Frees the memory allocated for storing plugin data, and closes all
    // the dynamically loaded libraries.
    for (len_t i = 0; i < plugin_count; ++i)
    {
        free(plugins.plugins[i].name);
        free(plugins.plugins[i].path);

        FreeLibrary(handlers[i]);
    }

    free(handlers);
    free(plugins.plugins);

    // Resets the variables to their defaults to mark them as un-initialized.
    handlers = NULL;
    plugins.size = plugin_count = 0;
}
