#include <stdlib.h>
#include <stdbool.h>

#include "typedefs.h"
#include "structs.h"

// Stores pointers to the data type parser functions.
void *(*parsers[])(char *);

integer_t *parse_int(char *src)
{
    integer_t *result = (integer_t *)malloc(sizeof(integer_t));

    // Frees the memory and returns NULL if the source string does
    // not represent an integer.
    if (!sscanf(src, "%lld", result))
    {
        free(result);
        return NULL;
    }

    return result;
}

double *parse_float(char *src)
{
    double *result = (double *)malloc(sizeof(double));

    // Frees the memory and returns NULL if the source string does
    // not represent a floating point number.
    if (sscanf(src, "%lf", result) != 1)
    {
        free(result);
        return NULL;
    }

    return result;
}

char *parse_string(char *src)
{
    return src;
}
