/* Minimal LAPACK block-size query used by the Bazel LAPACK wrapper layer. */

#include <ctype.h>

#include "common.h"

typedef blasint integer;
typedef int ftnlen;

static int is_prefix(const char *name, const char *prefix) {
  while (*prefix != '\0') {
    char a = (char)toupper((unsigned char)*name++);
    char b = (char)toupper((unsigned char)*prefix++);
    if (a != b) return 0;
  }
  return 1;
}

integer ilaenv_(integer *ispec, char *name__, char *opts,
                integer *n1, integer *n2, integer *n3, integer *n4,
                ftnlen name_len, ftnlen opts_len) {
  (void)opts;
  (void)n1;
  (void)n2;
  (void)n3;
  (void)n4;
  (void)name_len;
  (void)opts_len;

  if (*ispec == 1) {
    if (is_prefix(name__, "SGETRI") || is_prefix(name__, "SGEQRF")) {
      return 1;
    }
  }

  if (*ispec == 2) {
    return 2;
  }

  return 1;
}