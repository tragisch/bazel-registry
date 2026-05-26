#ifndef OPENBLAS_BAZEL_CONFIG_H
#define OPENBLAS_BAZEL_CONFIG_H

#if defined(__APPLE__)
#define OS_DARWIN 1
#elif defined(_WIN32)
#define OS_WINNT 1
#elif defined(__linux__)
#define OS_LINUX 1
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
#define ARCH_ARM64 1
#elif defined(__x86_64__) || defined(_M_X64)
#define ARCH_X86_64 1
#elif defined(__i386__) || defined(_M_IX86)
#define ARCH_X86 1
#endif

#if defined(__clang__)
#define C_CLANG 1
#elif defined(__GNUC__)
#define C_GCC 1
#elif defined(_MSC_VER)
#define C_MSVC 1
#endif

#if defined(_WIN32)
#define FUNDERSCORE
#define BUNDERSCORE
#else
#define FUNDERSCORE _
#define BUNDERSCORE _
#define NEEDBUNDERSCORE 1
#endif

#if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
#define __64BIT__ 1
#endif

#define GENERIC 1
#define CORE_generic 1
#define CHAR_CORENAME "generic"

#define L1_DATA_SIZE 32768
#define L1_DATA_LINESIZE 64
#define L2_SIZE 524288
#define L2_LINESIZE 64
#define DTB_DEFAULT_ENTRIES 128
#define DTB_SIZE 4096
#define L2_ASSOCIATIVE 8
#define GEMM_MULTITHREAD_THRESHOLD 4

#endif
