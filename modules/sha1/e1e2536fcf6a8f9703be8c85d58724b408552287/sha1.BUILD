load("@aspect_bazel_lib//lib:copy_to_directory.bzl", "copy_to_directory")

copy_to_directory(
    name = "sha1_h",
    srcs = [
        "sha1.h",
    ],
    out = "include",
    add_directory_to_runfiles = True,
    visibility = ["//visibility:__pkg__"],
)

cc_library(
    name = "sha1",
    srcs = [
        "sha1.c",
    ],
    hdrs = [
        "sha1.h",
        ":sha1_h",
    ],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
