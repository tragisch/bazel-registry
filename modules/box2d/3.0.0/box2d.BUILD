cc_library(
    name = "box2d",
    srcs = glob([
        "src/*.c",
        "src/*.h",
    ]) + [
        "extern/glad/include/KHR/khrplatform.h",
        "extern/glad/include/glad/glad.h",
        "extern/glad/src/glad.c",
    ] + ["extern/jsmn/jsmn.h"] + glob([
        "extern/simde/*.h",
        "extern/simde/x86/*.h",
    ]),
    hdrs = glob(["include/box2d/*.h"]),
    copts = [
        "-Wno-undef",
        "-Wno-unused-variable",
        "-Wno-unused-but-set-variable",
        "-arch arm64",
        "-fsanitize=address",
        "-fsanitize-address-use-after-scope",
        "-fsanitize=undefined",
    ],
    includes = [
        "extern/glad/include",
        "extern/jsmn",
        "extern/simde",
        "include",
    ],
    linkopts = [
        "-fsanitize=address",
        "-fsanitize-address-use-after-scope",
        "-fsanitize=undefined",
    ],
    linkstatic = 1,
    visibility = ["//visibility:public"],
)
