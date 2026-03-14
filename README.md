## About
A small Bazel-registry for common non-Bazel libraries. 
- Native Build patches
- MacOS and Linux (Windows not tested).
- Focus on C-Developer

## How to use

```
# make sure bzlmod is enabled
common --enable_bzlmod

# add this bazel registry
common --registry=https://raw.githubusercontent.com/tragisch/bazel-registry/main
# by adding this registry the BCR gets removed so you must add it here as well
common --registry=https://bcr.bazel.build
```

## Available Modules

| Module                                                        | Version  | Description                                                                 | Build |
| ------------------------------------------------------------- | -------- | --------------------------------------------------------------------------- | ----- |
| [argon2](https://github.com/P-H-C/phc-winner-argon2/)         | 20190702 | Argon2 password hashing                                                     | ✅     |
| [argtable3](https://github.com/argtable/argtable3)            | 3.2.2    | ANSI C library for parsing GNU-style command line options                   | ✅     |
| [box2d](https://github.com/erincatto/box2d)                   | 3.0.0    | 2D physics engine for games                                                 | ✅     |
| [czmq](https://github.com/zeromq/czmq)                        | 4.2.1    | High-level C binding for ZeroMQ                                             | ✅     |
| [graphblas](https://github.com/DrTimothyAldenDavis/GraphBLAS) | 10.3.1   | SuiteSparse:GraphBLAS – a complete implementation of the GraphBLAS standard | ✅     |
| [liblinear](https://github.com/cjlin1/liblinear)              | 247      | A library for large linear classification                                   | ✅     |
| [libsodium](https://github.com/jedisct1/libsodium)            | 1.0.20   | Portable, cross-compilable crypto library (NaCl-compatible)                 | ✅     |
| [libzmq](https://github.com/zeromq/libzmq/)                   | 4.3.5    | High-performance asynchronous messaging library                             | ✅     |
| [raylib](https://github.com/raysan5/raylib)                   | 5.5      | Simple library to enjoy videogames programming                              | ✅     |
| [sha1](https://github.com/clibs/sha1)                         | e1e2536  | NIST Secure Hash Algorithm (SHA-1) implementation                           | ✅     |
| [throw_the_switch](https://github.com/ThrowTheSwitch/Unity)   | 2.5.2    | Unity – simple unit testing framework for C                                 | ✅     |
| [uthash](https://github.com/troydhanson/uthash)               | 2.3.0    | C macros for hash tables and more                                           | ✅     |

![Test Registry](https://github.com/tragisch/bazel-registry/actions/workflows/test-registry.yml/badge.svg)
