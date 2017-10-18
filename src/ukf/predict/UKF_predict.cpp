//
// File: UKF_predict.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:32:57
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_predict.h"
#include "sqrt.h"
#include "UKF_predict_emxutil.h"
#include "schur.h"
#include "UKF_predict_rtwutil.h"

// Function Definitions

//
// Inputs:
//    x, P - state and covariance
//    v, g - control inputs: velocity and gamma (steer angle)
//    Q - covariance matrix for velocity and gamma
//    dt - timestep
//
//  Outputs:
//    x, P - predicted state and covariance
// Arguments    : double x[6]
//                double P[36]
//                double dt
//                const double RE[36]
// Return Type  : void
//
void UKF_predict(double x[6], double P[36], double dt, const double RE[36])
{
  emxArray_real_T *X;
  int k;
  double dv1[36];
  creal_T Q[36];
  creal_T T[36];
  creal_T R[36];
  int j;
  int exitg4;
  boolean_T p;
  int i;
  int exitg3;
  creal_T b_Q[36];
  double P_sqrt[36];
  int i0;
  double s_re;
  double s_im;
  boolean_T exitg2;
  double R_re;
  double R_im;
  double brm;
  double ar;
  double ai;
  boolean_T exitg1;
  emxArray_real_T *b_X;
  emxArray_real_T *c_X;
  double w_m[13];
  double w_c[13];
  double x_star[78];
  static const signed char iv0[6] = { 0, 0, 0, 1, 0, 0 };

  static const signed char iv1[6] = { 0, 0, 0, 0, 1, 0 };

  static const signed char iv2[6] = { 0, 0, 0, 0, 0, 1 };

  double x_bar[6];
  double b_w_c[6];
  double b_x_star[6];
  emxInit_real_T(&X, 2);

  //  <------------------------- TO DO -------------------------->
  // generate sigma points
  //  lambda=0.1;
  //  x_aug=[x;0;0];
  //  X=x_aug;
  //  P_aug=[P,zeros(3,2);
  //      zeros(2,3),Q];
  k = X->size[0] * X->size[1];
  X->size[0] = 6;
  X->size[1] = 1;
  emxEnsureCapacity((emxArray__common *)X, k, (int)sizeof(double));
  for (k = 0; k < 6; k++) {
    X->data[k] = x[k];
  }

  for (k = 0; k < 36; k++) {
    dv1[k] = 5.9999999999504894E-6 * P[k];
  }

  schur(dv1, Q, T);
  for (k = 0; k < 36; k++) {
    R[k].re = 0.0;
    R[k].im = 0.0;
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

  for (k = 0; k < 6; k++) {
    for (i0 = 0; i0 < 6; i0++) {
      b_Q[k + 6 * i0].re = 0.0;
      b_Q[k + 6 * i0].im = 0.0;
      for (j = 0; j < 6; j++) {
        b_Q[k + 6 * i0].re += Q[k + 6 * j].re * R[j + 6 * i0].re - Q[k + 6 * j].
          im * R[j + 6 * i0].im;
        b_Q[k + 6 * i0].im += Q[k + 6 * j].re * R[j + 6 * i0].im + Q[k + 6 * j].
          im * R[j + 6 * i0].re;
      }
    }

    for (i0 = 0; i0 < 6; i0++) {
      T[k + 6 * i0].re = 0.0;
      T[k + 6 * i0].im = 0.0;
      for (j = 0; j < 6; j++) {
        s_re = Q[i0 + 6 * j].re;
        s_im = -Q[i0 + 6 * j].im;
        T[k + 6 * i0].re += b_Q[k + 6 * j].re * s_re - b_Q[k + 6 * j].im * s_im;
        T[k + 6 * i0].im += b_Q[k + 6 * j].re * s_im + b_Q[k + 6 * j].im * s_re;
      }
    }
  }

  for (k = 0; k < 36; k++) {
    P_sqrt[k] = T[k].im;
  }

  s_re = 0.0;
  j = 0;
  exitg2 = false;
  while ((!exitg2) && (j < 6)) {
    brm = 0.0;
    for (i = 0; i < 6; i++) {
      brm += fabs(P_sqrt[i + 6 * j]);
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
      brm += rt_hypotd_snf(T[i + 6 * j].re, T[i + 6 * j].im);
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
        T[i + 6 * j].im = 0.0;
      }
    }
  }

  for (k = 0; k < 36; k++) {
    P_sqrt[k] = T[k].re;
  }

  emxInit_real_T(&b_X, 2);
  for (i = 0; i < 6; i++) {
    k = b_X->size[0] * b_X->size[1];
    b_X->size[0] = 6;
    b_X->size[1] = X->size[1] + 1;
    emxEnsureCapacity((emxArray__common *)b_X, k, (int)sizeof(double));
    j = X->size[1];
    for (k = 0; k < j; k++) {
      for (i0 = 0; i0 < 6; i0++) {
        b_X->data[i0 + b_X->size[0] * k] = X->data[i0 + X->size[0] * k];
      }
    }

    for (k = 0; k < 6; k++) {
      b_X->data[k + b_X->size[0] * X->size[1]] = x[k] + P_sqrt[k + 6 * i];
    }

    k = X->size[0] * X->size[1];
    X->size[0] = 6;
    X->size[1] = b_X->size[1];
    emxEnsureCapacity((emxArray__common *)X, k, (int)sizeof(double));
    j = b_X->size[1];
    for (k = 0; k < j; k++) {
      for (i0 = 0; i0 < 6; i0++) {
        X->data[i0 + X->size[0] * k] = b_X->data[i0 + b_X->size[0] * k];
      }
    }
  }

  emxFree_real_T(&b_X);
  emxInit_real_T(&c_X, 2);
  for (i = 0; i < 6; i++) {
    k = c_X->size[0] * c_X->size[1];
    c_X->size[0] = 6;
    c_X->size[1] = X->size[1] + 1;
    emxEnsureCapacity((emxArray__common *)c_X, k, (int)sizeof(double));
    j = X->size[1];
    for (k = 0; k < j; k++) {
      for (i0 = 0; i0 < 6; i0++) {
        c_X->data[i0 + c_X->size[0] * k] = X->data[i0 + X->size[0] * k];
      }
    }

    for (k = 0; k < 6; k++) {
      c_X->data[k + c_X->size[0] * X->size[1]] = x[k] - P_sqrt[k + 6 * i];
    }

    k = X->size[0] * X->size[1];
    X->size[0] = 6;
    X->size[1] = c_X->size[1];
    emxEnsureCapacity((emxArray__common *)X, k, (int)sizeof(double));
    j = c_X->size[1];
    for (k = 0; k < j; k++) {
      for (i0 = 0; i0 < 6; i0++) {
        X->data[i0 + X->size[0] * k] = c_X->data[i0 + c_X->size[0] * k];
      }
    }
  }

  emxFree_real_T(&c_X);

  // calculate weights
  memset(&w_m[0], 0, 13U * sizeof(double));
  memset(&w_c[0], 0, 13U * sizeof(double));
  w_m[0] = -999999.00000825175;
  w_c[0] = -999996.00000925176;
  for (i = 0; i < 12; i++) {
    w_m[1 + i] = 83333.333334020979;
    w_c[1 + i] = 83333.333334020979;
  }

  P_sqrt[0] = 1.0;
  P_sqrt[6] = 0.0;
  P_sqrt[12] = 0.0;
  P_sqrt[18] = dt;
  P_sqrt[24] = 0.0;
  P_sqrt[30] = 0.0;
  P_sqrt[1] = 0.0;
  P_sqrt[7] = 1.0;
  P_sqrt[13] = 0.0;
  P_sqrt[19] = 0.0;
  P_sqrt[25] = dt;
  P_sqrt[31] = 0.0;
  P_sqrt[2] = 0.0;
  P_sqrt[8] = 0.0;
  P_sqrt[14] = 1.0;
  P_sqrt[20] = 0.0;
  P_sqrt[26] = 0.0;
  P_sqrt[32] = dt;
  for (k = 0; k < 6; k++) {
    P_sqrt[3 + 6 * k] = iv0[k];
    P_sqrt[4 + 6 * k] = iv1[k];
    P_sqrt[5 + 6 * k] = iv2[k];
  }

  for (i = 0; i < 13; i++) {
    for (k = 0; k < 6; k++) {
      x_star[k + 6 * i] = 0.0;
    }

    for (k = 0; k < 6; k++) {
      x_star[k + 6 * i] = 0.0;
      for (i0 = 0; i0 < 6; i0++) {
        x_star[k + 6 * i] += P_sqrt[k + 6 * i0] * X->data[i0 + X->size[0] * i];
      }
    }
  }

  emxFree_real_T(&X);
  for (i = 0; i < 6; i++) {
    x_bar[i] = 0.0;
  }

  memset(&P[0], 0, 36U * sizeof(double));
  for (i = 0; i < 13; i++) {
    for (k = 0; k < 6; k++) {
      x_bar[k] += w_m[i] * x_star[k + 6 * i];
    }
  }

  //  x=real(x_bar);
  //  x(3,1)=pi_to_pi(x(3,1));
  for (i = 0; i < 13; i++) {
    for (k = 0; k < 6; k++) {
      b_w_c[k] = w_c[i] * (x_star[k + 6 * i] - x_bar[k]);
      b_x_star[k] = x_star[k + 6 * i] - x_bar[k];
    }

    for (k = 0; k < 6; k++) {
      for (i0 = 0; i0 < 6; i0++) {
        P[k + 6 * i0] += b_w_c[k] * b_x_star[i0];
      }
    }
  }

  for (k = 0; k < 36; k++) {
    P[k] += RE[k];
  }

  for (k = 0; k < 6; k++) {
    x[k] = x_bar[k];
  }
}

//
// File trailer for UKF_predict.cpp
//
// [EOF]
//
