/* Haswell-tuned OpenBLAS config for registry build.
 * This file is intentionally conservative; it enables x86_64-specific
 * kernel pathing and sets thread-related defaults closer to typical
 * Homebrew builds (DYNAMIC_ARCH-like behavior at runtime is still
 * limited in this simplified overlay).
 */

#ifndef OPENBLAS_BAZEL_CONFIG_HASWELL_H
#define OPENBLAS_BAZEL_CONFIG_HASWELL_H

#if defined(__APPLE__)
#define OS_DARWIN 1
#elif defined(_WIN32)
#define OS_WINNT 1
#elif defined(__linux__)
#define OS_LINUX 1
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define ARCH_X86_64 1
#endif

#if defined(__clang__)
#define C_CLANG 1
#elif defined(__GNUC__)
#define C_GCC 1
#endif

#define __64BIT__ 1

/* Indicate a haswell core where OpenBLAS can enable tuned kernels */
#define CORE_haswell 1
#define HASWELL 1
#define CHAR_CORENAME "haswell"

/* Build both single and double precision */
#define BUILD_SINGLE 1
#define BUILD_DOUBLE 1
#define BUILD_COMPLEX 1
#define BUILD_COMPLEX16 1

/* Adopt larger parallel defaults to match local Homebrew builds */
#define USE_OPENMP 1
#define SMP
#define SMP_SERVER 1
#define MAX_CPU_NUMBER 64
#define MAX_PARALLEL_NUMBER 56
#define GEMM_MULTITHREAD_THRESHOLD 1

/* CPU feature flags (required by kernel dispatch code) */
#define HAVE_CMOV 1
#define HAVE_MMX 1
#define HAVE_SSE 1
#define HAVE_SSE2 1
#define HAVE_SSE3 1
#define HAVE_SSSE3 1
#define HAVE_SSE4_1 1
#define HAVE_SSE4_2 1
#define HAVE_AVX 1
#define HAVE_AVX2 1
#define HAVE_FMA3 1

/* GEMM parameters from param.h HASWELL block */
#define SNUMOPT 16
#define DNUMOPT 8
#define GEMM_DEFAULT_OFFSET_A 0
#define GEMM_DEFAULT_OFFSET_B 0
#define GEMM_DEFAULT_ALIGN (BLASLONG)0x03fffUL

/* GEMM unrolling parameters for x86_64 (not ARCH_X86) */
#define SGEMM_DEFAULT_UNROLL_M 8
#define SGEMM_DEFAULT_UNROLL_N 4
#define SGEMM_DEFAULT_UNROLL_MN 32
#define DGEMM_DEFAULT_UNROLL_M 4
#define DGEMM_DEFAULT_UNROLL_N 8
#define DGEMM_DEFAULT_UNROLL_MN 32

/* GEMM blocking parameters for x86_64 — match param.h HASWELL values */
#define SGEMM_DEFAULT_P 512
#define SGEMM_DEFAULT_Q 256
#define SGEMM_DEFAULT_R sgemm_r
#define DGEMM_DEFAULT_P 512
#define DGEMM_DEFAULT_Q 256
#define DGEMM_DEFAULT_R dgemm_r

/* Cache sizes (typical x86_64 values) */
#define L1_DATA_SIZE 32768
#define L1_DATA_LINESIZE 64
#define L2_SIZE 262144
#define L2_LINESIZE 64
#define DTB_DEFAULT_ENTRIES 64
#define DTB_SIZE 4096
#define L2_ASSOCIATIVE 8

#endif /* OPENBLAS_BAZEL_CONFIG_HASWELL_H */
