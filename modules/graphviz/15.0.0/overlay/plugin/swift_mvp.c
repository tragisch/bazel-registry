#include "swift_mvp.h"

#include "builtin_dot_core.h"

const lt_symlist_t *gv_bazel_swift_mvp_builtins(void) {
    return gv_bazel_dot_core_builtins();
}

GVC_t *gv_bazel_swift_mvp_context(int demand_loading) {
    return gvContextPlugins(gv_bazel_swift_mvp_builtins(), demand_loading);
}
