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
 */
static char *convert_unix_path(char *src)
{
    for (char *ptr = src; *ptr; ++ptr)
        if (*ptr == '/')
            *ptr = '\\';

    return src;
}

/**
 * @brief Validates the specified directory entry for a plugin file.
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
 */
static char *get_plugin_dir(void)
{
    char *buff = (char *)malloc(MAX_PATH + strlen(plugin_dir) + 2);
    len_t length = GetCurrentDirectory(MAX_PATH, buff);

    // Adds the relative path to the plugin directory at the end and
    // converts it into a Windows compatible path.
    sprintf(buff + length, "\\%s", plugin_dir);
    convert_unix_path(buff + length);

    return buff;
}

/**
 * @brief Extracts the number of plugin files within the specified directory.
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
        char buff[dir_path_len + strlen(fs_entry.cFileName)];
        strcpy(buff, dir_path);
        strcat(buff, fs_entry.cFileName);

        HMODULE lib = LoadLibrary(buff);

        // Continues if the library cannot be opened successfully.
        if (!lib)
            continue;

        Domains *(**function)(void) = (Domains * (**)(void)) GetProcAddress(lib, plugin_domain_func);
        char **name = (char **)GetProcAddress(lib, plugin_name_var);

        // Continues if the target function or variable was not found in the library.
        if (!function || !name)
        {
            FreeLibrary(lib);
            continue;
        }

        // Stores the library handler and the plugin data.
        handlers[ctr] = lib;
        plugins.plugins[ctr++] = (PluginData){
            .name = *name,
            .function = *function,
            .enabled = true,
        };

    } while (FindNextFile(hFind, &fs_entry));

    // Saves the counter for future usage.
    plugins.size = plugin_count = ctr;

    FindClose(hFind);
    free(dir_path);

    return &plugins;
}

/**
 * @details Closes the plugin libraries and frees up heap memory.
 */
void clean(void)
{
    // Skips the clean process if it has already been done.
    if (!handlers)
        return;

    // Closes all the dynamically loaded plugin libraries.
    for (len_t i = 0; i < plugin_count; ++i)
        FreeLibrary(handlers[i]);

    free(handlers);
    free(plugins.plugins);

    // Resets the variables to their defaults to mark them as un-initialized.
    handlers = NULL;
    plugins.size = plugin_count = 0;
}
