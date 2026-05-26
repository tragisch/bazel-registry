#include <math.h>
#include <stdio.h>
#include <string.h>

#include "cblas.h"

int sgesv(blasint *n, blasint *nrhs, float *a, blasint *lda, blasint *ipiv,
          float *b, blasint *ldb, blasint *info);
int dgesv(blasint *n, blasint *nrhs, double *a, blasint *lda, blasint *ipiv,
          double *b, blasint *ldb, blasint *info);
int sgetrf(blasint *m, blasint *n, float *a, blasint *lda, blasint *ipiv,
           blasint *info);
int dgetrf(blasint *m, blasint *n, double *a, blasint *lda, blasint *ipiv,
           blasint *info);
void sgetri(blasint *n, float *a, blasint *lda, blasint *ipiv, float *work,
            blasint *lwork, blasint *info);
void dgetri(blasint *n, double *a, blasint *lda, blasint *ipiv, double *work,
            blasint *lwork, blasint *info);
int spotrf(char *uplo, blasint *n, float *a, blasint *lda, blasint *info);
int dpotrf(char *uplo, blasint *n, double *a, blasint *lda, blasint *info);
int cgesv(blasint *n, blasint *nrhs, float *a, blasint *lda, blasint *ipiv,
          float *b, blasint *ldb, blasint *info);
int zgesv(blasint *n, blasint *nrhs, double *a, blasint *lda, blasint *ipiv,
          double *b, blasint *ldb, blasint *info);
int cgetrf(blasint *m, blasint *n, float *a, blasint *lda, blasint *ipiv,
           blasint *info);
int zgetrf(blasint *m, blasint *n, double *a, blasint *lda, blasint *ipiv,
           blasint *info);
void cgetri(blasint *n, float *a, blasint *lda, blasint *ipiv, float *work,
            blasint *lwork, blasint *info);
void zgetri(blasint *n, double *a, blasint *lda, blasint *ipiv, double *work,
            blasint *lwork, blasint *info);
int cpotrf(char *uplo, blasint *n, float *a, blasint *lda, blasint *info);
int zpotrf(char *uplo, blasint *n, double *a, blasint *lda, blasint *info);
int spotri(char *uplo, blasint *n, float *a, blasint *lda, blasint *info);
int dpotri(char *uplo, blasint *n, double *a, blasint *lda, blasint *info);
int strtri(char *uplo, char *diag, blasint *n, float *a, blasint *lda,
           blasint *info);
int dtrtri(char *uplo, char *diag, blasint *n, double *a, blasint *lda,
           blasint *info);
int ctrtri(char *uplo, char *diag, blasint *n, float *a, blasint *lda,
           blasint *info);
int ztrtri(char *uplo, char *diag, blasint *n, double *a, blasint *lda,
           blasint *info);
int strtrs(char *uplo, char *trans, char *diag, blasint *n, blasint *nrhs,
           float *a, blasint *lda, float *b, blasint *ldb, blasint *info);
int dtrtrs(char *uplo, char *trans, char *diag, blasint *n, blasint *nrhs,
           double *a, blasint *lda, double *b, blasint *ldb, blasint *info);
int ctrtrs(char *uplo, char *trans, char *diag, blasint *n, blasint *nrhs,
           float *a, blasint *lda, float *b, blasint *ldb, blasint *info);
int ztrtrs(char *uplo, char *trans, char *diag, blasint *n, blasint *nrhs,
           double *a, blasint *lda, double *b, blasint *ldb, blasint *info);
int cpotri(char *uplo, blasint *n, float *a, blasint *lda, blasint *info);
int zpotri(char *uplo, blasint *n, double *a, blasint *lda, blasint *info);

static int near_double(double actual, double expected, double tolerance,
                       const char *label) {
  if (fabs(actual - expected) > tolerance) {
    fprintf(stderr, "%s: expected %.12g, got %.12g\n", label, expected, actual);
    return 0;
  }
  return 1;
}

