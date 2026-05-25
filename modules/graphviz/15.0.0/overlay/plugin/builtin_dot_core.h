#pragma once

#include <gvc/gvcext.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lt_symlist_t gv_bazel_builtin_dot_core[];
const lt_symlist_t *gv_bazel_dot_core_builtins(void);

#ifdef __cplusplus
}
#endif
