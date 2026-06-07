#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "cblas.h"
#include "common.h"

static int lapack_lsame_char(char a, char b) {
  return toupper((unsigned char)a) == toupper((unsigned char)b);
}

static CBLAS_UPLO lapack_cblas_uplo(char uplo) {
  return lapack_lsame_char(uplo, 'U') ? CblasUpper : CblasLower;
}

static CBLAS_TRANSPOSE lapack_cblas_transpose(char trans) {
  if (lapack_lsame_char(trans, 'N')) {
    return CblasNoTrans;
  }
  if (lapack_lsame_char(trans, 'T')) {
    return CblasTrans;
  }
  return CblasConjTrans;
}

static CBLAS_DIAG lapack_cblas_diag(char diag) {
  return lapack_lsame_char(diag, 'U') ? CblasUnit : CblasNonUnit;
}

static CBLAS_SIDE lapack_cblas_side(char side) {
  return lapack_lsame_char(side, 'L') ? CblasLeft : CblasRight;
}

void scopy(blasint *n, float *sx, blasint *incx, float *sy, blasint *incy) {
  cblas_scopy(*n, sx, *incx, sy, *incy);
}

void sscal(blasint *n, float *sa, float *sx, blasint *incx) {
  cblas_sscal(*n, *sa, sx, *incx);
}

float snrm2(blasint *n, float *x, blasint *incx) {
  return cblas_snrm2(*n, x, *incx);
}

void sger(blasint *m, blasint *n, float *alpha, float *x, blasint *incx,
          float *y, blasint *incy, float *a, blasint *lda) {
  cblas_sger(CblasColMajor, *m, *n, *alpha, x, *incx, y, *incy, a, *lda);
}

void strmm(char *side, char *uplo, char *transa, char *diag,
           blasint *m, blasint *n, float *alpha,
           float *a, blasint *lda, float *b, blasint *ldb) {
  cblas_strmm(CblasColMajor, lapack_cblas_side(*side), lapack_cblas_uplo(*uplo),
              lapack_cblas_transpose(*transa), lapack_cblas_diag(*diag),
              *m, *n, *alpha, a, *lda, b, *ldb);
}

blasint isamax_(blasint *n, float *sx, blasint *incx) {
  if (*n <= 0 || *incx <= 0) {
    return 0;
  }

  blasint best = 0;
  float max_abs = fabsf(sx[0]);
  blasint ix = *incx;

  for (blasint i = 1; i < *n; ++i, ix += *incx) {
    float v = fabsf(sx[ix]);
    if (v > max_abs || isnan(v)) {
      max_abs = v;
      best = i;
    }
  }

  return best + 1;
}

void strmv_(char *uplo, char *trans, char *diag,
            blasint *n, float *a, blasint *lda,
            float *x, blasint *incx) {
  if (*n <= 0 || *incx <= 0) {
    return;
  }

  const int upper = lapack_lsame_char(*uplo, 'U');
  const int notrans = lapack_lsame_char(*trans, 'N');
  const int nounit = lapack_lsame_char(*diag, 'N');

  float *out = (float *)malloc((size_t)(*n) * sizeof(float));
  if (out == NULL) {
    return;
  }

  for (blasint i = 0; i < *n; ++i) {
    float sum = 0.0f;

    if (notrans) {
      blasint j_begin = upper ? i : 0;
      blasint j_end = upper ? *n : i + 1;
      for (blasint j = j_begin; j < j_end; ++j) {
        float aij = (i == j && !nounit) ? 1.0f : a[i + j * (*lda)];
        sum += aij * x[j * (*incx)];
      }
    } else {
      blasint j_begin = upper ? 0 : i;
      blasint j_end = upper ? i + 1 : *n;
      for (blasint j = j_begin; j < j_end; ++j) {
        float aji = (i == j && !nounit) ? 1.0f : a[j + i * (*lda)];
        sum += aji * x[j * (*incx)];
      }
    }

    out[i] = sum;
  }

  for (blasint i = 0; i < *n; ++i) {
    x[i * (*incx)] = out[i];
  }

  free(out);
}