static int check_inverse_double(const double *a, const double *ainv, blasint n,
                                double tolerance, const char *label) {
  int ok = 1;
  for (blasint col = 0; col < n; ++col) {
    for (blasint row = 0; row < n; ++row) {
      double sum = 0.0;
      for (blasint k = 0; k < n; ++k) {
        sum += a[row + k * n] * ainv[k + col * n];
      }
      double expected = row == col ? 1.0 : 0.0;
      char item[64];
      snprintf(item, sizeof(item), "%s %ld,%ld", label, (long)row,
               (long)col);
      ok &= near_double(sum, expected, tolerance, item);
    }
  }
  return ok;
}

static int check_inverse_float(const float *a, const float *ainv, blasint n,
                               double tolerance, const char *label) {
  int ok = 1;
  for (blasint col = 0; col < n; ++col) {
    for (blasint row = 0; row < n; ++row) {
      double sum = 0.0;
      for (blasint k = 0; k < n; ++k) {
        sum += (double)a[row + k * n] * (double)ainv[k + col * n];
      }
      double expected = row == col ? 1.0 : 0.0;
      char item[64];
      snprintf(item, sizeof(item), "%s %ld,%ld", label, (long)row,
               (long)col);
      ok &= near_double(sum, expected, tolerance, item);
    }
  }
  return ok;
}

static int check_inverse_zdouble(const double *a, const double *ainv,
                                 blasint n, double tolerance,
                                 const char *label) {
  int ok = 1;
  for (blasint col = 0; col < n; ++col) {
    for (blasint row = 0; row < n; ++row) {
      double sum_re = 0.0;
      double sum_im = 0.0;
      for (blasint k = 0; k < n; ++k) {
        blasint ai = 2 * (row + k * n);
        blasint bi = 2 * (k + col * n);
        double ar = a[ai];
        double ai_im = a[ai + 1];
        double br = ainv[bi];
        double bi_im = ainv[bi + 1];
        sum_re += ar * br - ai_im * bi_im;
        sum_im += ar * bi_im + ai_im * br;
      }
      double expected = row == col ? 1.0 : 0.0;
      char item[64];
      snprintf(item, sizeof(item), "%s re %ld,%ld", label, (long)row,
               (long)col);
      ok &= near_double(sum_re, expected, tolerance, item);
      snprintf(item, sizeof(item), "%s im %ld,%ld", label, (long)row,
               (long)col);
      ok &= near_double(sum_im, 0.0, tolerance, item);
    }
  }
  return ok;
}

static int check_inverse_cfloat(const float *a, const float *ainv, blasint n,
                                double tolerance, const char *label) {
  int ok = 1;
  for (blasint col = 0; col < n; ++col) {
    for (blasint row = 0; row < n; ++row) {
      double sum_re = 0.0;
      double sum_im = 0.0;
      for (blasint k = 0; k < n; ++k) {
        blasint ai = 2 * (row + k * n);
        blasint bi = 2 * (k + col * n);
        double ar = a[ai];
        double ai_im = a[ai + 1];
        double br = ainv[bi];
        double bi_im = ainv[bi + 1];
        sum_re += ar * br - ai_im * bi_im;
        sum_im += ar * bi_im + ai_im * br;
      }
      double expected = row == col ? 1.0 : 0.0;
      char item[64];
      snprintf(item, sizeof(item), "%s re %ld,%ld", label, (long)row,
               (long)col);
      ok &= near_double(sum_re, expected, tolerance, item);
      snprintf(item, sizeof(item), "%s im %ld,%ld", label, (long)row,
               (long)col);
      ok &= near_double(sum_im, 0.0, tolerance, item);
    }
  }
  return ok;
}

static int test_dgesv(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  double a[4] = {3.0, 1.0, 1.0, 2.0};
  double b[2] = {5.0, 5.0};

  dgesv(&n, &nrhs, a, &lda, ipiv, b, &ldb, &info);

  return info == 0 && near_double(b[0], 1.0, 1e-12, "dgesv x0") &&
         near_double(b[1], 2.0, 1e-12, "dgesv x1");
}

