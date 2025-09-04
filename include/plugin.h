#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <stdint.h>

typedef uint16_t dtype_t;
typedef uint32_t len_t;

typedef struct
{
    const char **strings;
    const uint32_t size;
} Strings;

#endif