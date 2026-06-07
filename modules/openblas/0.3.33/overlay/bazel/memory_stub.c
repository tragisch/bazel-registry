#include <stdlib.h>

#include <omp.h>

#include "common.h"

extern void openblas_read_env(void);
extern int openblas_num_threads_env(void);
extern int openblas_omp_num_threads_env(void);

int blas_cpu_number = 0;
int blas_num_threads = 0;
int blas_omp_linked = 1;

static int openblas_bazel_threads_initialized = 0;

static void openblas_bazel_init_threads(void) {
  int configured_threads;

  if (openblas_bazel_threads_initialized) {
    return;
  }

  openblas_read_env();

  configured_threads = openblas_num_threads_env();
  if (configured_threads <= 0) {
    configured_threads = openblas_omp_num_threads_env();
  }
  if (configured_threads <= 0) {
    configured_threads = omp_get_max_threads();
  }
  if (configured_threads < 1) {
    configured_threads = 1;
  }
  if (configured_threads > MAX_CPU_NUMBER) {
    configured_threads = MAX_CPU_NUMBER;
  }

  blas_num_threads = configured_threads;
  blas_cpu_number = configured_threads;
  omp_set_num_threads(configured_threads);

  openblas_bazel_threads_initialized = 1;
}

int blas_get_cpu_number(void) {
  openblas_bazel_init_threads();
  return blas_cpu_number;
}

void *blas_memory_alloc(int procpos) {
  (void)procpos;
  openblas_bazel_init_threads();
  return malloc(BUFFER_SIZE);
}

void blas_memory_free(void *buffer) {
  free(buffer);
}

void *blas_memory_alloc_nolock(int procpos) {
  return blas_memory_alloc(procpos);
}

void blas_memory_free_nolock(void *buffer) {
  blas_memory_free(buffer);
}