static int test_sgesv(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  float a[4] = {3.0f, 1.0f, 1.0f, 2.0f};
  float b[2] = {5.0f, 5.0f};

  sgesv(&n, &nrhs, a, &lda, ipiv, b, &ldb, &info);

  return info == 0 && near_double(b[0], 1.0, 1e-5, "sgesv x0") &&
         near_double(b[1], 2.0, 1e-5, "sgesv x1");
}

static int test_dpotrf(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  double a[4] = {4.0, 2.0, 2.0, 3.0};

  dpotrf(&uplo, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 2.0, 1e-12, "dpotrf l00") &&
         near_double(a[1], 1.0, 1e-12, "dpotrf l10") &&
         near_double(a[3], sqrt(2.0), 1e-12, "dpotrf l11");
}

static int test_spotrf(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  float a[4] = {4.0f, 2.0f, 2.0f, 3.0f};

  spotrf(&uplo, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 2.0, 1e-5, "spotrf l00") &&
         near_double(a[1], 1.0, 1e-5, "spotrf l10") &&
         near_double(a[3], sqrt(2.0), 1e-5, "spotrf l11");
}

static int test_zgesv(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  double a[8] = {1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 2.0, -1.0};
  double b[4] = {2.0, 0.0, 5.0, 0.0};

  zgesv(&n, &nrhs, a, &lda, ipiv, b, &ldb, &info);

  return info == 0 && near_double(b[0], 1.0, 1e-12, "zgesv x0 re") &&
         near_double(b[1], -1.0, 1e-12, "zgesv x0 im") &&
         near_double(b[2], 2.0, 1e-12, "zgesv x1 re") &&
         near_double(b[3], 1.0, 1e-12, "zgesv x1 im");
}

static int test_cgesv(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  float a[8] = {1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, -1.0f};
  float b[4] = {2.0f, 0.0f, 5.0f, 0.0f};

  cgesv(&n, &nrhs, a, &lda, ipiv, b, &ldb, &info);

  return info == 0 && near_double(b[0], 1.0, 1e-5, "cgesv x0 re") &&
         near_double(b[1], -1.0, 1e-5, "cgesv x0 im") &&
         near_double(b[2], 2.0, 1e-5, "cgesv x1 re") &&
         near_double(b[3], 1.0, 1e-5, "cgesv x1 im");
}

static int test_zpotrf(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  double a[8] = {4.0, 0.0, 1.0, -1.0, 1.0, 1.0, 3.0, 0.0};

  zpotrf(&uplo, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 2.0, 1e-12, "zpotrf l00 re") &&
         near_double(a[1], 0.0, 1e-12, "zpotrf l00 im") &&
         near_double(a[2], 0.5, 1e-12, "zpotrf l10 re") &&
         near_double(a[3], -0.5, 1e-12, "zpotrf l10 im") &&
         near_double(a[6], sqrt(2.5), 1e-12, "zpotrf l11 re") &&
         near_double(a[7], 0.0, 1e-12, "zpotrf l11 im");
}

static int test_cpotrf(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  float a[8] = {4.0f, 0.0f, 1.0f, -1.0f, 1.0f, 1.0f, 3.0f, 0.0f};

  cpotrf(&uplo, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 2.0, 1e-5, "cpotrf l00 re") &&
         near_double(a[1], 0.0, 1e-5, "cpotrf l00 im") &&
         near_double(a[2], 0.5, 1e-5, "cpotrf l10 re") &&
         near_double(a[3], -0.5, 1e-5, "cpotrf l10 im") &&
         near_double(a[6], sqrt(2.5), 1e-5, "cpotrf l11 re") &&
         near_double(a[7], 0.0, 1e-5, "cpotrf l11 im");
}

