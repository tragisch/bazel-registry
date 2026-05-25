#include "builtin_dot_core.h"

#include <gvc/gvplugin.h>

extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;

const lt_symlist_t gv_bazel_builtin_dot_core[] = {
    {"gvplugin_dot_layout_LTX_library", &gvplugin_dot_layout_LTX_library},
    {"gvplugin_core_LTX_library", &gvplugin_core_LTX_library},
    {0, 0},
};

const lt_symlist_t *gv_bazel_dot_core_builtins(void) {
    return gv_bazel_builtin_dot_core;
}
