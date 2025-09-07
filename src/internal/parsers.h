// Only meant for internal usage by core/parsers.h

#ifndef _INTERNAL_PARSERS_H
#define _INTERNAL_PARSERS_H

#include "typedefs.h"
#include "structs.h"

static void *parse_bool(char *src);
static void *parse_int(char *src);
static void *parse_float(char *src);
static void *parse_complex(char *src);
static void *parse_numeric(char *src);
static void *parse_string(char *src);

#endif