static int test_ztrtrs(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  char uplo = 'L';
  char trans = 'N';
  char diag = 'N';
  double a[8] = {2.0, 0.0, 1.0, -1.0, 0.0, 0.0, 3.0, 0.0};
  double b[4] = {2.0, -2.0, 6.0, 1.0};

  ztrtrs(&uplo, &trans, &diag, &n, &nrhs, a, &lda, b, &ldb, &info);

  return info == 0 && near_double(b[0], 1.0, 1e-12, "ztrtrs x0 re") &&
         near_double(b[1], -1.0, 1e-12, "ztrtrs x0 im") &&
         near_double(b[2], 2.0, 1e-12, "ztrtrs x1 re") &&
         near_double(b[3], 1.0, 1e-12, "ztrtrs x1 im");
}

static int test_ctrtrs(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  char uplo = 'L';
  char trans = 'N';
  char diag = 'N';
  float a[8] = {2.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 3.0f, 0.0f};
  float b[4] = {2.0f, -2.0f, 6.0f, 1.0f};

  ctrtrs(&uplo, &trans, &diag, &n, &nrhs, a, &lda, b, &ldb, &info);

  return info == 0 && near_double(b[0], 1.0, 1e-5, "ctrtrs x0 re") &&
         near_double(b[1], -1.0, 1e-5, "ctrtrs x0 im") &&
         near_double(b[2], 2.0, 1e-5, "ctrtrs x1 re") &&
         near_double(b[3], 1.0, 1e-5, "ctrtrs x1 im");
}

static int test_ztrtri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  char diag = 'N';
  double a[8] = {2.0, 0.0, 1.0, -1.0, 0.0, 0.0, 3.0, 0.0};

  ztrtri(&uplo, &diag, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 0.5, 1e-12, "ztrtri l00 re") &&
         near_double(a[1], 0.0, 1e-12, "ztrtri l00 im") &&
         near_double(a[2], -1.0 / 6.0, 1e-12, "ztrtri l10 re") &&
         near_double(a[3], 1.0 / 6.0, 1e-12, "ztrtri l10 im") &&
         near_double(a[6], 1.0 / 3.0, 1e-12, "ztrtri l11 re") &&
         near_double(a[7], 0.0, 1e-12, "ztrtri l11 im");
}

static int test_ctrtri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  char diag = 'N';
  float a[8] = {2.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 3.0f, 0.0f};

  ctrtri(&uplo, &diag, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 0.5, 1e-5, "ctrtri l00 re") &&
         near_double(a[1], 0.0, 1e-5, "ctrtri l00 im") &&
         near_double(a[2], -1.0 / 6.0, 1e-5, "ctrtri l10 re") &&
         near_double(a[3], 1.0 / 6.0, 1e-5, "ctrtri l10 im") &&
         near_double(a[6], 1.0 / 3.0, 1e-5, "ctrtri l11 re") &&
         near_double(a[7], 0.0, 1e-5, "ctrtri l11 im");
}

static int test_zpotri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  double a[8] = {4.0, 0.0, 1.0, -1.0, 1.0, 1.0, 3.0, 0.0};

  zpotrf(&uplo, &n, a, &lda, &info);
  if (info != 0) return 0;
  zpotri(&uplo, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 0.3, 1e-12, "zpotri a00 re") &&
         near_double(a[1], 0.0, 1e-12, "zpotri a00 im") &&
         near_double(a[2], -0.1, 1e-12, "zpotri a10 re") &&
         near_double(a[3], 0.1, 1e-12, "zpotri a10 im") &&
         near_double(a[6], 0.4, 1e-12, "zpotri a11 re") &&
         near_double(a[7], 0.0, 1e-12, "zpotri a11 im");
}

