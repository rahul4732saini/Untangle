#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <stdint.h>

typedef struct
{
    const char **strings;
    const uint32_t size;
} Strings;

#endif