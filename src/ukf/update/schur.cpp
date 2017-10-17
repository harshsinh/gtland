//
// File: schur.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:35:16
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_update.h"
#include "schur.h"
#include "sqrt.h"
#include "xdlanv2.h"
#include "xhseqr.h"
#include "xungorghr.h"
#include "xgehrd.h"
#include "UKF_update_rtwutil.h"

// Function Definitions

//
// Arguments    : const double A[36]
//                creal_T V[36]
//                creal_T T[36]
// Return Type  : void
//
void schur(const double A[36], creal_T V[36], creal_T T[36])
{
  double b_A[36];
  double tau[5];
  double Vr[36];
  int j;
  int m;
  double r;
  double s;
  double t1_re;
  double t1_im;
  double mu1_im;
  double rt1i;
  double mu1_re;
  double rt2i;
  double cs;
  double sn;
  memcpy(&b_A[0], &A[0], 36U * sizeof(double));
  xgehrd(b_A, tau);
  memcpy(&Vr[0], &b_A[0], 36U * sizeof(double));
  xungorghr(Vr, tau);
  xhseqr(b_A, Vr);
  for (j = 0; j < 36; j++) {
    T[j].re = b_A[j];
    T[j].im = 0.0;
    V[j].re = Vr[j];
    V[j].im = 0.0;
  }

  for (m = 4; m >= 0; m += -1) {
    if (b_A[(m + 6 * m) + 1] != 0.0) {
      r = b_A[m + 6 * m];
      s = b_A[m + 6 * (m + 1)];
      t1_re = b_A[(m + 6 * m) + 1];
      t1_im = b_A[(m + 6 * (m + 1)) + 1];
      xdlanv2(&r, &s, &t1_re, &t1_im, &mu1_im, &rt1i, &mu1_re, &rt2i, &cs, &sn);
      mu1_re = mu1_im - b_A[(m + 6 * (m + 1)) + 1];
      r = rt_hypotd_snf(rt_hypotd_snf(mu1_re, rt1i), b_A[(m + 6 * m) + 1]);
      if (rt1i == 0.0) {
        mu1_re /= r;
        mu1_im = 0.0;
      } else if (mu1_re == 0.0) {
        mu1_re = 0.0;
        mu1_im = rt1i / r;
      } else {
        mu1_re /= r;
        mu1_im = rt1i / r;
      }

      s = b_A[(m + 6 * m) + 1] / r;
      for (j = m; j + 1 < 7; j++) {
        t1_re = T[m + 6 * j].re;
        t1_im = T[m + 6 * j].im;
        r = T[m + 6 * j].re;
        T[m + 6 * j].re = (mu1_re * T[m + 6 * j].re + mu1_im * T[m + 6 * j].im)
          + s * T[(m + 6 * j) + 1].re;
        T[m + 6 * j].im = (mu1_re * T[m + 6 * j].im - mu1_im * r) + s * T[(m + 6
          * j) + 1].im;
        r = mu1_re * T[(m + 6 * j) + 1].im + mu1_im * T[(m + 6 * j) + 1].re;
        T[(m + 6 * j) + 1].re = (mu1_re * T[(m + 6 * j) + 1].re - mu1_im * T[(m
          + 6 * j) + 1].im) - s * t1_re;
        T[(m + 6 * j) + 1].im = r - s * t1_im;
      }

      for (j = 0; j + 1 <= m + 2; j++) {
        t1_re = T[j + 6 * m].re;
        t1_im = T[j + 6 * m].im;
        r = mu1_re * T[j + 6 * m].im + mu1_im * T[j + 6 * m].re;
        T[j + 6 * m].re = (mu1_re * T[j + 6 * m].re - mu1_im * T[j + 6 * m].im)
          + s * T[j + 6 * (m + 1)].re;
        T[j + 6 * m].im = r + s * T[j + 6 * (m + 1)].im;
        r = T[j + 6 * (m + 1)].re;
        T[j + 6 * (m + 1)].re = (mu1_re * T[j + 6 * (m + 1)].re + mu1_im * T[j +
          6 * (m + 1)].im) - s * t1_re;
        T[j + 6 * (m + 1)].im = (mu1_re * T[j + 6 * (m + 1)].im - mu1_im * r) -
          s * t1_im;
      }

      for (j = 0; j < 6; j++) {
        t1_re = V[j + 6 * m].re;
        t1_im = V[j + 6 * m].im;
        r = mu1_re * V[j + 6 * m].im + mu1_im * V[j + 6 * m].re;
        V[j + 6 * m].re = (mu1_re * V[j + 6 * m].re - mu1_im * V[j + 6 * m].im)
          + s * V[j + 6 * (m + 1)].re;
        V[j + 6 * m].im = r + s * V[j + 6 * (m + 1)].im;
        r = V[j + 6 * (m + 1)].re;
        V[j + 6 * (m + 1)].re = (mu1_re * V[j + 6 * (m + 1)].re + mu1_im * V[j +
          6 * (m + 1)].im) - s * t1_re;
        V[j + 6 * (m + 1)].im = (mu1_re * V[j + 6 * (m + 1)].im - mu1_im * r) -
          s * t1_im;
      }

      T[(m + 6 * m) + 1].re = 0.0;
      T[(m + 6 * m) + 1].im = 0.0;
    }
  }
}

//
// File trailer for schur.cpp
//
// [EOF]
//
