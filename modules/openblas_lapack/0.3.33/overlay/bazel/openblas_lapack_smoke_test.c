#include <stdio.h>

#include "cblas.h"

int dgesv(blasint *n, blasint *nrhs, double *a, blasint *lda, blasint *ipiv,
          double *b, blasint *ldb, blasint *info);

int main(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  double a[4] = {3.0, 1.0, 1.0, 2.0};
  double b[2] = {5.0, 5.0};

  dgesv(&n, &nrhs, a, &lda, ipiv, b, &ldb, &info);
  if (info != 0) {
    fprintf(stderr, "dgesv failed with info=%ld\n", (long)info);
    return 1;
  }

  return 0;
}
