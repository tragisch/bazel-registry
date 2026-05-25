#pragma once

#include <gvc/gvc.h>
#include <gvc/gvcext.h>

#ifdef __cplusplus
extern "C" {
#endif

const lt_symlist_t *gv_bazel_swift_mvp_builtins(void);
GVC_t *gv_bazel_swift_mvp_context(int demand_loading);

#ifdef __cplusplus
}
#endif
