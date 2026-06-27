def _homebrew_openblas_repository_impl(repository_ctx):
    prefix = repository_ctx.os.environ.get(
        "HOMEBREW_OPENBLAS_PREFIX",
        "/home/linuxbrew/.linuxbrew/opt/openblas",
    )
    build_file = """load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_import(
    name = "openblas_shared",
    shared_library = "lib/libopenblas.so",
)

cc_library(
    name = "openblas",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    linkopts = ["-Wl,-rpath,{prefix}/lib"],
    deps = [":openblas_shared"],
)
""".format(prefix = prefix)

    repository_ctx.file("MODULE.bazel", "module(name = \"homebrew_openblas\")\n")
    repository_ctx.symlink(prefix + "/include", "include")
    repository_ctx.symlink(prefix + "/lib", "lib")
    repository_ctx.file("BUILD.bazel", build_file)

homebrew_openblas_repository = repository_rule(
    implementation = _homebrew_openblas_repository_impl,
    environ = ["HOMEBREW_OPENBLAS_PREFIX"],
    local = True,
)
