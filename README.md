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

| Module                                                        | Version  | Description                                                                   | Dependency                         | Build |
| ------------------------------------------------------------- | -------- | ----------------------------------------------------------------------------- | ---------------------------------- | ----- |
| [argon2](https://github.com/P-H-C/phc-winner-argon2/)         | 20190702 | Argon2 password hashing                                                       | `@argon2//:argon2`                 | ✅     |
| [argtable3](https://github.com/argtable/argtable3)            | 3.2.2    | ANSI C library for parsing GNU-style command line options                     | `@argtable3//:argtable3`           | ✅     |
| [box2d](https://github.com/erincatto/box2d)                   | 3.0.0    | 2D physics engine for games                                                   | `@box2d//:box2d`                   | ✅     |
| [cglm](https://github.com/recp/cglm)                          | 0.9.6    | Highly optimized 2D / 3D graphics math library for C                          | `@cglm//:cglm`                     | ✅     |
| [cjson](https://github.com/DaveGamble/cJSON)                  | 1.7.19   | Ultralightweight JSON parser in ANSI C                                        | `@cjson//:cjson`                   | ✅     |
| [csv_fast](https://github.com/tragisch/csv_fast)              | 0.1.0    | Fast CSV parser and writer library in C                                       | `@csv_fast//:csv_fast`             | ✅     |
| [czmq](https://github.com/zeromq/czmq)                        | 4.2.1    | High-level C binding for ZeroMQ                                               | `@czmq//:czmq`                     | ✅     |
| [gnuplot_i](https://github.com/longradix/gnuplot_i)           | 1.0      | gnuplot C interface library                                                   | `@gnuplot_i//:gnuplot_i`           | ✅     |
| [graphblas](https://github.com/DrTimothyAldenDavis/GraphBLAS) | 10.3.1   | SuiteSparse:GraphBLAS – a complete implementation of the GraphBLAS standard   | `@graphblas//:graphblas`           | ✅     |
| [graphviz](https://graphviz.org/)                             | 15.0.0   | Graph visualization tools with root alias for the `cgraph` library            | `@graphviz//:graphviz`             | ✅     |
| [gunslinger](https://github.com/MrFrenik/gunslinger)          | 0.03-alpha | Header-only C99 framework for multimedia applications                       | `@gunslinger//:gunslinger`         | ✅     |
| [libb64](https://github.com/libb64/libb64)                    | 2.0.0.1  | Base64 encode and decode library in C                                         | `@libb64//:libb64`                 | ✅     |
| [libcsv](https://github.com/rgamble/libcsv)                   | b1d5212  | CSV parser and writer library in C                                            | `@libcsv//:libcsv`                 | ✅     |
| [liblinear](https://github.com/cjlin1/liblinear)              | 247      | A library for large linear classification                                     | `@liblinear//:liblinear`           | ✅     |
| [libsodium](https://github.com/jedisct1/libsodium)            | 1.0.20   | Portable, cross-compilable crypto library (NaCl-compatible)                   | `@libsodium//:libsodium`           | ✅     |
| [libzmq](https://github.com/zeromq/libzmq/)                   | 4.3.5    | High-performance asynchronous messaging library                               | `@libzmq//:libzmq`                 | ✅     |
| [linmath](https://github.com/datenwolf/linmath.h)             | 3eef828  | Small linear math library for computer graphics                               | `@linmath//:linmath`               | ✅     |
| [lmdb](https://github.com/LMDB/lmdb)                          | 0.9.31   | Lightning memory-mapped database library                                      | `@lmdb//:lmdb`                     | ✅     |
| [matio](https://github.com/tbeu/matio)                        | 1.5.27   | MATLAB MAT file I/O library                                                   | `@matio//:matio`                   | ✅     |
| [mpc](https://github.com/orangeduck/mpc)                      | 0.9.0    | Parser combinator library for C                                               | `@mpc//:mpc`                       | ✅     |
| [openmp](https://openmp.llvm.org/)                            | 22.1.1   | LLVM OpenMP runtime as native Bazel module with static `libomp`               | `@openmp//:libomp`                 | ✅     |
| [pbplots](https://github.com/InductiveComputerScience/pbPlots) | 0.1.9.1 | Plotting library available in many programming languages                      | `@pbplots//:pbPlots`               | ✅     |
| [pcg](https://github.com/imneme/pcg-c)                        | 83252d9  | PCG random number generation library, C edition                              | `@pcg//:pcg`                       | ✅     |
| [raylib](https://github.com/raysan5/raylib)                   | 5.5      | Simple library to enjoy videogames programming                                | `@raylib//:raylib`                 | ✅     |
| [sds](https://github.com/antirez/sds)                         | 2.0.0    | Simple Dynamic Strings library for C                                          | `@sds//:sds`                       | ✅     |
| [sha1](https://github.com/clibs/sha1)                         | e1e2536  | NIST Secure Hash Algorithm (SHA-1) implementation                             | `@sha1//:sha1`                     | ✅     |
| [throw_the_switch](https://github.com/ThrowTheSwitch/Unity)   | 2.5.2    | Unity – simple unit testing framework for C                                   | `@throw_the_switch//:Unity`        | ✅     |
| [utest](https://github.com/tymonx/utest)                      | f2fe288  | Lightweight unit testing framework for C++/C projects                         | `@utest//:utest`                   | ✅     |
| [uthash](https://github.com/troydhanson/uthash)               | 2.3.0    | C macros for hash tables and more                                             | `@uthash//:uthash`                 | ✅     |
| [yyjson](https://github.com/ibireme/yyjson)                   | 0.10.0   | High performance JSON library written in ANSI C                               | `@yyjson//:yyjson`                 | ✅     |

> **Note:** `openmp` exposes the static runtime target `@openmp//:libomp` (alias for `libomp_runtime_static`) and is validated in CI via `@openmp//:omp_smoke_test`.

> **Note:** `cglm` exposes `@cglm//:cglm` (header-only) and `@cglm//:cglmc` (compiled `glmc_*` API).

> **Note:** `graphviz` exposes `@graphviz//:graphviz` as the public root target; it currently resolves to the `cgraph` library.

## Module-Generator

Für neue Registry-Einträge gibt es ein lokales Hilfsskript in `tools/add_module.py`.
Es ist auf dieses Repo zugeschnitten und erzeugt direkt das Layout mit
`metadata.json`, `MODULE.bazel`, `source.json` und `overlay/BUILD.bazel`.

### JSON-Modus

Beispiel für eine Input-Datei:

```json
{
	"name": "demo",
	"version": "1.2.3",
	"url": "https://example.com/demo-1.2.3.tar.gz",
	"build_file": "/absolute/path/to/BUILD.bazel",
	"module_file": "/absolute/path/to/MODULE.bazel",
	"strip_prefix": "demo-1.2.3",
	"license": "MIT",
	"description": "Demo library",
	"homepage": "https://example.com/demo",
	"system": ["macos", "linux"],
	"patches": ["/absolute/path/to/fix.patch"]
}
```

Alternativ kann die Quelle auch aus einem `git_repository`-ähnlichen Input
abgeleitet werden. Das Skript normalisiert diesen Fall intern auf eine
Archiv-URL, damit der Registry-Eintrag weiterhin als `source.json` mit
`url` und `integrity` geschrieben wird.

```json
{
	"name": "demo",
	"version": "1.2.3",
	"remote": "https://github.com/example/demo.git",
	"tag": "v1.2.3",
	"build_file": "/absolute/path/to/BUILD.bazel"
}
```

Ausführen:

```bash
python3 tools/add_module.py --registry . --input /path/to/module.json
```

### Interaktiv

```bash
python3 tools/add_module.py --registry .
```

Das Skript berechnet automatisch die Source-Integrität sowie die Hashes für
Overlay- und Patch-Dateien in `source.json`.

![Test Registry](https://github.com/tragisch/bazel-registry/actions/workflows/test-registry.yml/badge.svg)
