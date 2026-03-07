//------------------------------------------------------------------------------
// bazel/GB_JITpackage.c: empty JIT package for the Bazel no-JIT build
//------------------------------------------------------------------------------

#include "GB_JITpackage.h"

int GB_JIT_package_nfiles_get(void)
{
    return 0;
}

static GB_JITpackage_index_struct GB_JITpackage_index[1] = {
    {0, 0, NULL, NULL},
};

void *GB_JITpackage_index_get(void)
{
    return (void *) GB_JITpackage_index;
}
