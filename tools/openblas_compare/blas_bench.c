#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cblas.h"
#include "simple_bench.h"

#ifndef BLAS_PROVIDER
#define BLAS_PROVIDER "unknown"
#endif

typedef struct {
  int m;
  int n;
  int k;
  float *a;
  float *b;
  float *c;
  double *ad;
  double *bd;
  double *cd;
} bench_buffers;

static int current_m = 0;
static int current_n = 0;
static int current_k = 0;
static bench_buffers buffers = {0};

static void *checked_malloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    fprintf(stderr, "malloc failed for %zu bytes\n", size);
    exit(1);
  }
  return ptr;
}

static void fill_float_buffer(float *buf, size_t count, float scale) {
  size_t i;
  for (i = 0; i < count; ++i) {
    buf[i] = scale * (float)((i % 17) - 8) / 7.0f;
  }
}

static void fill_double_buffer(double *buf, size_t count, double scale) {
  size_t i;
  for (i = 0; i < count; ++i) {
    buf[i] = scale * (double)((i % 23) - 11) / 9.0;
  }
}

static void ensure_buffers(int m, int n, int k) {
  size_t a_count;
  size_t b_count;
  size_t c_count;

  if (current_m == m && current_n == n && current_k == k) {
    return;
  }

  free(buffers.a);
  free(buffers.b);
  free(buffers.c);
  free(buffers.ad);
  free(buffers.bd);
  free(buffers.cd);

  current_m = m;
  current_n = n;
  current_k = k;

  a_count = (size_t)m * (size_t)k;
  b_count = (size_t)k * (size_t)n;
  c_count = (size_t)m * (size_t)n;

  buffers.m = m;
  buffers.n = n;
  buffers.k = k;
  buffers.a = checked_malloc(sizeof(float) * a_count);
  buffers.b = checked_malloc(sizeof(float) * b_count);
  buffers.c = checked_malloc(sizeof(float) * c_count);
  buffers.ad = checked_malloc(sizeof(double) * a_count);
  buffers.bd = checked_malloc(sizeof(double) * b_count);
  buffers.cd = checked_malloc(sizeof(double) * c_count);

  fill_float_buffer(buffers.a, a_count, 1.0f);
  fill_float_buffer(buffers.b, b_count, 0.5f);
  fill_double_buffer(buffers.ad, a_count, 1.0);
  fill_double_buffer(buffers.bd, b_count, 0.5);
}

static void run_sgemm_case(void) {
  memset(buffers.c, 0, sizeof(float) * (size_t)buffers.m * (size_t)buffers.n);
  cblas_sgemm(
      CblasRowMajor,
      CblasNoTrans,
      CblasNoTrans,
      buffers.m,
      buffers.n,
      buffers.k,
      1.0f,
      buffers.a,
      buffers.k,
      buffers.b,
      buffers.n,
      0.0f,
      buffers.c,
      buffers.n);
}

static void run_dgemm_case(void) {
  memset(buffers.cd, 0, sizeof(double) * (size_t)buffers.m * (size_t)buffers.n);
  cblas_dgemm(
      CblasRowMajor,
      CblasNoTrans,
      CblasNoTrans,
      buffers.m,
      buffers.n,
      buffers.k,
      1.0,
      buffers.ad,
      buffers.k,
      buffers.bd,
      buffers.n,
      0.0,
      buffers.cd,
      buffers.n);
}

static void print_case_header(const char *name, int m, int n, int k) {
  printf("provider=%s benchmark=%s size=%dx%dx%d threads=%s omp_threads=%s\n",
         BLAS_PROVIDER,
         name,
         m,
         n,
         k,
         getenv("OPENBLAS_NUM_THREADS") ? getenv("OPENBLAS_NUM_THREADS") : "unset",
         getenv("OMP_NUM_THREADS") ? getenv("OMP_NUM_THREADS") : "unset");
}

static void benchmark_sgemm(int m, int n, int k, int runs, int warmup) {
  double flops = 2.0 * (double)m * (double)n * (double)k;
  ensure_buffers(m, n, k);
  print_case_header("sgemm", m, n, k);
  BENCH_THROUGHPUT(run_sgemm_case(), flops, "flop", runs, warmup);
}

static void benchmark_dgemm(int m, int n, int k, int runs, int warmup) {
  double flops = 2.0 * (double)m * (double)n * (double)k;
  ensure_buffers(m, n, k);
  print_case_header("dgemm", m, n, k);
  BENCH_THROUGHPUT(run_dgemm_case(), flops, "flop", runs, warmup);
}

int main(void) {
  int runs = 12;
  int warmup = 2;

  print_environment_info();
  benchmark_sgemm(512, 512, 512, runs, warmup);
  benchmark_sgemm(1024, 1024, 1024, runs, warmup);
  benchmark_sgemm(2048, 2048, 2048, runs, warmup);
  benchmark_dgemm(512, 512, 512, runs, warmup);
  benchmark_dgemm(1024, 1024, 1024, runs, warmup);
  benchmark_dgemm(2048, 2048, 2048, runs, warmup);

  free(buffers.a);
  free(buffers.b);
  free(buffers.c);
  free(buffers.ad);
  free(buffers.bd);
  free(buffers.cd);
  return 0;
}
