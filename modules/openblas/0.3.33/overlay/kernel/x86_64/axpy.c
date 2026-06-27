/* Profile wrapper for Bazel registry overlay. */
#if defined(OPENBLAS_PROFILE_HASWELL) && defined(__x86_64__)
#if defined(DOUBLE)
#include "kernel/x86_64/daxpy.c"
#else
#include "kernel/x86_64/saxpy.c"
#endif
#else
#include "kernel/arm/axpy.c"
#endif
