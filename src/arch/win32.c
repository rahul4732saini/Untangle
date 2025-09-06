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

    // Adds the relative path to the plugin directory at the end.
    sprintf(buff + length, "\\%s\\", plugin_dir);
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
