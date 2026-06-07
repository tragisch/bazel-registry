#include <math.h>
#include <stdio.h>

#include "lapacke.h"

static int nearf(float a, float b) {
  return fabsf(a - b) < 1e-4f;
}

static int test_lapacke_sgesv(void) {
  float a[] = {
      3.0f, 1.0f,
      1.0f, 2.0f,
  };
  float b[] = {9.0f, 8.0f};
  lapack_int ipiv[2] = {0, 0};

  lapack_int info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, 2, 1, a, 2, ipiv, b, 1);
  if (info != 0) {
    fprintf(stderr, "LAPACKE_sgesv failed: %ld\n", (long)info);
    return 1;
  }

  if (!nearf(b[0], 2.0f) || !nearf(b[1], 3.0f)) {
    fprintf(stderr, "LAPACKE_sgesv returned wrong solution: %f %f\n", b[0], b[1]);
    return 1;
  }

  return 0;
}

static int test_sgetri(void) {
  float a[] = {
      4.0f, 2.0f,
      7.0f, 6.0f,
  };
  lapack_int ipiv[2] = {0, 0};
  lapack_int info = 0;
  lapack_int n = 2;
  lapack_int lda = 2;
  lapack_int lwork = 8;
  float work[8];

  LAPACK_sgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) {
    fprintf(stderr, "sgetrf failed: %ld\n", (long)info);
    return 1;
  }

  LAPACK_sgetri(&n, a, &lda, ipiv, work, &lwork, &info);
  if (info != 0) {
    fprintf(stderr, "sgetri failed: %ld\n", (long)info);
    return 1;
  }

  if (!nearf(a[0], 0.6f) || !nearf(a[1], -0.2f) ||
      !nearf(a[2], -0.7f) || !nearf(a[3], 0.4f)) {
    fprintf(stderr, "sgetri returned wrong inverse\n");
    return 1;
  }

  return 0;
}

static int test_sgeqrf(void) {
  float a[] = {
      1.0f, 3.0f,
      2.0f, 4.0f,
  };
  lapack_int info = 0;
  lapack_int m = 2;
  lapack_int n = 2;
  lapack_int lda = 2;
  lapack_int lwork = -1;
  float tau[2] = {0.0f, 0.0f};
  float work_query = 0.0f;

  LAPACK_sgeqrf(&m, &n, a, &lda, tau, &work_query, &lwork, &info);
  if (info != 0 || work_query <= 0.0f) {
    fprintf(stderr, "sgeqrf workspace query failed: %ld, %f\n", (long)info, work_query);
    return 1;
  }

  lwork = 8;
  float work[8];
  LAPACK_sgeqrf(&m, &n, a, &lda, tau, work, &lwork, &info);
  if (info != 0) {
    fprintf(stderr, "sgeqrf factorization failed: %ld\n", (long)info);
    return 1;
  }

  return 0;
}

int main(void) {
  if (test_lapacke_sgesv() != 0) return 1;
  if (test_sgetri() != 0) return 1;
  if (test_sgeqrf() != 0) return 1;

  puts("openblas_lapack smoke test passed");
  return 0;
}
