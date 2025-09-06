#include "structs.h"
#include "topics.h"

// Name of the plugin to be displayed within the application.
const char *plugin_name = "Core Plugin";

const Domains *get_domains(void)
{
    return &domains;
}
