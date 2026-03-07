def _homebrew_libomp_repository_impl(repository_ctx):
    root = repository_ctx.path("/opt/homebrew/opt/libomp")
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
            "#error \"Homebrew libomp not found at /opt/homebrew/opt/libomp. Run 'brew install libomp'.\"\n",
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
