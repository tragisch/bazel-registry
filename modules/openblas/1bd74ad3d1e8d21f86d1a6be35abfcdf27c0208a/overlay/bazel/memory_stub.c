#include <stdlib.h>

int blas_num_threads = 1;

int openblas_block_factor(void) { return 1; }

void *blas_memory_alloc(int procpos) {
  (void)procpos;
  return malloc(65536);
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
