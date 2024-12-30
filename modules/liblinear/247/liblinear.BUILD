load("@aspect_bazel_lib//lib:copy_to_directory.bzl", "copy_to_directory")

copy_to_directory(
    name = "linear_h",
    srcs = [
        "linear.h",
    ],
    out = "include",
    add_directory_to_runfiles = True,
    visibility = ["//visibility:__pkg__"],
)

cc_library(
    name = "liblinear",
    srcs = [
        "linear.cpp",
        "newton.cpp",
        "newton.h",
        "predict.c",
        "svm-scale.c",
        "train.c",
    ],
    hdrs = [
        "linear.h",
        ":linear_h",
    ],
    copts = select({
        "//conditions:default": [
            "-Wall",
            "-Wconversion",
            "-Wno-sign-conversion",
            "-Wno-deprecated-declarations",
            "-O3",
            "-fPIC",
        ],
        "@bazel_tools//src/conditions:darwin": [
            "-Wall",
            "-Wconversion",
            "-Wno-sign-conversion",
            "-Wno-deprecated-declarations",
            "-O3",
            "-fPIC",
            "-DACCELERATE_NEW_LAPACK",
        ],
    }),
    includes = [
        "include",
    ],
    linkopts = select({
        "//conditions:default": [],
        "@bazel_tools//src/conditions:darwin": [
            "-framework Accelerate",
        ],
        "@bazel_tools//src/conditions:linux": [
            "-lblas",
            "-llapack",
        ],
    }),
    visibility = ["//visibility:public"],
)
