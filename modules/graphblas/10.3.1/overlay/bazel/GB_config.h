//------------------------------------------------------------------------------
// bazel/GB_config.h: Bazel configuration for a native macOS GraphBLAS build
//------------------------------------------------------------------------------

#ifndef GB_CONFIG_H
#define GB_CONFIG_H

#ifndef GB_C_COMPILER
#define GB_C_COMPILER "bazel-apple-clang"
#endif

#ifndef GB_C_FLAGS
#define GB_C_FLAGS "-std=c11"
#endif

#ifndef GB_C_LINK_FLAGS
#define GB_C_LINK_FLAGS ""
#endif

#ifndef GB_LIB_PREFIX
#define GB_LIB_PREFIX "lib"
#endif

#ifndef GB_LIB_SUFFIX
#define GB_LIB_SUFFIX ".dylib"
#endif

#ifndef GB_OBJ_SUFFIX
#define GB_OBJ_SUFFIX ".o"
#endif

#ifndef GB_OMP_INC
#define GB_OMP_INC ""
#endif

#ifndef GB_OMP_INC_DIRS
#define GB_OMP_INC_DIRS ""
#endif

#ifndef GB_C_LIBRARIES
#define GB_C_LIBRARIES ""
#endif

#ifndef GB_CMAKE_LIBRARIES
#define GB_CMAKE_LIBRARIES ""
#endif

#ifndef GB_CUDA_COMPILER
#define GB_CUDA_COMPILER ""
#endif

#ifndef GB_CUDA_FLAGS
#define GB_CUDA_FLAGS ""
#endif

#ifndef GB_CUDA_INC
#define GB_CUDA_INC ""
#endif

#endif
