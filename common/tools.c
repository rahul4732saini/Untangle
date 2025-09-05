#include <stdbool.h>
#include <ctype.h>

/*
 * @brief Compares the given two strings irrespective of their character casing.
*/
bool str_case_cmp(char *str1, char *str2)
{
    for (char c1, c2; *str1 && *str2; ++str1, ++str2)
        if (tolower(*str1) != tolower(*str2))
            break;

    return !*str1 && !*str2;
}

