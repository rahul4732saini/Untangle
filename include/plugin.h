#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <stdint.h>

typedef uint16_t dtype_t;
typedef uint32_t len_t;

typedef struct
{
    const char **strings;
    const len_t size;
} Strings;

typedef struct
{
    const dtype_t type;
    void *data;
} ResultStep;

typedef struct
{
    ResultStep *steps;
    len_t size;
} Result;

typedef struct
{
    const dtype_t *args;
    const len_t args_size;
    const Strings *prompts;
    Result *(*handler)(void *, ...);
} Problem;

#endif