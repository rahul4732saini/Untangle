#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "typedefs.h"

/**
 * @brief Compares the given two strings irrespective of their character casing.
 */
bool str_case_cmp(char *str1, char *str2)
{
    for (char c1, c2; *str1 && *str2; ++str1, ++str2)
        if (tolower(*str1) != tolower(*str2))
            break;

    return !*str1 && !*str2;
}

/**
 * @brief Strips leading and trailing spaces from the string.
 */
char *str_strip(char *src)
{
    while (*src == ' ')
        ++src;

    for (len_t i = strlen(src) - 1; i >= 0; --i)
    {
        if (src[i] != ' ')
            break;

        src[i] = 0;
    }

    return src;
}
