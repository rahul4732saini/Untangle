#include <stdlib.h>
#include <stdbool.h>

#include "typedefs.h"
#include "structs.h"

// Stores pointers to the data type parser functions.
void *(*parsers[])(char *);

integer_t *parse_int(char *src)
{
    integer_t *result = (integer_t *)malloc(sizeof(integer_t));
    len_t ctr;

    len_t matched = sscanf(src, "%lld%n", result, &ctr);

    if (matched == 1 && !src[ctr])
        return result;

    // Frees the memory and returns NULL if the source string does
    // not represent an integer.
    free(result);
    return NULL;
}

double *parse_float(char *src)
{
    double *result = (double *)malloc(sizeof(double));
    len_t ctr;

    len_t matched = sscanf(src, "%lf%n", result, &ctr);

    if (matched == 1 && !src[ctr])
        return result;

    // Frees the memory and returns NULL if the source string does
    // not represent an floating point number.
    free(result);
    return NULL;
}

    return result;
}

char *parse_string(char *src)
{
    return src;
}
