#if defined(_WIN32) || defined(_WIN64)
#define EXPORT __declspec(dllexport)

#else
#define EXPORT __attribute__((visibility("default")))

#endif

#include "structs.h"

// Name of the plugin to be displayed in the application interface.
EXPORT const char *plugin_name = "Core Plugin";