static int test_cpotri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  float a[8] = {4.0f, 0.0f, 1.0f, -1.0f, 1.0f, 1.0f, 3.0f, 0.0f};

  cpotrf(&uplo, &n, a, &lda, &info);
  if (info != 0) return 0;
  cpotri(&uplo, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 0.3, 1e-5, "cpotri a00 re") &&
         near_double(a[1], 0.0, 1e-5, "cpotri a00 im") &&
         near_double(a[2], -0.1, 1e-5, "cpotri a10 re") &&
         near_double(a[3], 0.1, 1e-5, "cpotri a10 im") &&
         near_double(a[6], 0.4, 1e-5, "cpotri a11 re") &&
         near_double(a[7], 0.0, 1e-5, "cpotri a11 im");
}

static int test_dtrtrs(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  char uplo = 'L';
  char trans = 'N';
  char diag = 'N';
  double a[4] = {2.0, 1.0, 0.0, 3.0};
  double b[2] = {2.0, 7.0};

  dtrtrs(&uplo, &trans, &diag, &n, &nrhs, a, &lda, b, &ldb, &info);

  return info == 0 && near_double(b[0], 1.0, 1e-12, "dtrtrs x0") &&
         near_double(b[1], 2.0, 1e-12, "dtrtrs x1");
}

static int test_strtrs(void) {
  blasint n = 2;
  blasint nrhs = 1;
  blasint lda = 2;
  blasint ldb = 2;
  blasint info = -1;
  char uplo = 'L';
  char trans = 'N';
  char diag = 'N';
  float a[4] = {2.0f, 1.0f, 0.0f, 3.0f};
  float b[2] = {2.0f, 7.0f};

  strtrs(&uplo, &trans, &diag, &n, &nrhs, a, &lda, b, &ldb, &info);

  return info == 0 && near_double(b[0], 1.0, 1e-5, "strtrs x0") &&
         near_double(b[1], 2.0, 1e-5, "strtrs x1");
}

static int test_dtrtri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  char diag = 'N';
  double a[4] = {2.0, 1.0, 0.0, 3.0};

  dtrtri(&uplo, &diag, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 0.5, 1e-12, "dtrtri l00") &&
         near_double(a[1], -1.0 / 6.0, 1e-12, "dtrtri l10") &&
         near_double(a[3], 1.0 / 3.0, 1e-12, "dtrtri l11");
}

static int test_strtri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  char diag = 'N';
  float a[4] = {2.0f, 1.0f, 0.0f, 3.0f};

  strtri(&uplo, &diag, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 0.5, 1e-5, "strtri l00") &&
         near_double(a[1], -1.0 / 6.0, 1e-5, "strtri l10") &&
         near_double(a[3], 1.0 / 3.0, 1e-5, "strtri l11");
}

static int test_dpotri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  double a[4] = {4.0, 2.0, 2.0, 3.0};

  dpotrf(&uplo, &n, a, &lda, &info);
  if (info != 0) return 0;
  dpotri(&uplo, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 0.375, 1e-12, "dpotri a00") &&
         near_double(a[1], -0.25, 1e-12, "dpotri a10") &&
         near_double(a[3], 0.5, 1e-12, "dpotri a11");
}

static int test_spotri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  char uplo = 'L';
  float a[4] = {4.0f, 2.0f, 2.0f, 3.0f};

  spotrf(&uplo, &n, a, &lda, &info);
  if (info != 0) return 0;
  spotri(&uplo, &n, a, &lda, &info);

  return info == 0 && near_double(a[0], 0.375, 1e-5, "spotri a00") &&
         near_double(a[1], -0.25, 1e-5, "spotri a10") &&
         near_double(a[3], 0.5, 1e-5, "spotri a11");
}

static int test_dgetri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  blasint lwork = 4;
  double work[4] = {0.0, 0.0, 0.0, 0.0};
  double a[4] = {1.0, 3.0, 2.0, 4.0};

  dgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) return 0;
  dgetri(&n, a, &lda, ipiv, work, &lwork, &info);

  return info == 0 && near_double(a[0], -2.0, 1e-12, "dgetri a00") &&
         near_double(a[1], 1.5, 1e-12, "dgetri a10") &&
         near_double(a[2], 1.0, 1e-12, "dgetri a01") &&
         near_double(a[3], -0.5, 1e-12, "dgetri a11");
}

