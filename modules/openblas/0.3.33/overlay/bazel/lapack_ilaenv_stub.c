#include "common.h"

typedef int ftnlen;

static int lapack_name_matches(const char *name, ftnlen len,
                               const char *expected) {
  ftnlen i = 0;
  for (; expected[i] != '\0'; ++i) {
    if (i >= len) return 0;
    char c = name[i];
    if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
    if (c != expected[i]) return 0;
  }
  return i == len || name[i] == ' ';
}

static int is_getri_name(const char *name, ftnlen len) {
  return lapack_name_matches(name, len, "SGETRI") ||
         lapack_name_matches(name, len, "DGETRI") ||
         lapack_name_matches(name, len, "CGETRI") ||
         lapack_name_matches(name, len, "ZGETRI");
}

blasint openblas_bazel_ilaenv(blasint *ispec, char *name, char *opts,
                              blasint *n1, blasint *n2, blasint *n3,
                              blasint *n4, ftnlen name_len,
                              ftnlen opts_len) {
  (void)opts;
  (void)n1;
  (void)n2;
  (void)n3;
  (void)n4;
  (void)opts_len;

  if (ispec != 0 && name != 0 && is_getri_name(name, name_len)) {
    if (*ispec == 1) return 2;
    if (*ispec == 2) return 2;
  }

  return 1;
}
