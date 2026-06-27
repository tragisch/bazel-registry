/* Profile wrapper for Bazel registry overlay. */
#if defined(OPENBLAS_PROFILE_HASWELL) && defined(__x86_64__)
#if defined(DOUBLE)
#include "kernel/x86_64/dasum.c"
#else
#include "kernel/x86_64/sasum.c"
#endif
#else
#include "kernel/arm/asum.c"
#endif
