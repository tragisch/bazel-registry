#include <stdlib.h>

#include "lapacke.h"

static void transpose_copy_float(const float *src, lapack_int src_ld,
                                 float *dst, lapack_int dst_ld,
                                 lapack_int rows, lapack_int cols) {
  for (lapack_int i = 0; i < rows; ++i) {
    for (lapack_int j = 0; j < cols; ++j) {
      dst[j * dst_ld + i] = src[i * src_ld + j];
    }
  }
}

lapack_int LAPACKE_sgesv(int matrix_layout, lapack_int n, lapack_int nrhs,
                         float *a, lapack_int lda, lapack_int *ipiv,
                         float *b, lapack_int ldb) {
  lapack_int info = 0;

  if (matrix_layout == LAPACK_COL_MAJOR) {
    LAPACK_sgesv(&n, &nrhs, a, &lda, ipiv, b, &ldb, &info);
    return info;
  }

  if (matrix_layout != LAPACK_ROW_MAJOR) {
    return -1;
  }

  if (n < 0) return -2;
  if (nrhs < 0) return -3;
  if (lda < (n > 0 ? n : 1)) return -5;
  if (ldb < (nrhs > 0 ? nrhs : 1)) return -7;

  float *a_cm = (float *)malloc((size_t)n * (size_t)n * sizeof(float));
  float *b_cm = (float *)malloc((size_t)n * (size_t)nrhs * sizeof(float));
  if (a_cm == NULL || b_cm == NULL) {
    free(a_cm);
    free(b_cm);
    return LAPACK_WORK_MEMORY_ERROR;
  }

  transpose_copy_float(a, lda, a_cm, n, n, n);
  transpose_copy_float(b, ldb, b_cm, n, n, nrhs);

  LAPACK_sgesv(&n, &nrhs, a_cm, &n, ipiv, b_cm, &n, &info);

  if (info == 0) {
    transpose_copy_float(b_cm, n, b, ldb, nrhs, n);
  }

  free(a_cm);
  free(b_cm);

  return info;
}