# This file is licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

"""Source and configuration data for the experimental Bazel libomp port."""

LIBOMP_RUNTIME_SRCS = [
    "runtime/src/kmp_affinity.cpp",
    "runtime/src/kmp_alloc.cpp",
    "runtime/src/kmp_atomic.cpp",
    "runtime/src/kmp_barrier.cpp",
    "runtime/src/kmp_cancel.cpp",
    "runtime/src/kmp_collapse.cpp",
    "runtime/src/kmp_csupport.cpp",
    "runtime/src/kmp_debug.cpp",
    "runtime/src/kmp_dispatch.cpp",
    "runtime/src/kmp_environment.cpp",
    "runtime/src/kmp_error.cpp",
    "runtime/src/kmp_ftn_cdecl.cpp",
    "runtime/src/kmp_ftn_extra.cpp",
    "runtime/src/kmp_global.cpp",
    "runtime/src/kmp_gsupport.cpp",
    "runtime/src/kmp_i18n.cpp",
    "runtime/src/kmp_io.cpp",
    "runtime/src/kmp_lock.cpp",
    "runtime/src/kmp_runtime.cpp",
    "runtime/src/kmp_sched.cpp",
    "runtime/src/kmp_settings.cpp",
    "runtime/src/kmp_str.cpp",
    "runtime/src/kmp_taskdeps.cpp",
    "runtime/src/kmp_tasking.cpp",
    "runtime/src/kmp_threadprivate.cpp",
    "runtime/src/kmp_utility.cpp",
    "runtime/src/kmp_version.cpp",
    "runtime/src/kmp_wait_release.cpp",
    "runtime/src/z_Linux_asm.S",
    "runtime/src/z_Linux_util.cpp",
]

LIBOMP_COMMON_TEMPLATE_DEFINES = {
    "DEBUG_BUILD": "0",
    "KMP_BUILD_DATE": "No_Timestamp",
    "LIBOMP_ENABLE_ASSERTIONS": "0",
    "LIBOMP_ENABLE_SHARED": "0",
    "LIBOMP_HAVE_ALIGNED_ALLOC": "1",
    "LIBOMP_HAVE_POSIX_MEMALIGN": "1",
    "LIBOMP_HAVE_PSAPI": "0",
    "LIBOMP_HAVE_PTHREAD_SET_NAME_NP": "0",
    "LIBOMP_HAVE_RTM_INTRINSICS": "0",
    "LIBOMP_HAVE_VERSION_SYMBOLS": "0",
    "LIBOMP_HAVE_WAITPKG_INTRINSICS": "0",
    "LIBOMP_HAVE_WEAK_ATTRIBUTE": "1",
    "LIBOMP_OMPD_SUPPORT": "0",
    "LIBOMP_OMPT_DEBUG": "0",
    "LIBOMP_OMPT_OPTIONAL": "0",
    "LIBOMP_OMPT_SUPPORT": "0",
    "LIBOMP_OMPX_TASKGRAPH": "0",
    "LIBOMP_PROFILING_SUPPORT": "0",
    "LIBOMP_STATS": "0",
    "LIBOMP_USE_ADAPTIVE_LOCKS": "0",
    "LIBOMP_USE_DEBUGGER": "0",
    "LIBOMP_USE_HWLOC": "0",
    "LIBOMP_USE_INTERNODE_ALIGNMENT": "0",
    "LIBOMP_USE_ITT_NOTIFY": "0",
    "LIBOMP_USE_VERSION_SYMBOLS": "0",
    "LIBOMP_VERSION_BUILD": "20140926",
    "LIBOMP_VERSION_MAJOR": "5",
    "LIBOMP_VERSION_MINOR": "0",
    "MSVC": "0",
    "OPENMP_ENABLE_LIBOMPTARGET": "0",
    "RELWITHDEBINFO_BUILD": "0",
    "STUBS_LIBRARY": "0",
}

LIBOMP_PLATFORM_TEMPLATE_DEFINES = {
    "linux_x86_64": {
        "LIBOMP_ARCH_AARCH64_A64FX": "0",
        "LIBOMP_HAVE_IMMINTRIN_H": "1",
        "LIBOMP_HAVE_INTRIN_H": "0",
        "LIBOMP_HAVE_ATTRIBUTE_RTM": "0",
        "LIBOMP_HAVE_ATTRIBUTE_WAITPKG": "0",
        "LIBOMP_HAVE_X86INTRIN_H": "1",
        "LIBOMP_HAVE_XMMINTRIN_H": "1",
        "LIBOMP_HAVE__MM_MALLOC": "1",
        "LIBOMP_HAVE__ALIGNED_MALLOC": "0",
        "LIBOMP_LEGAL_ARCH": "x86_64",
        "LIBOMP_LIB_FILE": "libomp.a",
    },
    "macos_arm64": {
        "LIBOMP_ARCH_AARCH64_A64FX": "0",
        "LIBOMP_HAVE_IMMINTRIN_H": "0",
        "LIBOMP_HAVE_INTRIN_H": "0",
        "LIBOMP_HAVE_ATTRIBUTE_RTM": "0",
        "LIBOMP_HAVE_ATTRIBUTE_WAITPKG": "0",
        "LIBOMP_HAVE_X86INTRIN_H": "0",
        "LIBOMP_HAVE_XMMINTRIN_H": "0",
        "LIBOMP_HAVE__MM_MALLOC": "0",
        "LIBOMP_HAVE__ALIGNED_MALLOC": "0",
        "LIBOMP_LEGAL_ARCH": "aarch64",
        "LIBOMP_LIB_FILE": "libomp.a",
    },
}

def _merge_dicts(lhs, rhs):
    merged = dict(lhs)
    merged.update(rhs)
    return merged

def _shell_quote(value):
    return "'{}'".format(value.replace("'", "'\\''"))

def render_template_cmd(renderer_label, template_label, defines):
    merged = _merge_dicts(LIBOMP_COMMON_TEMPLATE_DEFINES, defines)
    args = [
        "--define {}={}".format(key, _shell_quote(merged[key]))
        for key in sorted(merged)
    ]
    return " ".join([
        "$(location {})".format(renderer_label),
        "--template $(location {})".format(template_label),
        "--output $@",
    ] + args)
