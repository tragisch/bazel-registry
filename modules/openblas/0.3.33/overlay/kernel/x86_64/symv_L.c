/* Profile wrapper for Bazel registry overlay. */
#if defined(OPENBLAS_PROFILE_HASWELL) && defined(__x86_64__)
#if defined(DOUBLE)
#include "kernel/x86_64/dsymv_L.c"
#else
#include "kernel/x86_64/ssymv_L.c"
#endif
#else
#include "kernel/arm/symv_L.c"
#endif
