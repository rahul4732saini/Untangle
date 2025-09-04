#ifndef _SHARED_PLUGIN_H
#define _SHARED_PLUGIN_H

#include <stdint.h>

typedef struct
{
    const char **strings;
    const uint32_t size;
} Strings;

#endif