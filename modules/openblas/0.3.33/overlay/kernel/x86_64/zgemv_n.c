/* Profile wrapper for Bazel registry overlay. */
#if defined(OPENBLAS_PROFILE_HASWELL) && defined(__x86_64__)
#if defined(DOUBLE)
#include "kernel/x86_64/zgemv_n_4.c"
#else
#include "kernel/x86_64/cgemv_n_4.c"
#endif
#else
#include "kernel/arm/zgemv_n.c"
#endif
