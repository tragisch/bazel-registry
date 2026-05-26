#include <math.h>
#include <stdio.h>

#include "cblas.h"

static int close_double(double actual, double expected) {
  return fabs(actual - expected) <= 1e-12;
}

static int close_float(float actual, float expected) {
  return fabsf(actual - expected) <= 1e-6f;
}

static int check_double(const char *label, double actual, double expected) {
  if (close_double(actual, expected)) return 1;
  fprintf(stderr, "%s: got %.17g, expected %.17g\n", label, actual, expected);
  return 0;
}

static int check_float(const char *label, float actual, float expected) {
  if (close_float(actual, expected)) return 1;
  fprintf(stderr, "%s: got %.9g, expected %.9g\n", label, actual, expected);
  return 0;
}

int main(void) {
  double dx[] = {1.0, -2.0, 3.0};
  double dy[] = {4.0, 5.0, 6.0};
  double dz[] = {0.0, 0.0, 0.0};

  cblas_daxpy(3, 2.0, dx, 1, dy, 1);
  if (!close_double(dy[0], 6.0)) return 1;
  if (!close_double(dy[1], 1.0)) return 1;
  if (!close_double(dy[2], 12.0)) return 1;

  cblas_dcopy(3, dy, 1, dz, 1);
  if (!close_double(dz[0], 6.0)) return 1;
  if (!close_double(dz[1], 1.0)) return 1;
  if (!close_double(dz[2], 12.0)) return 1;

  cblas_dswap(3, dx, 1, dz, 1);
  if (!close_double(dx[0], 6.0)) return 1;
  if (!close_double(dz[0], 1.0)) return 1;

  cblas_dscal(3, 0.5, dz, 1);
  if (!close_double(dz[0], 0.5)) return 1;
  if (!close_double(dz[1], -1.0)) return 1;
  if (!close_double(dz[2], 1.5)) return 1;

  if (!close_double(cblas_ddot(3, dz, 1, dy, 1), 20.0)) return 1;
  if (!close_double(cblas_dasum(3, dz, 1), 3.0)) return 1;
  if (!close_double(cblas_dnrm2(3, dz, 1), sqrt(3.5))) return 1;
  if (cblas_idamax(3, dz, 1) != 2) return 1;

  float sx[] = {1.0f, -2.0f, 3.0f};
  float sy[] = {4.0f, 5.0f, 6.0f};
  float sz[] = {0.0f, 0.0f, 0.0f};

  cblas_saxpy(3, 2.0f, sx, 1, sy, 1);
  if (!close_float(sy[0], 6.0f)) return 1;
  if (!close_float(sy[1], 1.0f)) return 1;
  if (!close_float(sy[2], 12.0f)) return 1;

  cblas_scopy(3, sy, 1, sz, 1);
  if (!close_float(sz[0], 6.0f)) return 1;
  if (!close_float(sz[1], 1.0f)) return 1;
  if (!close_float(sz[2], 12.0f)) return 1;

  cblas_sswap(3, sx, 1, sz, 1);
  if (!close_float(sx[0], 6.0f)) return 1;
  if (!close_float(sz[0], 1.0f)) return 1;

  cblas_sscal(3, 0.5f, sz, 1);
  if (!close_float(sz[0], 0.5f)) return 1;
  if (!close_float(sz[1], -1.0f)) return 1;
  if (!close_float(sz[2], 1.5f)) return 1;

  if (!close_float(cblas_sdot(3, sz, 1, sy, 1), 20.0f)) return 1;
  if (!close_float(cblas_sasum(3, sz, 1), 3.0f)) return 1;
  if (!close_float(cblas_snrm2(3, sz, 1), sqrtf(3.5f))) return 1;
  if (cblas_isamax(3, sz, 1) != 2) return 1;

  const float c_alpha[] = {2.0f, -1.0f};
  const float c_x[] = {1.0f, 2.0f, 3.0f, -1.0f};
  float c_y[] = {4.0f, -2.0f, -1.0f, 5.0f};
  cblas_caxpy(2, c_alpha, c_x, 1, c_y, 1);
  if (!check_float("caxpy y0 real", c_y[0], 8.0f)) return 1;
  if (!check_float("caxpy y0 imag", c_y[1], 1.0f)) return 1;
  if (!check_float("caxpy y1 real", c_y[2], 4.0f)) return 1;
  if (!check_float("caxpy y1 imag", c_y[3], 0.0f)) return 1;

  float c_axpyc_y[] = {4.0f, -2.0f, -1.0f, 5.0f};
  cblas_caxpyc(2, c_alpha, c_x, 1, c_axpyc_y, 1);
  if (!check_float("caxpyc y0 real", c_axpyc_y[0], 4.0f)) return 1;
  if (!check_float("caxpyc y0 imag", c_axpyc_y[1], -7.0f)) return 1;
  if (!check_float("caxpyc y1 real", c_axpyc_y[2], 6.0f)) return 1;
  if (!check_float("caxpyc y1 imag", c_axpyc_y[3], 4.0f)) return 1;

  float c_copy_dest[] = {0.0f, 0.0f, 0.0f, 0.0f};
  cblas_ccopy(2, c_y, 1, c_copy_dest, 1);
  if (!check_float("ccopy y1 imag", c_copy_dest[3], 0.0f)) return 1;

  float c_swap_x[] = {1.0f, 2.0f, 3.0f, 4.0f};
  float c_swap_y[] = {5.0f, 6.0f, 7.0f, 8.0f};
  cblas_cswap(2, c_swap_x, 1, c_swap_y, 1);
  if (!check_float("cswap x0 real", c_swap_x[0], 5.0f)) return 1;
  if (!check_float("cswap y1 imag", c_swap_y[3], 4.0f)) return 1;

  float c_scale_x[] = {1.0f, 2.0f, 3.0f, -1.0f};
  cblas_cscal(2, c_alpha, c_scale_x, 1);
  if (!check_float("cscal x0 real", c_scale_x[0], 4.0f)) return 1;
  if (!check_float("cscal x0 imag", c_scale_x[1], 3.0f)) return 1;
  if (!check_float("cscal x1 real", c_scale_x[2], 5.0f)) return 1;
  if (!check_float("cscal x1 imag", c_scale_x[3], -5.0f)) return 1;

  cblas_csscal(2, 0.5f, c_scale_x, 1);
  if (!check_float("csscal x0 real", c_scale_x[0], 2.0f)) return 1;
  if (!check_float("csscal x1 imag", c_scale_x[3], -2.5f)) return 1;

  float c_dotu[2] = {0.0f, 0.0f};
  float c_dotc[2] = {0.0f, 0.0f};
  cblas_cdotu_sub(2, c_x, 1, (float[]){4.0f, -2.0f, -1.0f, 5.0f}, 1,
                  c_dotu);
  cblas_cdotc_sub(2, c_x, 1, (float[]){4.0f, -2.0f, -1.0f, 5.0f}, 1,
                  c_dotc);
  if (!check_float("cdotu real", c_dotu[0], 10.0f)) return 1;
  if (!check_float("cdotu imag", c_dotu[1], 22.0f)) return 1;
  if (!check_float("cdotc real", c_dotc[0], -8.0f)) return 1;
  if (!check_float("cdotc imag", c_dotc[1], 4.0f)) return 1;
  if (!check_float("scasum", cblas_scasum(2, c_x, 1), 7.0f)) return 1;
  if (!check_float("scnrm2", cblas_scnrm2(2, c_x, 1), sqrtf(15.0f))) return 1;
  if (cblas_icamax(2, c_x, 1) != 1) return 1;

  const double z_alpha[] = {2.0, -1.0};
  const double z_x[] = {1.0, 2.0, 3.0, -1.0};
  double z_y[] = {4.0, -2.0, -1.0, 5.0};
  cblas_zaxpy(2, z_alpha, z_x, 1, z_y, 1);
  if (!check_double("zaxpy y0 real", z_y[0], 8.0)) return 1;
  if (!check_double("zaxpy y1 imag", z_y[3], 0.0)) return 1;

  double z_axpyc_y[] = {4.0, -2.0, -1.0, 5.0};
  cblas_zaxpyc(2, z_alpha, z_x, 1, z_axpyc_y, 1);
  if (!check_double("zaxpyc y0 imag", z_axpyc_y[1], -7.0)) return 1;
  if (!check_double("zaxpyc y1 real", z_axpyc_y[2], 6.0)) return 1;

  double z_copy_dest[] = {0.0, 0.0, 0.0, 0.0};
  cblas_zcopy(2, z_y, 1, z_copy_dest, 1);
  if (!check_double("zcopy y0 real", z_copy_dest[0], 8.0)) return 1;

  double z_swap_x[] = {1.0, 2.0, 3.0, 4.0};
  double z_swap_y[] = {5.0, 6.0, 7.0, 8.0};
  cblas_zswap(2, z_swap_x, 1, z_swap_y, 1);
  if (!check_double("zswap x1 real", z_swap_x[2], 7.0)) return 1;
  if (!check_double("zswap y0 imag", z_swap_y[1], 2.0)) return 1;

  double z_scale_x[] = {1.0, 2.0, 3.0, -1.0};
  cblas_zscal(2, z_alpha, z_scale_x, 1);
  if (!check_double("zscal x0 real", z_scale_x[0], 4.0)) return 1;
  if (!check_double("zscal x1 imag", z_scale_x[3], -5.0)) return 1;

  cblas_zdscal(2, 0.5, z_scale_x, 1);
  if (!check_double("zdscal x0 imag", z_scale_x[1], 1.5)) return 1;
  if (!check_double("zdscal x1 real", z_scale_x[2], 2.5)) return 1;

  double z_dotu[2] = {0.0, 0.0};
  double z_dotc[2] = {0.0, 0.0};
  cblas_zdotu_sub(2, z_x, 1, (double[]){4.0, -2.0, -1.0, 5.0}, 1, z_dotu);
  cblas_zdotc_sub(2, z_x, 1, (double[]){4.0, -2.0, -1.0, 5.0}, 1, z_dotc);
  if (!check_double("zdotu real", z_dotu[0], 10.0)) return 1;
  if (!check_double("zdotu imag", z_dotu[1], 22.0)) return 1;
  if (!check_double("zdotc real", z_dotc[0], -8.0)) return 1;
  if (!check_double("zdotc imag", z_dotc[1], 4.0)) return 1;
  if (!check_double("dzasum", cblas_dzasum(2, z_x, 1), 7.0)) return 1;
  if (!check_double("dznrm2", cblas_dznrm2(2, z_x, 1), sqrt(15.0))) return 1;
  if (cblas_izamax(2, z_x, 1) != 1) return 1;

  const float c_gemv_alpha[] = {1.0f, 0.0f};
  const float c_gemv_beta[] = {0.0f, 0.0f};
  const float c_gemv_a[] = {1.0f, 1.0f, 2.0f, -1.0f, 3.0f, 0.0f, 0.0f, 4.0f};
  const float c_gemv_x[] = {1.0f, 0.0f, 0.0f, 1.0f};
  float c_gemv_y[] = {0.0f, 0.0f, 0.0f, 0.0f};
  cblas_cgemv(CblasColMajor, CblasNoTrans, 2, 2, c_gemv_alpha, c_gemv_a, 2,
              c_gemv_x, 1, c_gemv_beta, c_gemv_y, 1);
  if (!check_float("cgemv y0 real", c_gemv_y[0], 1.0f)) return 1;
  if (!check_float("cgemv y0 imag", c_gemv_y[1], 4.0f)) return 1;
  if (!check_float("cgemv y1 real", c_gemv_y[2], -2.0f)) return 1;
  if (!check_float("cgemv y1 imag", c_gemv_y[3], -1.0f)) return 1;

  const float c_ger_alpha[] = {1.0f, 0.0f};
  const float c_ger_x[] = {1.0f, 1.0f, 2.0f, 0.0f};
  const float c_ger_y[] = {3.0f, 0.0f, 0.0f, 1.0f};
  float c_geru_a[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  cblas_cgeru(CblasColMajor, 2, 2, c_ger_alpha, c_ger_x, 1, c_ger_y, 1,
              c_geru_a, 2);
  if (!check_float("cgeru a00 real", c_geru_a[0], 3.0f)) return 1;
  if (!check_float("cgeru a00 imag", c_geru_a[1], 3.0f)) return 1;
  if (!check_float("cgeru a01 real", c_geru_a[4], -1.0f)) return 1;
  if (!check_float("cgeru a01 imag", c_geru_a[5], 1.0f)) return 1;
  if (!check_float("cgeru a11 imag", c_geru_a[7], 2.0f)) return 1;

  float c_gerc_a[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  cblas_cgerc(CblasColMajor, 2, 2, c_ger_alpha, c_ger_x, 1, c_ger_y, 1,
              c_gerc_a, 2);
  if (!check_float("cgerc a01 real", c_gerc_a[4], 1.0f)) return 1;
  if (!check_float("cgerc a01 imag", c_gerc_a[5], -1.0f)) return 1;
  if (!check_float("cgerc a11 imag", c_gerc_a[7], -2.0f)) return 1;

  const double z_gemv_alpha[] = {1.0, 0.0};
  const double z_gemv_beta[] = {0.0, 0.0};
  const double z_gemv_a[] = {1.0, 1.0, 2.0, -1.0, 3.0, 0.0, 0.0, 4.0};
  const double z_gemv_x[] = {1.0, 0.0, 0.0, 1.0};
  double z_gemv_y[] = {0.0, 0.0, 0.0, 0.0};
  cblas_zgemv(CblasColMajor, CblasNoTrans, 2, 2, z_gemv_alpha, z_gemv_a, 2,
              z_gemv_x, 1, z_gemv_beta, z_gemv_y, 1);
  if (!check_double("zgemv y0 imag", z_gemv_y[1], 4.0)) return 1;
  if (!check_double("zgemv y1 real", z_gemv_y[2], -2.0)) return 1;

  const double z_ger_alpha[] = {1.0, 0.0};
  const double z_ger_x[] = {1.0, 1.0, 2.0, 0.0};
  const double z_ger_y[] = {3.0, 0.0, 0.0, 1.0};
  double z_geru_a[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  cblas_zgeru(CblasColMajor, 2, 2, z_ger_alpha, z_ger_x, 1, z_ger_y, 1,
              z_geru_a, 2);
  if (!check_double("zgeru a01 real", z_geru_a[4], -1.0)) return 1;
  if (!check_double("zgeru a11 imag", z_geru_a[7], 2.0)) return 1;

  double z_gerc_a[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  cblas_zgerc(CblasColMajor, 2, 2, z_ger_alpha, z_ger_x, 1, z_ger_y, 1,
              z_gerc_a, 2);
  if (!check_double("zgerc a01 real", z_gerc_a[4], 1.0)) return 1;
  if (!check_double("zgerc a11 imag", z_gerc_a[7], -2.0)) return 1;

  const float c_tri_a[] = {1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 3.0f, 0.0f};
  const float c_tri_packed[] = {1.0f, 0.0f, 2.0f, 0.0f, 3.0f, 0.0f};
  const float c_tri_band[] = {0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 3.0f, 0.0f};
  float c_tri_x[] = {1.0f, 0.0f, 2.0f, 0.0f};
  cblas_ctrmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2,
              c_tri_a, 2, c_tri_x, 1);
  if (!check_float("ctrmv x0 real", c_tri_x[0], 5.0f)) return 1;
  if (!check_float("ctrmv x1 real", c_tri_x[2], 6.0f)) return 1;

  cblas_ctrsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2,
              c_tri_a, 2, c_tri_x, 1);
  if (!check_float("ctrsv x0 real", c_tri_x[0], 1.0f)) return 1;
  if (!check_float("ctrsv x1 real", c_tri_x[2], 2.0f)) return 1;

  c_tri_x[0] = 1.0f;
  c_tri_x[2] = 2.0f;
  cblas_ctpmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2,
              c_tri_packed, c_tri_x, 1);
  if (!check_float("ctpmv x0 real", c_tri_x[0], 5.0f)) return 1;
  if (!check_float("ctpmv x1 real", c_tri_x[2], 6.0f)) return 1;

  cblas_ctpsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2,
              c_tri_packed, c_tri_x, 1);
  if (!check_float("ctpsv x0 real", c_tri_x[0], 1.0f)) return 1;
  if (!check_float("ctpsv x1 real", c_tri_x[2], 2.0f)) return 1;

  c_tri_x[0] = 1.0f;
  c_tri_x[2] = 2.0f;
  cblas_ctbmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2, 1,
              c_tri_band, 2, c_tri_x, 1);
  if (!check_float("ctbmv x0 real", c_tri_x[0], 5.0f)) return 1;
  if (!check_float("ctbmv x1 real", c_tri_x[2], 6.0f)) return 1;

  cblas_ctbsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2, 1,
              c_tri_band, 2, c_tri_x, 1);
  if (!check_float("ctbsv x0 real", c_tri_x[0], 1.0f)) return 1;
  if (!check_float("ctbsv x1 real", c_tri_x[2], 2.0f)) return 1;

  const double z_tri_a[] = {1.0, 0.0, 0.0, 0.0, 2.0, 0.0, 3.0, 0.0};
  const double z_tri_packed[] = {1.0, 0.0, 2.0, 0.0, 3.0, 0.0};
  const double z_tri_band[] = {0.0, 0.0, 1.0, 0.0, 2.0, 0.0, 3.0, 0.0};
  double z_tri_x[] = {1.0, 0.0, 2.0, 0.0};
  cblas_ztrmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2,
              z_tri_a, 2, z_tri_x, 1);
  if (!check_double("ztrmv x0 real", z_tri_x[0], 5.0)) return 1;
  if (!check_double("ztrmv x1 real", z_tri_x[2], 6.0)) return 1;

  cblas_ztrsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2,
              z_tri_a, 2, z_tri_x, 1);
  if (!check_double("ztrsv x0 real", z_tri_x[0], 1.0)) return 1;
  if (!check_double("ztrsv x1 real", z_tri_x[2], 2.0)) return 1;

  z_tri_x[0] = 1.0;
  z_tri_x[2] = 2.0;
  cblas_ztpmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2,
              z_tri_packed, z_tri_x, 1);
  if (!check_double("ztpmv x0 real", z_tri_x[0], 5.0)) return 1;
  if (!check_double("ztpmv x1 real", z_tri_x[2], 6.0)) return 1;

  cblas_ztpsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2,
              z_tri_packed, z_tri_x, 1);
  if (!check_double("ztpsv x0 real", z_tri_x[0], 1.0)) return 1;
  if (!check_double("ztpsv x1 real", z_tri_x[2], 2.0)) return 1;

  z_tri_x[0] = 1.0;
  z_tri_x[2] = 2.0;
  cblas_ztbmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2, 1,
              z_tri_band, 2, z_tri_x, 1);
  if (!check_double("ztbmv x0 real", z_tri_x[0], 5.0)) return 1;
  if (!check_double("ztbmv x1 real", z_tri_x[2], 6.0)) return 1;

  cblas_ztbsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2, 1,
              z_tri_band, 2, z_tri_x, 1);
  if (!check_double("ztbsv x0 real", z_tri_x[0], 1.0)) return 1;
  if (!check_double("ztbsv x1 real", z_tri_x[2], 2.0)) return 1;

  const float c_gbmv_a[] = {1.0f, 0.0f, 4.0f, 0.0f};
  const float c_gbmv_x[] = {1.0f, 0.0f, 2.0f, 0.0f};
  float c_gbmv_y[] = {0.0f, 0.0f, 0.0f, 0.0f};
  cblas_cgbmv(CblasColMajor, CblasNoTrans, 2, 2, 0, 0, c_gemv_alpha,
              c_gbmv_a, 1, c_gbmv_x, 1, c_gemv_beta, c_gbmv_y, 1);
  if (!check_float("cgbmv y0 real", c_gbmv_y[0], 1.0f)) return 1;
  if (!check_float("cgbmv y1 real", c_gbmv_y[2], 8.0f)) return 1;

  const float c_herm_a[] = {1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 5.0f, 0.0f};
  const float c_herm_band[] = {0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 5.0f, 0.0f};
  const float c_herm_packed[] = {1.0f, 0.0f, 2.0f, 0.0f, 5.0f, 0.0f};
  const float c_herm_x[] = {1.0f, 0.0f, 2.0f, 0.0f};
  float c_hemv_y[] = {0.0f, 0.0f, 0.0f, 0.0f};
  cblas_chemv(CblasColMajor, CblasUpper, 2, c_gemv_alpha, c_herm_a, 2,
              c_herm_x, 1, c_gemv_beta, c_hemv_y, 1);
  if (!check_float("chemv y0 real", c_hemv_y[0], 5.0f)) return 1;
  if (!check_float("chemv y1 real", c_hemv_y[2], 12.0f)) return 1;

  float c_hbmv_y[] = {0.0f, 0.0f, 0.0f, 0.0f};
  cblas_chbmv(CblasColMajor, CblasUpper, 2, 1, c_gemv_alpha, c_herm_band, 2,
              c_herm_x, 1, c_gemv_beta, c_hbmv_y, 1);
  if (!check_float("chbmv y0 real", c_hbmv_y[0], 5.0f)) return 1;
  if (!check_float("chbmv y1 real", c_hbmv_y[2], 12.0f)) return 1;

  float c_hpmv_y[] = {0.0f, 0.0f, 0.0f, 0.0f};
  cblas_chpmv(CblasColMajor, CblasUpper, 2, c_gemv_alpha, c_herm_packed,
              c_herm_x, 1, c_gemv_beta, c_hpmv_y, 1);
  if (!check_float("chpmv y0 real", c_hpmv_y[0], 5.0f)) return 1;
  if (!check_float("chpmv y1 real", c_hpmv_y[2], 12.0f)) return 1;

  float c_her_a[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  cblas_cher(CblasColMajor, CblasUpper, 2, 1.0f, c_herm_x, 1, c_her_a, 2);
  if (!check_float("cher a00 real", c_her_a[0], 1.0f)) return 1;
  if (!check_float("cher a01 real", c_her_a[4], 2.0f)) return 1;
  if (!check_float("cher a11 real", c_her_a[6], 4.0f)) return 1;

  float c_her2_a[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  const float c_her_y[] = {3.0f, 0.0f, 4.0f, 0.0f};
  cblas_cher2(CblasColMajor, CblasUpper, 2, c_gemv_alpha, c_herm_x, 1,
              c_her_y, 1, c_her2_a, 2);
  if (!check_float("cher2 a00 real", c_her2_a[0], 6.0f)) return 1;
  if (!check_float("cher2 a01 real", c_her2_a[4], 10.0f)) return 1;
  if (!check_float("cher2 a11 real", c_her2_a[6], 16.0f)) return 1;

  float c_hpr_a[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  cblas_chpr(CblasColMajor, CblasUpper, 2, 1.0f, c_herm_x, 1, c_hpr_a);
  if (!check_float("chpr a00 real", c_hpr_a[0], 1.0f)) return 1;
  if (!check_float("chpr a01 real", c_hpr_a[2], 2.0f)) return 1;
  if (!check_float("chpr a11 real", c_hpr_a[4], 4.0f)) return 1;

  float c_hpr2_a[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  cblas_chpr2(CblasColMajor, CblasUpper, 2, c_gemv_alpha, c_herm_x, 1,
              c_her_y, 1, c_hpr2_a);
  if (!check_float("chpr2 a00 real", c_hpr2_a[0], 6.0f)) return 1;
  if (!check_float("chpr2 a01 real", c_hpr2_a[2], 10.0f)) return 1;
  if (!check_float("chpr2 a11 real", c_hpr2_a[4], 16.0f)) return 1;

  const double z_gbmv_a[] = {1.0, 0.0, 4.0, 0.0};
  const double z_gbmv_x[] = {1.0, 0.0, 2.0, 0.0};
  double z_gbmv_y[] = {0.0, 0.0, 0.0, 0.0};
  cblas_zgbmv(CblasColMajor, CblasNoTrans, 2, 2, 0, 0, z_gemv_alpha,
              z_gbmv_a, 1, z_gbmv_x, 1, z_gemv_beta, z_gbmv_y, 1);
  if (!check_double("zgbmv y0 real", z_gbmv_y[0], 1.0)) return 1;
  if (!check_double("zgbmv y1 real", z_gbmv_y[2], 8.0)) return 1;

  const double z_herm_a[] = {1.0, 0.0, 0.0, 0.0, 2.0, 0.0, 5.0, 0.0};
  const double z_herm_band[] = {0.0, 0.0, 1.0, 0.0, 2.0, 0.0, 5.0, 0.0};
  const double z_herm_packed[] = {1.0, 0.0, 2.0, 0.0, 5.0, 0.0};
  const double z_herm_x[] = {1.0, 0.0, 2.0, 0.0};
  double z_hemv_y[] = {0.0, 0.0, 0.0, 0.0};
  cblas_zhemv(CblasColMajor, CblasUpper, 2, z_gemv_alpha, z_herm_a, 2,
              z_herm_x, 1, z_gemv_beta, z_hemv_y, 1);
  if (!check_double("zhemv y0 real", z_hemv_y[0], 5.0)) return 1;
  if (!check_double("zhemv y1 real", z_hemv_y[2], 12.0)) return 1;

  double z_hbmv_y[] = {0.0, 0.0, 0.0, 0.0};
  cblas_zhbmv(CblasColMajor, CblasUpper, 2, 1, z_gemv_alpha, z_herm_band, 2,
              z_herm_x, 1, z_gemv_beta, z_hbmv_y, 1);
  if (!check_double("zhbmv y1 real", z_hbmv_y[2], 12.0)) return 1;

  double z_hpmv_y[] = {0.0, 0.0, 0.0, 0.0};
  cblas_zhpmv(CblasColMajor, CblasUpper, 2, z_gemv_alpha, z_herm_packed,
              z_herm_x, 1, z_gemv_beta, z_hpmv_y, 1);
  if (!check_double("zhpmv y0 real", z_hpmv_y[0], 5.0)) return 1;

  double z_her_a[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  cblas_zher(CblasColMajor, CblasUpper, 2, 1.0, z_herm_x, 1, z_her_a, 2);
  if (!check_double("zher a01 real", z_her_a[4], 2.0)) return 1;

  double z_her2_a[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  const double z_her_y[] = {3.0, 0.0, 4.0, 0.0};
  cblas_zher2(CblasColMajor, CblasUpper, 2, z_gemv_alpha, z_herm_x, 1,
              z_her_y, 1, z_her2_a, 2);
  if (!check_double("zher2 a11 real", z_her2_a[6], 16.0)) return 1;

  double z_hpr_a[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  cblas_zhpr(CblasColMajor, CblasUpper, 2, 1.0, z_herm_x, 1, z_hpr_a);
  if (!check_double("zhpr a11 real", z_hpr_a[4], 4.0)) return 1;

  double z_hpr2_a[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  cblas_zhpr2(CblasColMajor, CblasUpper, 2, z_gemv_alpha, z_herm_x, 1,
              z_her_y, 1, z_hpr2_a);
  if (!check_double("zhpr2 a01 real", z_hpr2_a[2], 10.0)) return 1;

  const double d_col_major_a[] = {1.0, 4.0, 2.0, 5.0, 3.0, 6.0};
  const double d_row_major_a[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  const double d_gemv_x[] = {1.0, 2.0, 3.0};
  double d_gemv_y[] = {10.0, 20.0};

  cblas_dgemv(CblasColMajor, CblasNoTrans, 2, 3, 2.0, d_col_major_a, 2,
              d_gemv_x, 1, 0.5, d_gemv_y, 1);
  if (!close_double(d_gemv_y[0], 33.0)) return 1;
  if (!close_double(d_gemv_y[1], 74.0)) return 1;

  d_gemv_y[0] = 10.0;
  d_gemv_y[1] = 20.0;
  cblas_dgemv(CblasRowMajor, CblasNoTrans, 2, 3, 2.0, d_row_major_a, 3,
              d_gemv_x, 1, 0.5, d_gemv_y, 1);
  if (!close_double(d_gemv_y[0], 33.0)) return 1;
  if (!close_double(d_gemv_y[1], 74.0)) return 1;

  const double d_gemv_trans_x[] = {1.0, 2.0};
  double d_gemv_trans_y[] = {1.0, 1.0, 1.0};
  cblas_dgemv(CblasRowMajor, CblasTrans, 2, 3, 1.0, d_row_major_a, 3,
              d_gemv_trans_x, 1, 1.0, d_gemv_trans_y, 1);
  if (!close_double(d_gemv_trans_y[0], 10.0)) return 1;
  if (!close_double(d_gemv_trans_y[1], 13.0)) return 1;
  if (!close_double(d_gemv_trans_y[2], 16.0)) return 1;

  const float s_row_major_a[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  const float s_gemv_x[] = {1.0f, 2.0f, 3.0f};
  float s_gemv_y[] = {10.0f, 20.0f};
  cblas_sgemv(CblasRowMajor, CblasNoTrans, 2, 3, 2.0f, s_row_major_a, 3,
              s_gemv_x, 1, 0.5f, s_gemv_y, 1);
  if (!close_float(s_gemv_y[0], 33.0f)) return 1;
  if (!close_float(s_gemv_y[1], 74.0f)) return 1;

  double d_ger_col_major_a[] = {1.0, 4.0, 2.0, 5.0, 3.0, 6.0};
  const double d_ger_x[] = {1.0, 2.0};
  const double d_ger_y[] = {3.0, 4.0, 5.0};
  cblas_dger(CblasColMajor, 2, 3, 2.0, d_ger_x, 1, d_ger_y, 1,
             d_ger_col_major_a, 2);
  if (!close_double(d_ger_col_major_a[0], 7.0)) return 1;
  if (!close_double(d_ger_col_major_a[1], 16.0)) return 1;
  if (!close_double(d_ger_col_major_a[2], 10.0)) return 1;
  if (!close_double(d_ger_col_major_a[3], 21.0)) return 1;
  if (!close_double(d_ger_col_major_a[4], 13.0)) return 1;
  if (!close_double(d_ger_col_major_a[5], 26.0)) return 1;

  double d_ger_row_major_a[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  cblas_dger(CblasRowMajor, 2, 3, 2.0, d_ger_x, 1, d_ger_y, 1,
             d_ger_row_major_a, 3);
  if (!close_double(d_ger_row_major_a[0], 7.0)) return 1;
  if (!close_double(d_ger_row_major_a[1], 10.0)) return 1;
  if (!close_double(d_ger_row_major_a[2], 13.0)) return 1;
  if (!close_double(d_ger_row_major_a[3], 16.0)) return 1;
  if (!close_double(d_ger_row_major_a[4], 21.0)) return 1;
  if (!close_double(d_ger_row_major_a[5], 26.0)) return 1;

  float s_ger_row_major_a[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  const float s_ger_x[] = {1.0f, -99.0f, 2.0f};
  const float s_ger_y[] = {3.0f, -99.0f, 4.0f, -99.0f, 5.0f};
  cblas_sger(CblasRowMajor, 2, 3, 2.0f, s_ger_x, 2, s_ger_y, 2,
             s_ger_row_major_a, 3);
  if (!close_float(s_ger_row_major_a[0], 7.0f)) return 1;
  if (!close_float(s_ger_row_major_a[1], 10.0f)) return 1;
  if (!close_float(s_ger_row_major_a[2], 13.0f)) return 1;
  if (!close_float(s_ger_row_major_a[3], 16.0f)) return 1;
  if (!close_float(s_ger_row_major_a[4], 21.0f)) return 1;
  if (!close_float(s_ger_row_major_a[5], 26.0f)) return 1;

  const double d_symv_a[] = {1.0, 2.0, 3.0, 2.0, 5.0,
                             6.0, 3.0, 6.0, 9.0};
  const double d_symv_x[] = {1.0, 2.0, 3.0};
  double d_symv_y[] = {10.0, 20.0, 30.0};
  cblas_dsymv(CblasColMajor, CblasUpper, 3, 2.0, d_symv_a, 3, d_symv_x, 1,
              0.5, d_symv_y, 1);
  if (!close_double(d_symv_y[0], 33.0)) return 1;
  if (!close_double(d_symv_y[1], 70.0)) return 1;
  if (!close_double(d_symv_y[2], 99.0)) return 1;

  d_symv_y[0] = 10.0;
  d_symv_y[1] = 20.0;
  d_symv_y[2] = 30.0;
  cblas_dsymv(CblasRowMajor, CblasLower, 3, 2.0, d_symv_a, 3, d_symv_x, 1,
              0.5, d_symv_y, 1);
  if (!close_double(d_symv_y[0], 33.0)) return 1;
  if (!close_double(d_symv_y[1], 70.0)) return 1;
  if (!close_double(d_symv_y[2], 99.0)) return 1;

  const float s_symv_a[] = {1.0f, 2.0f, 3.0f, 2.0f, 5.0f,
                            6.0f, 3.0f, 6.0f, 9.0f};
  const float s_symv_x[] = {1.0f, 2.0f, 3.0f};
  float s_symv_y[] = {10.0f, 20.0f, 30.0f};
  cblas_ssymv(CblasRowMajor, CblasUpper, 3, 2.0f, s_symv_a, 3, s_symv_x, 1,
              0.5f, s_symv_y, 1);
  if (!close_float(s_symv_y[0], 33.0f)) return 1;
  if (!close_float(s_symv_y[1], 70.0f)) return 1;
  if (!close_float(s_symv_y[2], 99.0f)) return 1;

  const double d_trmv_upper_col_major_a[] = {1.0, 0.0, 0.0, 2.0, 5.0,
                                             0.0, 3.0, 6.0, 9.0};
  double d_trmv_x[] = {1.0, 2.0, 3.0};
  cblas_dtrmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 3,
              d_trmv_upper_col_major_a, 3, d_trmv_x, 1);
  if (!check_double("dtrmv upper col x0", d_trmv_x[0], 14.0)) return 1;
  if (!check_double("dtrmv upper col x1", d_trmv_x[1], 28.0)) return 1;
  if (!check_double("dtrmv upper col x2", d_trmv_x[2], 27.0)) return 1;

  const double d_trmv_lower_col_major_a[] = {99.0, 4.0, 7.0, 0.0, 99.0,
                                             8.0, 0.0, 0.0, 99.0};
  d_trmv_x[0] = 1.0;
  d_trmv_x[1] = 2.0;
  d_trmv_x[2] = 3.0;
  cblas_dtrmv(CblasColMajor, CblasLower, CblasTrans, CblasUnit, 3,
              d_trmv_lower_col_major_a, 3, d_trmv_x, 1);
  if (!check_double("dtrmv lower col trans x0", d_trmv_x[0], 30.0)) return 1;
  if (!check_double("dtrmv lower col trans x1", d_trmv_x[1], 26.0)) return 1;
  if (!check_double("dtrmv lower col trans x2", d_trmv_x[2], 3.0)) return 1;

  const double d_trmv_lower_row_major_a[] = {1.0, 0.0, 0.0, 4.0, 5.0,
                                             0.0, 7.0, 8.0, 9.0};
  d_trmv_x[0] = 1.0;
  d_trmv_x[1] = 2.0;
  d_trmv_x[2] = 3.0;
  cblas_dtrmv(CblasRowMajor, CblasLower, CblasNoTrans, CblasNonUnit, 3,
              d_trmv_lower_row_major_a, 3, d_trmv_x, 1);
  if (!check_double("dtrmv lower row x0", d_trmv_x[0], 1.0)) return 1;
  if (!check_double("dtrmv lower row x1", d_trmv_x[1], 14.0)) return 1;
  if (!check_double("dtrmv lower row x2", d_trmv_x[2], 50.0)) return 1;

  const float s_trmv_upper_row_major_a[] = {99.0f, 2.0f, 3.0f, 0.0f, 99.0f,
                                            6.0f, 0.0f, 0.0f, 99.0f};
  float s_trmv_x[] = {1.0f, -99.0f, 2.0f, -99.0f, 3.0f};
  cblas_strmv(CblasRowMajor, CblasUpper, CblasTrans, CblasUnit, 3,
              s_trmv_upper_row_major_a, 3, s_trmv_x, 2);
  if (!check_float("strmv upper row trans x0", s_trmv_x[0], 1.0f)) return 1;
  if (!check_float("strmv upper row trans x1", s_trmv_x[2], 4.0f)) return 1;
  if (!check_float("strmv upper row trans x2", s_trmv_x[4], 18.0f)) return 1;

  double d_trsv_x[] = {14.0, 28.0, 27.0};
  cblas_dtrsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 3,
              d_trmv_upper_col_major_a, 3, d_trsv_x, 1);
  if (!check_double("dtrsv upper col x0", d_trsv_x[0], 1.0)) return 1;
  if (!check_double("dtrsv upper col x1", d_trsv_x[1], 2.0)) return 1;
  if (!check_double("dtrsv upper col x2", d_trsv_x[2], 3.0)) return 1;

  d_trsv_x[0] = 30.0;
  d_trsv_x[1] = 26.0;
  d_trsv_x[2] = 3.0;
  cblas_dtrsv(CblasColMajor, CblasLower, CblasTrans, CblasUnit, 3,
              d_trmv_lower_col_major_a, 3, d_trsv_x, 1);
  if (!check_double("dtrsv lower col trans x0", d_trsv_x[0], 1.0)) return 1;
  if (!check_double("dtrsv lower col trans x1", d_trsv_x[1], 2.0)) return 1;
  if (!check_double("dtrsv lower col trans x2", d_trsv_x[2], 3.0)) return 1;

  d_trsv_x[0] = 1.0;
  d_trsv_x[1] = 14.0;
  d_trsv_x[2] = 50.0;
  cblas_dtrsv(CblasRowMajor, CblasLower, CblasNoTrans, CblasNonUnit, 3,
              d_trmv_lower_row_major_a, 3, d_trsv_x, 1);
  if (!check_double("dtrsv lower row x0", d_trsv_x[0], 1.0)) return 1;
  if (!check_double("dtrsv lower row x1", d_trsv_x[1], 2.0)) return 1;
  if (!check_double("dtrsv lower row x2", d_trsv_x[2], 3.0)) return 1;

  float s_trsv_x[] = {1.0f, -99.0f, 4.0f, -99.0f, 18.0f};
  cblas_strsv(CblasRowMajor, CblasUpper, CblasTrans, CblasUnit, 3,
              s_trmv_upper_row_major_a, 3, s_trsv_x, 2);
  if (!check_float("strsv upper row trans x0", s_trsv_x[0], 1.0f)) return 1;
  if (!check_float("strsv upper row trans x1", s_trsv_x[2], 2.0f)) return 1;
  if (!check_float("strsv upper row trans x2", s_trsv_x[4], 3.0f)) return 1;

  const double d_tpmv_upper_col_major_a[] = {1.0, 2.0, 5.0, 3.0, 6.0, 9.0};
  double d_tpmv_x[] = {1.0, 2.0, 3.0};
  cblas_dtpmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 3,
              d_tpmv_upper_col_major_a, d_tpmv_x, 1);
  if (!check_double("dtpmv upper col x0", d_tpmv_x[0], 14.0)) return 1;
  if (!check_double("dtpmv upper col x1", d_tpmv_x[1], 28.0)) return 1;
  if (!check_double("dtpmv upper col x2", d_tpmv_x[2], 27.0)) return 1;

  const double d_tpmv_lower_col_major_a[] = {99.0, 4.0, 7.0, 99.0, 8.0, 99.0};
  d_tpmv_x[0] = 1.0;
  d_tpmv_x[1] = 2.0;
  d_tpmv_x[2] = 3.0;
  cblas_dtpmv(CblasColMajor, CblasLower, CblasTrans, CblasUnit, 3,
              d_tpmv_lower_col_major_a, d_tpmv_x, 1);
  if (!check_double("dtpmv lower col trans x0", d_tpmv_x[0], 30.0)) return 1;
  if (!check_double("dtpmv lower col trans x1", d_tpmv_x[1], 26.0)) return 1;
  if (!check_double("dtpmv lower col trans x2", d_tpmv_x[2], 3.0)) return 1;

  double d_tpsv_x[] = {14.0, 28.0, 27.0};
  cblas_dtpsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 3,
              d_tpmv_upper_col_major_a, d_tpsv_x, 1);
  if (!check_double("dtpsv upper col x0", d_tpsv_x[0], 1.0)) return 1;
  if (!check_double("dtpsv upper col x1", d_tpsv_x[1], 2.0)) return 1;
  if (!check_double("dtpsv upper col x2", d_tpsv_x[2], 3.0)) return 1;

  float s_tpmv_upper_row_major_a[] = {99.0f, 2.0f, 3.0f, 99.0f, 6.0f, 99.0f};
  float s_tpmv_x[] = {1.0f, -99.0f, 2.0f, -99.0f, 3.0f};
  cblas_stpmv(CblasRowMajor, CblasUpper, CblasTrans, CblasUnit, 3,
              s_tpmv_upper_row_major_a, s_tpmv_x, 2);
  if (!check_float("stpmv upper row trans x0", s_tpmv_x[0], 1.0f)) return 1;
  if (!check_float("stpmv upper row trans x1", s_tpmv_x[2], 4.0f)) return 1;
  if (!check_float("stpmv upper row trans x2", s_tpmv_x[4], 18.0f)) return 1;

  float s_tpsv_x[] = {1.0f, -99.0f, 4.0f, -99.0f, 18.0f};
  cblas_stpsv(CblasRowMajor, CblasUpper, CblasTrans, CblasUnit, 3,
              s_tpmv_upper_row_major_a, s_tpsv_x, 2);
  if (!check_float("stpsv upper row trans x0", s_tpsv_x[0], 1.0f)) return 1;
  if (!check_float("stpsv upper row trans x1", s_tpsv_x[2], 2.0f)) return 1;
  if (!check_float("stpsv upper row trans x2", s_tpsv_x[4], 3.0f)) return 1;

  const double d_syr_x[] = {1.0, 2.0, 3.0};
  const double d_syr_y[] = {4.0, 5.0, 6.0};
  double d_syr_a[] = {1.0, 0.0, 0.0, 2.0, 5.0, 0.0, 3.0, 6.0, 9.0};
  cblas_dsyr(CblasColMajor, CblasUpper, 3, 2.0, d_syr_x, 1, d_syr_a, 3);
  if (!check_double("dsyr a00", d_syr_a[0], 3.0)) return 1;
  if (!check_double("dsyr a01", d_syr_a[3], 6.0)) return 1;
  if (!check_double("dsyr a11", d_syr_a[4], 13.0)) return 1;
  if (!check_double("dsyr a02", d_syr_a[6], 9.0)) return 1;
  if (!check_double("dsyr a12", d_syr_a[7], 18.0)) return 1;
  if (!check_double("dsyr a22", d_syr_a[8], 27.0)) return 1;

  double d_syr2_a[] = {1.0, 0.0, 0.0, 2.0, 5.0, 0.0, 3.0, 6.0, 9.0};
  cblas_dsyr2(CblasColMajor, CblasUpper, 3, 1.0, d_syr_x, 1, d_syr_y, 1,
              d_syr2_a, 3);
  if (!check_double("dsyr2 a00", d_syr2_a[0], 9.0)) return 1;
  if (!check_double("dsyr2 a01", d_syr2_a[3], 15.0)) return 1;
  if (!check_double("dsyr2 a11", d_syr2_a[4], 25.0)) return 1;
  if (!check_double("dsyr2 a02", d_syr2_a[6], 21.0)) return 1;
  if (!check_double("dsyr2 a12", d_syr2_a[7], 33.0)) return 1;
  if (!check_double("dsyr2 a22", d_syr2_a[8], 45.0)) return 1;

  double d_spr_a[] = {1.0, 2.0, 5.0, 3.0, 6.0, 9.0};
  cblas_dspr(CblasColMajor, CblasUpper, 3, 2.0, d_syr_x, 1, d_spr_a);
  if (!check_double("dspr a00", d_spr_a[0], 3.0)) return 1;
  if (!check_double("dspr a01", d_spr_a[1], 6.0)) return 1;
  if (!check_double("dspr a11", d_spr_a[2], 13.0)) return 1;
  if (!check_double("dspr a02", d_spr_a[3], 9.0)) return 1;
  if (!check_double("dspr a12", d_spr_a[4], 18.0)) return 1;
  if (!check_double("dspr a22", d_spr_a[5], 27.0)) return 1;

  double d_spr2_a[] = {1.0, 2.0, 5.0, 3.0, 6.0, 9.0};
  cblas_dspr2(CblasColMajor, CblasUpper, 3, 1.0, d_syr_x, 1, d_syr_y, 1,
              d_spr2_a);
  if (!check_double("dspr2 a00", d_spr2_a[0], 9.0)) return 1;
  if (!check_double("dspr2 a01", d_spr2_a[1], 15.0)) return 1;
  if (!check_double("dspr2 a11", d_spr2_a[2], 25.0)) return 1;
  if (!check_double("dspr2 a02", d_spr2_a[3], 21.0)) return 1;
  if (!check_double("dspr2 a12", d_spr2_a[4], 33.0)) return 1;
  if (!check_double("dspr2 a22", d_spr2_a[5], 45.0)) return 1;

  const float s_syr_x[] = {1.0f, 2.0f, 3.0f};
  const float s_syr_y[] = {4.0f, 5.0f, 6.0f};
  float s_syr_a[] = {1.0f, 0.0f, 0.0f, 2.0f, 5.0f, 0.0f, 3.0f, 6.0f, 9.0f};
  cblas_ssyr(CblasColMajor, CblasUpper, 3, 2.0f, s_syr_x, 1, s_syr_a, 3);
  if (!check_float("ssyr a22", s_syr_a[8], 27.0f)) return 1;

  float s_syr2_a[] = {1.0f, 0.0f, 0.0f, 2.0f, 5.0f, 0.0f, 3.0f, 6.0f, 9.0f};
  cblas_ssyr2(CblasColMajor, CblasUpper, 3, 1.0f, s_syr_x, 1, s_syr_y, 1,
              s_syr2_a, 3);
  if (!check_float("ssyr2 a22", s_syr2_a[8], 45.0f)) return 1;

  float s_spr_a[] = {1.0f, 2.0f, 5.0f, 3.0f, 6.0f, 9.0f};
  cblas_sspr(CblasColMajor, CblasUpper, 3, 2.0f, s_syr_x, 1, s_spr_a);
  if (!check_float("sspr a22", s_spr_a[5], 27.0f)) return 1;

  float s_spr2_a[] = {1.0f, 2.0f, 5.0f, 3.0f, 6.0f, 9.0f};
  cblas_sspr2(CblasColMajor, CblasUpper, 3, 1.0f, s_syr_x, 1, s_syr_y, 1,
              s_spr2_a);
  if (!check_float("sspr2 a22", s_spr2_a[5], 45.0f)) return 1;

  const double d_gbmv_a[] = {0.0, 1.0, 3.0, 2.0, 4.0, 6.0, 5.0, 7.0, 0.0};
  const double d_gbmv_x[] = {1.0, 2.0, 3.0};
  double d_gbmv_y[] = {10.0, 20.0, 30.0};
  cblas_dgbmv(CblasColMajor, CblasNoTrans, 3, 3, 1, 1, 2.0, d_gbmv_a, 3,
              d_gbmv_x, 1, 0.5, d_gbmv_y, 1);
  if (!check_double("dgbmv y0", d_gbmv_y[0], 15.0)) return 1;
  if (!check_double("dgbmv y1", d_gbmv_y[1], 62.0)) return 1;
  if (!check_double("dgbmv y2", d_gbmv_y[2], 81.0)) return 1;

  const float s_gbmv_a[] = {0.0f, 1.0f, 3.0f, 2.0f, 4.0f,
                            6.0f, 5.0f, 7.0f, 0.0f};
  const float s_gbmv_x[] = {1.0f, 2.0f, 3.0f};
  float s_gbmv_y[] = {10.0f, 20.0f, 30.0f};
  cblas_sgbmv(CblasColMajor, CblasNoTrans, 3, 3, 1, 1, 2.0f, s_gbmv_a, 3,
              s_gbmv_x, 1, 0.5f, s_gbmv_y, 1);
  if (!check_float("sgbmv y2", s_gbmv_y[2], 81.0f)) return 1;

  const double d_sbmv_a[] = {0.0, 1.0, 2.0, 5.0, 6.0, 9.0};
  double d_sbmv_y[] = {10.0, 20.0, 30.0};
  cblas_dsbmv(CblasColMajor, CblasUpper, 3, 1, 2.0, d_sbmv_a, 2, d_syr_x, 1,
              0.5, d_sbmv_y, 1);
  if (!check_double("dsbmv y0", d_sbmv_y[0], 15.0)) return 1;
  if (!check_double("dsbmv y1", d_sbmv_y[1], 70.0)) return 1;
  if (!check_double("dsbmv y2", d_sbmv_y[2], 93.0)) return 1;

  const float s_sbmv_a[] = {0.0f, 1.0f, 2.0f, 5.0f, 6.0f, 9.0f};
  float s_sbmv_y[] = {10.0f, 20.0f, 30.0f};
  cblas_ssbmv(CblasColMajor, CblasUpper, 3, 1, 2.0f, s_sbmv_a, 2, s_syr_x, 1,
              0.5f, s_sbmv_y, 1);
  if (!check_float("ssbmv y2", s_sbmv_y[2], 93.0f)) return 1;

  const double d_spmv_a[] = {1.0, 2.0, 5.0, 3.0, 6.0, 9.0};
  double d_spmv_y[] = {10.0, 20.0, 30.0};
  cblas_dspmv(CblasColMajor, CblasUpper, 3, 2.0, d_spmv_a, d_syr_x, 1, 0.5,
              d_spmv_y, 1);
  if (!check_double("dspmv y0", d_spmv_y[0], 33.0)) return 1;
  if (!check_double("dspmv y1", d_spmv_y[1], 70.0)) return 1;
  if (!check_double("dspmv y2", d_spmv_y[2], 99.0)) return 1;

  const float s_spmv_a[] = {1.0f, 2.0f, 5.0f, 3.0f, 6.0f, 9.0f};
  float s_spmv_y[] = {10.0f, 20.0f, 30.0f};
  cblas_sspmv(CblasColMajor, CblasUpper, 3, 2.0f, s_spmv_a, s_syr_x, 1, 0.5f,
              s_spmv_y, 1);
  if (!check_float("sspmv y2", s_spmv_y[2], 99.0f)) return 1;

  const double d_tbmv_upper_col_major_a[] = {0.0, 1.0, 2.0, 5.0, 6.0, 9.0};
  double d_tbmv_x[] = {1.0, 2.0, 3.0};
  cblas_dtbmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 3, 1,
              d_tbmv_upper_col_major_a, 2, d_tbmv_x, 1);
  if (!check_double("dtbmv upper col x0", d_tbmv_x[0], 5.0)) return 1;
  if (!check_double("dtbmv upper col x1", d_tbmv_x[1], 28.0)) return 1;
  if (!check_double("dtbmv upper col x2", d_tbmv_x[2], 27.0)) return 1;

  const double d_tbmv_lower_col_major_a[] = {99.0, 4.0, 99.0, 8.0, 99.0, 0.0};
  d_tbmv_x[0] = 1.0;
  d_tbmv_x[1] = 2.0;
  d_tbmv_x[2] = 3.0;
  cblas_dtbmv(CblasColMajor, CblasLower, CblasTrans, CblasUnit, 3, 1,
              d_tbmv_lower_col_major_a, 2, d_tbmv_x, 1);
  if (!check_double("dtbmv lower col trans x0", d_tbmv_x[0], 9.0)) return 1;
  if (!check_double("dtbmv lower col trans x1", d_tbmv_x[1], 26.0)) return 1;
  if (!check_double("dtbmv lower col trans x2", d_tbmv_x[2], 3.0)) return 1;

  double d_tbsv_x[] = {5.0, 28.0, 27.0};
  cblas_dtbsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 3, 1,
              d_tbmv_upper_col_major_a, 2, d_tbsv_x, 1);
  if (!check_double("dtbsv upper col x0", d_tbsv_x[0], 1.0)) return 1;
  if (!check_double("dtbsv upper col x1", d_tbsv_x[1], 2.0)) return 1;
  if (!check_double("dtbsv upper col x2", d_tbsv_x[2], 3.0)) return 1;

  const float s_tbmv_upper_col_major_a[] = {0.0f, 1.0f, 2.0f, 5.0f, 6.0f, 9.0f};
  float s_tbmv_x[] = {1.0f, -99.0f, 2.0f, -99.0f, 3.0f};
  cblas_stbmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 3, 1,
              s_tbmv_upper_col_major_a, 2, s_tbmv_x, 2);
  if (!check_float("stbmv upper col x0", s_tbmv_x[0], 5.0f)) return 1;
  if (!check_float("stbmv upper col x1", s_tbmv_x[2], 28.0f)) return 1;
  if (!check_float("stbmv upper col x2", s_tbmv_x[4], 27.0f)) return 1;

  float s_tbsv_x[] = {5.0f, -99.0f, 28.0f, -99.0f, 27.0f};
  cblas_stbsv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 3, 1,
              s_tbmv_upper_col_major_a, 2, s_tbsv_x, 2);
  if (!check_float("stbsv upper col x0", s_tbsv_x[0], 1.0f)) return 1;
  if (!check_float("stbsv upper col x1", s_tbsv_x[2], 2.0f)) return 1;
  if (!check_float("stbsv upper col x2", s_tbsv_x[4], 3.0f)) return 1;

  const double d_gemm_a[] = {1.0, 4.0, 2.0, 5.0, 3.0, 6.0};
  const double d_gemm_b[] = {7.0, 9.0, 11.0, 8.0, 10.0, 12.0};
  double d_gemm_c[] = {10.0, 20.0, 30.0, 40.0};
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, 2, 2, 3, 2.0,
              d_gemm_a, 2, d_gemm_b, 3, 0.5, d_gemm_c, 2);
  if (!check_double("dgemm c00", d_gemm_c[0], 121.0)) return 1;
  if (!check_double("dgemm c10", d_gemm_c[1], 288.0)) return 1;
  if (!check_double("dgemm c01", d_gemm_c[2], 143.0)) return 1;
  if (!check_double("dgemm c11", d_gemm_c[3], 328.0)) return 1;

  const float s_gemm_a[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  const float s_gemm_b[] = {7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f};
  float s_gemm_c[] = {10.0f, 30.0f, 20.0f, 40.0f};
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 2, 2, 3, 2.0f,
              s_gemm_a, 3, s_gemm_b, 2, 0.5f, s_gemm_c, 2);
  if (!check_float("sgemm c00", s_gemm_c[0], 121.0f)) return 1;
  if (!check_float("sgemm c01", s_gemm_c[1], 143.0f)) return 1;
  if (!check_float("sgemm c10", s_gemm_c[2], 288.0f)) return 1;
  if (!check_float("sgemm c11", s_gemm_c[3], 328.0f)) return 1;

  const double z_gemm_a[] = {1.0, 0.0, 4.0, 0.0, 2.0, 0.0,
                             5.0, 0.0, 3.0, 0.0, 6.0, 0.0};
  const double z_gemm_b[] = {7.0, 0.0, 9.0, 0.0, 11.0, 0.0,
                             8.0, 0.0, 10.0, 0.0, 12.0, 0.0};
  const double z_gemm_alpha[] = {2.0, 0.0};
  const double z_gemm_beta[] = {0.5, 0.0};
  double z_gemm_c[] = {10.0, 0.0, 20.0, 0.0, 30.0, 0.0, 40.0, 0.0};
  cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, 2, 2, 3, z_gemm_alpha,
              z_gemm_a, 2, z_gemm_b, 3, z_gemm_beta, z_gemm_c, 2);
  if (!check_double("zgemm c00 real", z_gemm_c[0], 121.0)) return 1;
  if (!check_double("zgemm c00 imag", z_gemm_c[1], 0.0)) return 1;
  if (!check_double("zgemm c10 real", z_gemm_c[2], 288.0)) return 1;
  if (!check_double("zgemm c10 imag", z_gemm_c[3], 0.0)) return 1;
  if (!check_double("zgemm c01 real", z_gemm_c[4], 143.0)) return 1;
  if (!check_double("zgemm c01 imag", z_gemm_c[5], 0.0)) return 1;
  if (!check_double("zgemm c11 real", z_gemm_c[6], 328.0)) return 1;
  if (!check_double("zgemm c11 imag", z_gemm_c[7], 0.0)) return 1;

  const float c_gemm_a[] = {1.0f, 0.0f, 2.0f, 0.0f, 3.0f, 0.0f,
                            4.0f, 0.0f, 5.0f, 0.0f, 6.0f, 0.0f};
  const float c_gemm_b[] = {7.0f, 0.0f, 8.0f, 0.0f, 9.0f, 0.0f,
                            10.0f, 0.0f, 11.0f, 0.0f, 12.0f, 0.0f};
  const float c_gemm_alpha[] = {2.0f, 0.0f};
  const float c_gemm_beta[] = {0.5f, 0.0f};
  float c_gemm_c[] = {10.0f, 0.0f, 30.0f, 0.0f, 20.0f, 0.0f, 40.0f, 0.0f};
  cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 2, 2, 3, c_gemm_alpha,
              c_gemm_a, 3, c_gemm_b, 2, c_gemm_beta, c_gemm_c, 2);
  if (!check_float("cgemm c00 real", c_gemm_c[0], 121.0f)) return 1;
  if (!check_float("cgemm c00 imag", c_gemm_c[1], 0.0f)) return 1;
  if (!check_float("cgemm c01 real", c_gemm_c[2], 143.0f)) return 1;
  if (!check_float("cgemm c01 imag", c_gemm_c[3], 0.0f)) return 1;
  if (!check_float("cgemm c10 real", c_gemm_c[4], 288.0f)) return 1;
  if (!check_float("cgemm c10 imag", c_gemm_c[5], 0.0f)) return 1;
  if (!check_float("cgemm c11 real", c_gemm_c[6], 328.0f)) return 1;
  if (!check_float("cgemm c11 imag", c_gemm_c[7], 0.0f)) return 1;

  const double z_symm_a[] = {1.0, 0.0, 2.0, 0.0, 2.0, 0.0, 5.0, 0.0};
  const double z_symm_b[] = {1.0, 0.0, 2.0, 0.0, 3.0, 0.0, 4.0, 0.0};
  double z_symm_c[] = {10.0, 0.0, 20.0, 0.0, 30.0, 0.0, 40.0, 0.0};
  cblas_zsymm(CblasColMajor, CblasLeft, CblasUpper, 2, 2, z_gemm_alpha,
              z_symm_a, 2, z_symm_b, 2, z_gemm_beta, z_symm_c, 2);
  if (!check_double("zsymm c00 real", z_symm_c[0], 15.0)) return 1;
  if (!check_double("zsymm c10 real", z_symm_c[2], 34.0)) return 1;
  if (!check_double("zsymm c01 real", z_symm_c[4], 37.0)) return 1;
  if (!check_double("zsymm c11 real", z_symm_c[6], 72.0)) return 1;

  double z_hemm_c[] = {10.0, 0.0, 20.0, 0.0, 30.0, 0.0, 40.0, 0.0};
  cblas_zhemm(CblasColMajor, CblasLeft, CblasUpper, 2, 2, z_gemm_alpha,
              z_symm_a, 2, z_symm_b, 2, z_gemm_beta, z_hemm_c, 2);
  if (!check_double("zhemm c00 real", z_hemm_c[0], 15.0)) return 1;
  if (!check_double("zhemm c10 real", z_hemm_c[2], 34.0)) return 1;
  if (!check_double("zhemm c01 real", z_hemm_c[4], 37.0)) return 1;
  if (!check_double("zhemm c11 real", z_hemm_c[6], 72.0)) return 1;

  const float c_symm_a[] = {1.0f, 0.0f, 2.0f, 0.0f, 2.0f, 0.0f, 5.0f, 0.0f};
  const float c_symm_b[] = {1.0f, 0.0f, 3.0f, 0.0f, 2.0f, 0.0f, 4.0f, 0.0f};
  float c_symm_c[] = {10.0f, 0.0f, 20.0f, 0.0f, 30.0f, 0.0f, 40.0f, 0.0f};
  cblas_csymm(CblasRowMajor, CblasLeft, CblasUpper, 2, 2, c_gemm_alpha,
              c_symm_a, 2, c_symm_b, 2, c_gemm_beta, c_symm_c, 2);
  if (!check_float("csymm c00 real", c_symm_c[0], 15.0f)) return 1;
  if (!check_float("csymm c01 real", c_symm_c[2], 32.0f)) return 1;
  if (!check_float("csymm c10 real", c_symm_c[4], 39.0f)) return 1;
  if (!check_float("csymm c11 real", c_symm_c[6], 72.0f)) return 1;

  float c_hemm_c[] = {10.0f, 0.0f, 20.0f, 0.0f, 30.0f, 0.0f, 40.0f, 0.0f};
  cblas_chemm(CblasRowMajor, CblasLeft, CblasUpper, 2, 2, c_gemm_alpha,
              c_symm_a, 2, c_symm_b, 2, c_gemm_beta, c_hemm_c, 2);
  if (!check_float("chemm c00 real", c_hemm_c[0], 15.0f)) return 1;
  if (!check_float("chemm c01 real", c_hemm_c[2], 32.0f)) return 1;
  if (!check_float("chemm c10 real", c_hemm_c[4], 39.0f)) return 1;
  if (!check_float("chemm c11 real", c_hemm_c[6], 72.0f)) return 1;

  const double d_symm_a[] = {1.0, 2.0, 2.0, 5.0};
  const double d_symm_b[] = {1.0, 2.0, 3.0, 4.0};
  double d_symm_c[] = {10.0, 20.0, 30.0, 40.0};
  cblas_dsymm(CblasColMajor, CblasLeft, CblasUpper, 2, 2, 2.0, d_symm_a,
              2, d_symm_b, 2, 0.5, d_symm_c, 2);
  if (!check_double("dsymm c00", d_symm_c[0], 15.0)) return 1;
  if (!check_double("dsymm c10", d_symm_c[1], 34.0)) return 1;
  if (!check_double("dsymm c01", d_symm_c[2], 37.0)) return 1;
  if (!check_double("dsymm c11", d_symm_c[3], 72.0)) return 1;

  const float s_symm_a[] = {1.0f, 2.0f, 2.0f, 5.0f};
  const float s_symm_b[] = {1.0f, 3.0f, 2.0f, 4.0f};
  float s_symm_c[] = {10.0f, 30.0f, 20.0f, 40.0f};
  cblas_ssymm(CblasRowMajor, CblasLeft, CblasUpper, 2, 2, 2.0f, s_symm_a,
              2, s_symm_b, 2, 0.5f, s_symm_c, 2);
  if (!check_float("ssymm c11", s_symm_c[3], 72.0f)) return 1;

  double d_syrk_c[] = {10.0, 0.0, 30.0, 40.0};
  cblas_dsyrk(CblasColMajor, CblasUpper, CblasNoTrans, 2, 3, 2.0,
              d_gemm_a, 2, 0.5, d_syrk_c, 2);
  if (!check_double("dsyrk c00", d_syrk_c[0], 33.0)) return 1;
  if (!check_double("dsyrk c01", d_syrk_c[2], 79.0)) return 1;
  if (!check_double("dsyrk c11", d_syrk_c[3], 174.0)) return 1;

  float s_syrk_c[] = {10.0f, 30.0f, 0.0f, 40.0f};
  cblas_ssyrk(CblasRowMajor, CblasUpper, CblasNoTrans, 2, 3, 2.0f,
              s_gemm_a, 3, 0.5f, s_syrk_c, 2);
  if (!check_float("ssyrk c11", s_syrk_c[3], 174.0f)) return 1;

  double d_syr2k_c[] = {10.0, 0.0, 30.0, 40.0};
  const double d_syr2k_b[] = {7.0, 10.0, 8.0, 11.0, 9.0, 12.0};
  cblas_dsyr2k(CblasColMajor, CblasUpper, CblasNoTrans, 2, 3, 1.0,
               d_gemm_a, 2, d_syr2k_b, 2, 0.5, d_syr2k_c, 2);
  if (!check_double("dsyr2k c00", d_syr2k_c[0], 105.0)) return 1;
  if (!check_double("dsyr2k c01", d_syr2k_c[2], 205.0)) return 1;
  if (!check_double("dsyr2k c11", d_syr2k_c[3], 354.0)) return 1;

  float s_syr2k_c[] = {10.0f, 30.0f, 0.0f, 40.0f};
  cblas_ssyr2k(CblasRowMajor, CblasUpper, CblasNoTrans, 2, 3, 1.0f,
               s_gemm_a, 3, s_gemm_b, 3, 0.5f, s_syr2k_c, 2);
  if (!check_float("ssyr2k c11", s_syr2k_c[3], 354.0f)) return 1;

  double z_syrk_c[] = {10.0, 0.0, 0.0, 0.0, 30.0, 0.0, 40.0, 0.0};
  cblas_zsyrk(CblasColMajor, CblasUpper, CblasNoTrans, 2, 3, z_gemm_alpha,
              z_gemm_a, 2, z_gemm_beta, z_syrk_c, 2);
  if (!check_double("zsyrk c00 real", z_syrk_c[0], 33.0)) return 1;
  if (!check_double("zsyrk c01 real", z_syrk_c[4], 79.0)) return 1;
  if (!check_double("zsyrk c11 real", z_syrk_c[6], 174.0)) return 1;

  float c_syrk_c[] = {10.0f, 0.0f, 30.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f};
  cblas_csyrk(CblasRowMajor, CblasUpper, CblasNoTrans, 2, 3, c_gemm_alpha,
              c_gemm_a, 3, c_gemm_beta, c_syrk_c, 2);
  if (!check_float("csyrk c00 real", c_syrk_c[0], 33.0f)) return 1;
  if (!check_float("csyrk c01 real", c_syrk_c[2], 79.0f)) return 1;
  if (!check_float("csyrk c11 real", c_syrk_c[6], 174.0f)) return 1;

  const double z_syr2k_b[] = {7.0, 0.0, 10.0, 0.0, 8.0, 0.0,
                              11.0, 0.0, 9.0, 0.0, 12.0, 0.0};
  const double z_alpha_one[] = {1.0, 0.0};
  double z_syr2k_c[] = {10.0, 0.0, 0.0, 0.0, 30.0, 0.0, 40.0, 0.0};
  cblas_zsyr2k(CblasColMajor, CblasUpper, CblasNoTrans, 2, 3, z_alpha_one,
               z_gemm_a, 2, z_syr2k_b, 2, z_gemm_beta, z_syr2k_c, 2);
  if (!check_double("zsyr2k c00 real", z_syr2k_c[0], 105.0)) return 1;
  if (!check_double("zsyr2k c01 real", z_syr2k_c[4], 205.0)) return 1;
  if (!check_double("zsyr2k c11 real", z_syr2k_c[6], 354.0)) return 1;

  const float c_syr2k_b[] = {7.0f, 0.0f, 8.0f, 0.0f, 9.0f, 0.0f,
                             10.0f, 0.0f, 11.0f, 0.0f, 12.0f, 0.0f};
  const float c_alpha_one[] = {1.0f, 0.0f};
  float c_syr2k_c[] = {10.0f, 0.0f, 30.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f};
  cblas_csyr2k(CblasRowMajor, CblasUpper, CblasNoTrans, 2, 3, c_alpha_one,
               c_gemm_a, 3, c_syr2k_b, 3, c_gemm_beta, c_syr2k_c, 2);
  if (!check_float("csyr2k c00 real", c_syr2k_c[0], 105.0f)) return 1;
  if (!check_float("csyr2k c01 real", c_syr2k_c[2], 205.0f)) return 1;
  if (!check_float("csyr2k c11 real", c_syr2k_c[6], 354.0f)) return 1;

  double z_herk_c[] = {10.0, 0.0, 0.0, 0.0, 30.0, 0.0, 40.0, 0.0};
  cblas_zherk(CblasColMajor, CblasUpper, CblasNoTrans, 2, 3, 2.0,
              z_gemm_a, 2, 0.5, z_herk_c, 2);
  if (!check_double("zherk c00 real", z_herk_c[0], 33.0)) return 1;
  if (!check_double("zherk c01 real", z_herk_c[4], 79.0)) return 1;
  if (!check_double("zherk c11 real", z_herk_c[6], 174.0)) return 1;

  float c_herk_c[] = {10.0f, 0.0f, 30.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f};
  cblas_cherk(CblasRowMajor, CblasUpper, CblasNoTrans, 2, 3, 2.0f,
              c_gemm_a, 3, 0.5f, c_herk_c, 2);
  if (!check_float("cherk c00 real", c_herk_c[0], 33.0f)) return 1;
  if (!check_float("cherk c01 real", c_herk_c[2], 79.0f)) return 1;
  if (!check_float("cherk c11 real", c_herk_c[6], 174.0f)) return 1;

  double z_her2k_c[] = {10.0, 0.0, 0.0, 0.0, 30.0, 0.0, 40.0, 0.0};
  cblas_zher2k(CblasColMajor, CblasUpper, CblasNoTrans, 2, 3, z_alpha_one,
               z_gemm_a, 2, z_syr2k_b, 2, 0.5, z_her2k_c, 2);
  if (!check_double("zher2k c00 real", z_her2k_c[0], 105.0)) return 1;
  if (!check_double("zher2k c01 real", z_her2k_c[4], 205.0)) return 1;
  if (!check_double("zher2k c11 real", z_her2k_c[6], 354.0)) return 1;

  float c_her2k_c[] = {10.0f, 0.0f, 30.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f};
  cblas_cher2k(CblasRowMajor, CblasUpper, CblasNoTrans, 2, 3, c_alpha_one,
               c_gemm_a, 3, c_syr2k_b, 3, 0.5f, c_her2k_c, 2);
  if (!check_float("cher2k c00 real", c_her2k_c[0], 105.0f)) return 1;
  if (!check_float("cher2k c01 real", c_her2k_c[2], 205.0f)) return 1;
  if (!check_float("cher2k c11 real", c_her2k_c[6], 354.0f)) return 1;

  const double d_trmm_a[] = {1.0, 0.0, 2.0, 5.0};
  double d_trmm_b[] = {1.0, 2.0, 3.0, 4.0};
  cblas_dtrmm(CblasColMajor, CblasLeft, CblasUpper, CblasNoTrans,
              CblasNonUnit, 2, 2, 2.0, d_trmm_a, 2, d_trmm_b, 2);
  if (!check_double("dtrmm b00", d_trmm_b[0], 10.0)) return 1;
  if (!check_double("dtrmm b10", d_trmm_b[1], 20.0)) return 1;
  if (!check_double("dtrmm b01", d_trmm_b[2], 22.0)) return 1;
  if (!check_double("dtrmm b11", d_trmm_b[3], 40.0)) return 1;

  double d_trsm_b[] = {5.0, 10.0, 11.0, 20.0};
  cblas_dtrsm(CblasColMajor, CblasLeft, CblasUpper, CblasNoTrans,
              CblasNonUnit, 2, 2, 1.0, d_trmm_a, 2, d_trsm_b, 2);
  if (!check_double("dtrsm b00", d_trsm_b[0], 1.0)) return 1;
  if (!check_double("dtrsm b10", d_trsm_b[1], 2.0)) return 1;
  if (!check_double("dtrsm b01", d_trsm_b[2], 3.0)) return 1;
  if (!check_double("dtrsm b11", d_trsm_b[3], 4.0)) return 1;

  const float s_trmm_a[] = {1.0f, 2.0f, 0.0f, 5.0f};
  float s_trmm_b[] = {1.0f, 3.0f, 2.0f, 4.0f};
  cblas_strmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans,
              CblasNonUnit, 2, 2, 2.0f, s_trmm_a, 2, s_trmm_b, 2);
  if (!check_float("strmm b11", s_trmm_b[3], 40.0f)) return 1;

  float s_trsm_b[] = {5.0f, 11.0f, 10.0f, 20.0f};
  cblas_strsm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans,
              CblasNonUnit, 2, 2, 1.0f, s_trmm_a, 2, s_trsm_b, 2);
  if (!check_float("strsm b11", s_trsm_b[3], 4.0f)) return 1;

  const double z_trmm_a[] = {1.0, 0.0, 0.0, 0.0, 2.0, 0.0, 5.0, 0.0};
  double z_trmm_b[] = {1.0, 0.0, 2.0, 0.0, 3.0, 0.0, 4.0, 0.0};
  cblas_ztrmm(CblasColMajor, CblasLeft, CblasUpper, CblasNoTrans,
              CblasNonUnit, 2, 2, z_gemm_alpha, z_trmm_a, 2, z_trmm_b, 2);
  if (!check_double("ztrmm b00 real", z_trmm_b[0], 10.0)) return 1;
  if (!check_double("ztrmm b10 real", z_trmm_b[2], 20.0)) return 1;
  if (!check_double("ztrmm b01 real", z_trmm_b[4], 22.0)) return 1;
  if (!check_double("ztrmm b11 real", z_trmm_b[6], 40.0)) return 1;

  double z_trsm_b[] = {5.0, 0.0, 10.0, 0.0, 11.0, 0.0, 20.0, 0.0};
  cblas_ztrsm(CblasColMajor, CblasLeft, CblasUpper, CblasNoTrans,
              CblasNonUnit, 2, 2, z_alpha_one, z_trmm_a, 2, z_trsm_b, 2);
  if (!check_double("ztrsm b00 real", z_trsm_b[0], 1.0)) return 1;
  if (!check_double("ztrsm b10 real", z_trsm_b[2], 2.0)) return 1;
  if (!check_double("ztrsm b01 real", z_trsm_b[4], 3.0)) return 1;
  if (!check_double("ztrsm b11 real", z_trsm_b[6], 4.0)) return 1;

  const float c_trmm_a[] = {1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f};
  float c_trmm_b[] = {1.0f, 0.0f, 3.0f, 0.0f, 2.0f, 0.0f, 4.0f, 0.0f};
  cblas_ctrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans,
              CblasNonUnit, 2, 2, c_gemm_alpha, c_trmm_a, 2, c_trmm_b, 2);
  if (!check_float("ctrmm b00 real", c_trmm_b[0], 10.0f)) return 1;
  if (!check_float("ctrmm b01 real", c_trmm_b[2], 22.0f)) return 1;
  if (!check_float("ctrmm b10 real", c_trmm_b[4], 20.0f)) return 1;
  if (!check_float("ctrmm b11 real", c_trmm_b[6], 40.0f)) return 1;

  float c_trsm_b[] = {5.0f, 0.0f, 11.0f, 0.0f, 10.0f, 0.0f, 20.0f, 0.0f};
  cblas_ctrsm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans,
              CblasNonUnit, 2, 2, c_alpha_one, c_trmm_a, 2, c_trsm_b, 2);
  if (!check_float("ctrsm b00 real", c_trsm_b[0], 1.0f)) return 1;
  if (!check_float("ctrsm b01 real", c_trsm_b[2], 3.0f)) return 1;
  if (!check_float("ctrsm b10 real", c_trsm_b[4], 2.0f)) return 1;
  if (!check_float("ctrsm b11 real", c_trsm_b[6], 4.0f)) return 1;

  return 0;
}
