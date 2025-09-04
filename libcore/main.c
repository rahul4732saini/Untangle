#include "plugin.h"
#include "consts.h"

const Strings *get_fields(void)
{
    return &fields;
}

const Strings *get_domains(uint32_t inx)
{
    return domains[inx];
}
