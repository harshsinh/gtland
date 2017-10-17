//
// File: sqrtm.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:35:16
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_update.h"
#include "sqrtm.h"
#include "sqrt.h"
#include "schur.h"
#include "UKF_update_rtwutil.h"

// Function Definitions

//
// Arguments    : const double A[36]
//                creal_T X[36]
// Return Type  : void
//
void sqrtm(const double A[36], creal_T X[36])
{
  creal_T Q[36];
  creal_T T[36];
  creal_T R[36];
  int j;
  int exitg4;
  boolean_T p;
  int i;
  int exitg3;
  double x[36];
  double s_re;
  double s_im;
  int k;
  boolean_T exitg2;
  double R_re;
  double R_im;
  double brm;
  double ar;
  double ai;
  boolean_T exitg1;
  schur(A, Q, T);
  for (j = 0; j < 36; j++) {
    R[j].re = 0.0;
    R[j].im = 0.0;
  }

  j = 0;
  do {
    exitg4 = 0;
    if (j + 1 < 7) {
      i = 1;
      do {
        exitg3 = 0;
        if (i <= j) {
          if ((T[(i + 6 * j) - 1].re != 0.0) || (T[(i + 6 * j) - 1].im != 0.0))
          {
            p = false;
            exitg3 = 1;
          } else {
            i++;
          }
        } else {
          j++;
          exitg3 = 2;
        }
      } while (exitg3 == 0);

      if (exitg3 == 1) {
        exitg4 = 1;
      }
    } else {
      p = true;
      exitg4 = 1;
    }
  } while (exitg4 == 0);

  if (p) {
    for (j = 0; j < 6; j++) {
      R[j + 6 * j] = T[j + 6 * j];
      b_sqrt(&R[j + 6 * j]);
    }
  } else {
    for (j = 0; j < 6; j++) {
      R[j + 6 * j] = T[j + 6 * j];
      b_sqrt(&R[j + 6 * j]);
      for (i = j - 1; i + 1 > 0; i--) {
        s_re = 0.0;
        s_im = 0.0;
        for (k = i + 1; k + 1 <= j; k++) {
          s_re += R[i + 6 * k].re * R[k + 6 * j].re - R[i + 6 * k].im * R[k + 6 *
            j].im;
          s_im += R[i + 6 * k].re * R[k + 6 * j].im + R[i + 6 * k].im * R[k + 6 *
            j].re;
        }

        R_re = R[i + 6 * i].re + R[j + 6 * j].re;
        R_im = R[i + 6 * i].im + R[j + 6 * j].im;
        ar = T[i + 6 * j].re - s_re;
        ai = T[i + 6 * j].im - s_im;
        if (R_im == 0.0) {
          if (ai == 0.0) {
            R[i + 6 * j].re = ar / R_re;
            R[i + 6 * j].im = 0.0;
          } else if (ar == 0.0) {
            R[i + 6 * j].re = 0.0;
            R[i + 6 * j].im = ai / R_re;
          } else {
            R[i + 6 * j].re = ar / R_re;
            R[i + 6 * j].im = ai / R_re;
          }
        } else if (R_re == 0.0) {
          if (ar == 0.0) {
            R[i + 6 * j].re = ai / R_im;
            R[i + 6 * j].im = 0.0;
          } else if (ai == 0.0) {
            R[i + 6 * j].re = 0.0;
            R[i + 6 * j].im = -(ar / R_im);
          } else {
            R[i + 6 * j].re = ai / R_im;
            R[i + 6 * j].im = -(ar / R_im);
          }
        } else {
          brm = fabs(R_re);
          s_re = fabs(R_im);
          if (brm > s_re) {
            brm = R_im / R_re;
            s_im = R_re + brm * R_im;
            R[i + 6 * j].re = (ar + brm * ai) / s_im;
            R[i + 6 * j].im = (ai - brm * ar) / s_im;
          } else if (s_re == brm) {
            if (R_re > 0.0) {
              s_im = 0.5;
            } else {
              s_im = -0.5;
            }

            if (R_im > 0.0) {
              s_re = 0.5;
            } else {
              s_re = -0.5;
            }

            R[i + 6 * j].re = (ar * s_im + ai * s_re) / brm;
            R[i + 6 * j].im = (ai * s_im - ar * s_re) / brm;
          } else {
            brm = R_re / R_im;
            s_im = R_im + brm * R_re;
            R[i + 6 * j].re = (brm * ar + ai) / s_im;
            R[i + 6 * j].im = (brm * ai - ar) / s_im;
          }
        }
      }
    }
  }

  for (j = 0; j < 6; j++) {
    for (i = 0; i < 6; i++) {
      T[j + 6 * i].re = 0.0;
      T[j + 6 * i].im = 0.0;
      for (k = 0; k < 6; k++) {
        T[j + 6 * i].re += Q[j + 6 * k].re * R[k + 6 * i].re - Q[j + 6 * k].im *
          R[k + 6 * i].im;
        T[j + 6 * i].im += Q[j + 6 * k].re * R[k + 6 * i].im + Q[j + 6 * k].im *
          R[k + 6 * i].re;
      }
    }

    for (i = 0; i < 6; i++) {
      X[j + 6 * i].re = 0.0;
      X[j + 6 * i].im = 0.0;
      for (k = 0; k < 6; k++) {
        s_re = Q[i + 6 * k].re;
        s_im = -Q[i + 6 * k].im;
        X[j + 6 * i].re += T[j + 6 * k].re * s_re - T[j + 6 * k].im * s_im;
        X[j + 6 * i].im += T[j + 6 * k].re * s_im + T[j + 6 * k].im * s_re;
      }
    }
  }

  for (j = 0; j < 36; j++) {
    x[j] = X[j].im;
  }

  s_re = 0.0;
  j = 0;
  exitg2 = false;
  while ((!exitg2) && (j < 6)) {
    brm = 0.0;
    for (i = 0; i < 6; i++) {
      brm += fabs(x[i + 6 * j]);
    }

    if (rtIsNaN(brm)) {
      s_re = rtNaN;
      exitg2 = true;
    } else {
      if (brm > s_re) {
        s_re = brm;
      }

      j++;
    }
  }

  s_im = 0.0;
  j = 0;
  exitg1 = false;
  while ((!exitg1) && (j < 6)) {
    brm = 0.0;
    for (i = 0; i < 6; i++) {
      brm += rt_hypotd_snf(X[i + 6 * j].re, X[i + 6 * j].im);
    }

    if (rtIsNaN(brm)) {
      s_im = rtNaN;
      exitg1 = true;
    } else {
      if (brm > s_im) {
        s_im = brm;
      }

      j++;
    }
  }

  if (s_re <= 1.3322676295501878E-14 * s_im) {
    for (j = 0; j < 6; j++) {
      for (i = 0; i < 6; i++) {
        X[i + 6 * j].im = 0.0;
      }
    }
  }
}

//
// File trailer for sqrtm.cpp
//
// [EOF]
//