static int test_sgetri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  blasint lwork = 4;
  float work[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  float a[4] = {1.0f, 3.0f, 2.0f, 4.0f};

  sgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) return 0;
  sgetri(&n, a, &lda, ipiv, work, &lwork, &info);

  return info == 0 && near_double(a[0], -2.0, 1e-5, "sgetri a00") &&
         near_double(a[1], 1.5, 1e-5, "sgetri a10") &&
         near_double(a[2], 1.0, 1e-5, "sgetri a01") &&
         near_double(a[3], -0.5, 1e-5, "sgetri a11");
}

static int test_zgetri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  blasint lwork = 4;
  double work[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  double a[8] = {1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 2.0, -1.0};

  zgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) return 0;
  zgetri(&n, a, &lda, ipiv, work, &lwork, &info);

  return info == 0 && near_double(a[0], 0.5, 1e-12, "zgetri a00 re") &&
         near_double(a[1], -0.5, 1e-12, "zgetri a00 im") &&
         near_double(a[2], 0.0, 1e-12, "zgetri a10 re") &&
         near_double(a[3], 0.0, 1e-12, "zgetri a10 im") &&
         near_double(a[4], 0.0, 1e-12, "zgetri a01 re") &&
         near_double(a[5], 0.0, 1e-12, "zgetri a01 im") &&
         near_double(a[6], 0.4, 1e-12, "zgetri a11 re") &&
         near_double(a[7], 0.2, 1e-12, "zgetri a11 im");
}

static int test_cgetri(void) {
  blasint n = 2;
  blasint lda = 2;
  blasint info = -1;
  blasint ipiv[2] = {0, 0};
  blasint lwork = 4;
  float work[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  float a[8] = {1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, -1.0f};

  cgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) return 0;
  cgetri(&n, a, &lda, ipiv, work, &lwork, &info);

  return info == 0 && near_double(a[0], 0.5, 1e-5, "cgetri a00 re") &&
         near_double(a[1], -0.5, 1e-5, "cgetri a00 im") &&
         near_double(a[2], 0.0, 1e-5, "cgetri a10 re") &&
         near_double(a[3], 0.0, 1e-5, "cgetri a10 im") &&
         near_double(a[4], 0.0, 1e-5, "cgetri a01 re") &&
         near_double(a[5], 0.0, 1e-5, "cgetri a01 im") &&
         near_double(a[6], 0.4, 1e-5, "cgetri a11 re") &&
         near_double(a[7], 0.2, 1e-5, "cgetri a11 im");
}

static int test_dgetri_blocked(void) {
  blasint n = 4;
  blasint lda = 4;
  blasint info = -1;
  blasint ipiv[4] = {0, 0, 0, 0};
  blasint lwork = 8;
  double work[8] = {0.0};
  double a[16] = {4.0,   1.0,   0.5,  -0.25, 0.25, 5.0,   -0.5, 0.75,
                  -0.5,  0.25,  6.0,  1.0,   0.125, -0.25, 0.5, 7.0};
  double original[16];
  memcpy(original, a, sizeof(a));

  dgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) return 0;
  dgetri(&n, a, &lda, ipiv, work, &lwork, &info);

  return info == 0 && near_double(work[0], 8.0, 1e-12, "dgetri lwork") &&
         check_inverse_double(original, a, n, 1e-11, "dgetri blocked");
}

static int test_sgetri_blocked(void) {
  blasint n = 4;
  blasint lda = 4;
  blasint info = -1;
  blasint ipiv[4] = {0, 0, 0, 0};
  blasint lwork = 8;
  float work[8] = {0.0f};
  float a[16] = {4.0f,   1.0f,   0.5f,  -0.25f, 0.25f, 5.0f,
                 -0.5f,  0.75f,  -0.5f, 0.25f,  6.0f,  1.0f,
                 0.125f, -0.25f, 0.5f,  7.0f};
  float original[16];
  memcpy(original, a, sizeof(a));

  sgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) return 0;
  sgetri(&n, a, &lda, ipiv, work, &lwork, &info);

  return info == 0 && near_double(work[0], 8.0, 1e-5, "sgetri lwork") &&
         check_inverse_float(original, a, n, 1e-4, "sgetri blocked");
}

