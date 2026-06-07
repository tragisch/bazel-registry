#ifndef OPENBLAS_LAPACKE_MINIMAL_H_
#define OPENBLAS_LAPACKE_MINIMAL_H_

#include "cblas.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef blasint lapack_int;

#define LAPACK_ROW_MAJOR 101
#define LAPACK_COL_MAJOR 102

#ifndef LAPACK_WORK_MEMORY_ERROR
#define LAPACK_WORK_MEMORY_ERROR -1010
#endif

int sgesv_(lapack_int *n, lapack_int *nrhs, float *a, lapack_int *lda,
		   lapack_int *ipiv, float *b, lapack_int *ldb, lapack_int *info);
int sgetrf_(lapack_int *m, lapack_int *n, float *a, lapack_int *lda,
			lapack_int *ipiv, lapack_int *info);
int sgetri_(lapack_int *n, float *a, lapack_int *lda, lapack_int *ipiv,
			float *work, lapack_int *lwork, lapack_int *info);
int sgeqrf_(lapack_int *m, lapack_int *n, float *a, lapack_int *lda,
			float *tau, float *work, lapack_int *lwork, lapack_int *info);

#define LAPACK_sgesv sgesv_
#define LAPACK_sgetrf sgetrf_
#define LAPACK_sgetri sgetri_
#define LAPACK_sgeqrf sgeqrf_

lapack_int LAPACKE_sgesv(int matrix_layout, lapack_int n, lapack_int nrhs,
						 float *a, lapack_int lda, lapack_int *ipiv,
						 float *b, lapack_int ldb);

#ifdef __cplusplus
}
#endif

#endif  // OPENBLAS_LAPACKE_MINIMAL_H_