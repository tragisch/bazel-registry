def _homebrew_libomp_repository_impl(repository_ctx):
    root = repository_ctx.path("/opt/homebrew/opt/libomp")
    include_dir = root.get_child("include")
    dylib = root.get_child("lib").get_child("libomp.dylib")

    if not include_dir.exists:
        # Fallback for Intel Mac with Homebrew in /usr/local
        root = repository_ctx.path("/usr/local/opt/libomp")
        include_dir = root.get_child("include")
        dylib = root.get_child("lib").get_child("libomp.dylib")

    if include_dir.exists and dylib.exists:
        repository_ctx.symlink(include_dir, "include")
        repository_ctx.symlink(dylib, "libomp.dylib")
        repository_ctx.file(
            "BUILD.bazel",
            """
load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

cc_import(
    name = "libomp_shared",
    shared_library = "libomp.dylib",
)

cc_library(
    name = "libomp",
    hdrs = glob(["include/**/*.h"]),
    includes = ["include"],
    deps = [":libomp_shared"],
    visibility = ["//visibility:public"],
)
""",
        )
    else:
        repository_ctx.file(
            "include/omp.h",
            "#error \"Homebrew libomp not found. Run 'brew install libomp'.\"\n",
        )
        repository_ctx.file(
            "BUILD.bazel",
            """
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libomp",
    hdrs = ["include/omp.h"],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
""",
        )

homebrew_libomp_repository = repository_rule(
    implementation = _homebrew_libomp_repository_impl,
    local = True,
)

def _system_openmp_repository_impl(repository_ctx):
    """Locate system OpenMP on Linux (libomp-dev / libgomp)."""
    result = repository_ctx.execute(["sh", "-c", "ls /usr/lib/x86_64-linux-gnu/libomp.so 2>/dev/null || ls /usr/lib/x86_64-linux-gnu/libgomp.so 2>/dev/null || ls /usr/lib/libgomp.so 2>/dev/null"])

    # Try standard include paths
    omp_header = None
    for p in ["/usr/include", "/usr/lib/llvm-18/include", "/usr/lib/llvm-17/include", "/usr/lib/llvm-16/include"]:
        h = repository_ctx.path(p + "/omp.h")
        if h.exists:
            omp_header = repository_ctx.path(p)
            break

    # Try standard lib paths for the shared library
    omp_lib = None
    for lib in [
        "/usr/lib/x86_64-linux-gnu/libomp.so",
        "/usr/lib/x86_64-linux-gnu/libgomp.so",
        "/usr/lib/libgomp.so",
        "/usr/lib64/libgomp.so",
    ]:
        if repository_ctx.path(lib).exists:
            omp_lib = repository_ctx.path(lib)
            break

    if omp_header and omp_lib:
        repository_ctx.symlink(omp_header, "include")
        repository_ctx.symlink(omp_lib, "libomp.so")
        repository_ctx.file(
            "BUILD.bazel",
            """
load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

cc_import(
    name = "openmp_shared",
    shared_library = "libomp.so",
)

cc_library(
    name = "openmp",
    hdrs = glob(["include/omp.h"]),
    includes = ["include"],
    deps = [":openmp_shared"],
    visibility = ["//visibility:public"],
)
""",
        )
    else:
        repository_ctx.file(
            "include/omp.h",
            "#error \"System OpenMP not found. Install libomp-dev or libgomp via your package manager.\"\n",
        )
        repository_ctx.file(
            "BUILD.bazel",
            """
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "openmp",
    hdrs = ["include/omp.h"],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
""",
        )

system_openmp_repository = repository_rule(
    implementation = _system_openmp_repository_impl,
    local = True,
)
