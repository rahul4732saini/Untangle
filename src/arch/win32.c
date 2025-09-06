#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "consts.h"
#include "typedefs.h"

static const char *lib_file_suffix = ".dll";
static const len_t lib_file_suffix_len = 4;

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
        if (fs_entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        char *match = strstr(fs_entry.cFileName, lib_file_suffix);

        // Skips if no match is found or there are more characters
        // present ahead of the expected suffix.
        if (match && !match[lib_file_suffix_len])
            ++ctr;

    } while (FindNextFile(hFind, &fs_entry));

    FindClose(hFind);
    return ctr;
}
