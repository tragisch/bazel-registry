## How to use

```
# make sure bzlmod is enabled
common --enable_bzlmod

# add bazelboost registry
common --registry=https://raw.githubusercontent.com/tragisch/bazel-registry/main
# by adding the bazelboost registry the BCR gets removed so you must add it here as well
common --registry=https://bcr.bazel.build
```
