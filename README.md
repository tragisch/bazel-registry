## About
Tryout to understand how a bazel registry works.

Bazel-registry for common non-Bazel libraries. 
Build patches currently for MacOS. 

## How to use

```
# make sure bzlmod is enabled
common --enable_bzlmod

# add this bazel registry
common --registry=https://raw.githubusercontent.com/tragisch/bazel-registry/main
# by adding this registry the BCR gets removed so you must add it here as well
common --registry=https://bcr.bazel.build
```