static int test_zgetri_blocked(void) {
  blasint n = 4;
  blasint lda = 4;
  blasint info = -1;
  blasint ipiv[4] = {0, 0, 0, 0};
  blasint lwork = 8;
  double work[16] = {0.0};
  double a[32] = {
      4.0,   1.0,   1.0,  0.25, 0.5,  -0.25, -0.25, 0.0,
      0.25,  -0.5,  5.0,  -1.0, -0.5, 0.0,   0.75,  0.25,
      -0.5,  0.25,  0.25, 0.5,  6.0,  0.5,   1.0,   -0.5,
      0.125, 0.0,   -0.25, 0.25, 0.5, -1.0,  7.0,   -0.25};
  double original[32];
  memcpy(original, a, sizeof(a));

  zgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) return 0;
  zgetri(&n, a, &lda, ipiv, work, &lwork, &info);

  return info == 0 && near_double(work[0], 8.0, 1e-12, "zgetri lwork re") &&
         near_double(work[1], 0.0, 1e-12, "zgetri lwork im") &&
         check_inverse_zdouble(original, a, n, 1e-11, "zgetri blocked");
}

static int test_cgetri_blocked(void) {
  blasint n = 4;
  blasint lda = 4;
  blasint info = -1;
  blasint ipiv[4] = {0, 0, 0, 0};
  blasint lwork = 8;
  float work[16] = {0.0f};
  float a[32] = {
      4.0f,   1.0f,   1.0f,  0.25f, 0.5f,  -0.25f, -0.25f, 0.0f,
      0.25f,  -0.5f,  5.0f,  -1.0f, -0.5f, 0.0f,   0.75f,  0.25f,
      -0.5f,  0.25f,  0.25f, 0.5f,  6.0f,  0.5f,   1.0f,   -0.5f,
      0.125f, 0.0f,   -0.25f, 0.25f, 0.5f, -1.0f,  7.0f,   -0.25f};
  float original[32];
  memcpy(original, a, sizeof(a));

  cgetrf(&n, &n, a, &lda, ipiv, &info);
  if (info != 0) return 0;
  cgetri(&n, a, &lda, ipiv, work, &lwork, &info);

  return info == 0 && near_double(work[0], 8.0, 1e-5, "cgetri lwork re") &&
         near_double(work[1], 0.0, 1e-5, "cgetri lwork im") &&
         check_inverse_cfloat(original, a, n, 1e-4, "cgetri blocked");
}

int main(void) {
  int ok = 1;

  ok &= test_dgesv();
  ok &= test_sgesv();
  ok &= test_dpotrf();
  ok &= test_spotrf();
  ok &= test_zgesv();
  ok &= test_cgesv();
  ok &= test_zpotrf();
  ok &= test_cpotrf();
  ok &= test_ztrtrs();
  ok &= test_ctrtrs();
  ok &= test_ztrtri();
  ok &= test_ctrtri();
  ok &= test_zpotri();
  ok &= test_cpotri();
  ok &= test_dtrtrs();
  ok &= test_strtrs();
  ok &= test_dtrtri();
  ok &= test_strtri();
  ok &= test_dpotri();
  ok &= test_spotri();
  ok &= test_dgetri();
  ok &= test_sgetri();
  ok &= test_zgetri();
  ok &= test_cgetri();
  ok &= test_dgetri_blocked();
  ok &= test_sgetri_blocked();
  ok &= test_zgetri_blocked();
  ok &= test_cgetri_blocked();

  if (!ok) return 1;

  puts("LAPACK smoke tests passed");
  return 0;
}
