//
// File: UKF_update.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:35:16
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_update.h"
#include "UKF_update_emxutil.h"
#include "sqrtm.h"

// Function Definitions

//
// correcting the predicted pose using Kalman gain.
// Arguments    : double x[6]
//                double P[36]
//                const double z[3]
//                const double QE[9]
//                double R_actual
//                double f
//                double cx
//                double cy
// Return Type  : void
//
void UKF_update(double x[6], double P[36], const double z[3], const double QE[9],
                double R_actual, double f, double cx, double cy)
{
  emxArray_real_T *X;
  int p2;
  double dv2[36];
  creal_T dcv0[36];
  double P_sqrt[36];
  emxArray_real_T *b_X;
  int i;
  emxArray_real_T *c_X;
  int p1;
  double w_m[13];
  double w_c[13];
  int p3;
  double Z[39];
  double z_hat[3];
  double S[9];
  double b_w_c[3];
  double b_z[3];
  double P_bar[18];
  double c_w_c[6];
  double b_x[9];
  double absx11;
  double absx21;
  double absx31;
  int itmp;
  double y[9];
  double K[18];
  double b_K[18];
  emxInit_real_T(&X, 2);

  //  Inputs:
  //    x, P -  state and covariance
  //    z, R - range-bearing measurements and covariances
  //    idf - indecies of each landmark from which measurements have arrived
  //  Outputs:
  //    x, P - updated state and covariance
  //  <---------------------------- TO DO -------------------------->
  // generate sigma points
  //  lambda=1;
  //  x(3,1)=pi_to_pi(x(3,1));
  p2 = X->size[0] * X->size[1];
  X->size[0] = 6;
  X->size[1] = 1;
  emxEnsureCapacity((emxArray__common *)X, p2, (int)sizeof(double));
  for (p2 = 0; p2 < 6; p2++) {
    X->data[p2] = x[p2];
  }

  for (p2 = 0; p2 < 36; p2++) {
    dv2[p2] = 5.9999999999504894E-6 * P[p2];
  }

  sqrtm(dv2, dcv0);
  for (p2 = 0; p2 < 36; p2++) {
    P_sqrt[p2] = dcv0[p2].re;
  }

  emxInit_real_T(&b_X, 2);
  for (i = 0; i < 6; i++) {
    p2 = b_X->size[0] * b_X->size[1];
    b_X->size[0] = 6;
    b_X->size[1] = X->size[1] + 1;
    emxEnsureCapacity((emxArray__common *)b_X, p2, (int)sizeof(double));
    p1 = X->size[1];
    for (p2 = 0; p2 < p1; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        b_X->data[p3 + b_X->size[0] * p2] = X->data[p3 + X->size[0] * p2];
      }
    }

    for (p2 = 0; p2 < 6; p2++) {
      b_X->data[p2 + b_X->size[0] * X->size[1]] = x[p2] + P_sqrt[p2 + 6 * i];
    }

    p2 = X->size[0] * X->size[1];
    X->size[0] = 6;
    X->size[1] = b_X->size[1];
    emxEnsureCapacity((emxArray__common *)X, p2, (int)sizeof(double));
    p1 = b_X->size[1];
    for (p2 = 0; p2 < p1; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        X->data[p3 + X->size[0] * p2] = b_X->data[p3 + b_X->size[0] * p2];
      }
    }
  }

  emxFree_real_T(&b_X);
  emxInit_real_T(&c_X, 2);
  for (i = 0; i < 6; i++) {
    p2 = c_X->size[0] * c_X->size[1];
    c_X->size[0] = 6;
    c_X->size[1] = X->size[1] + 1;
    emxEnsureCapacity((emxArray__common *)c_X, p2, (int)sizeof(double));
    p1 = X->size[1];
    for (p2 = 0; p2 < p1; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        c_X->data[p3 + c_X->size[0] * p2] = X->data[p3 + X->size[0] * p2];
      }
    }

    for (p2 = 0; p2 < 6; p2++) {
      c_X->data[p2 + c_X->size[0] * X->size[1]] = x[p2] - P_sqrt[p2 + 6 * i];
    }

    p2 = X->size[0] * X->size[1];
    X->size[0] = 6;
    X->size[1] = c_X->size[1];
    emxEnsureCapacity((emxArray__common *)X, p2, (int)sizeof(double));
    p1 = c_X->size[1];
    for (p2 = 0; p2 < p1; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        X->data[p3 + X->size[0] * p2] = c_X->data[p3 + c_X->size[0] * p2];
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

  for (i = 0; i < 13; i++) {
    Z[3 * i] = R_actual * f / sqrt((X->data[X->size[0] * i] * X->data[X->size[0]
      * i] + X->data[1 + X->size[0] * i] * X->data[1 + X->size[0] * i]) +
      X->data[2 + X->size[0] * i] * X->data[2 + X->size[0] * i]);
    Z[1 + 3 * i] = f * X->data[X->size[0] * i] / X->data[2 + X->size[0] * i] +
      cx;
    Z[2 + 3 * i] = f * X->data[1 + X->size[0] * i] / X->data[2 + X->size[0] * i]
      + cy;
  }

  for (i = 0; i < 3; i++) {
    z_hat[i] = 0.0;
  }

  for (i = 0; i < 13; i++) {
    for (p2 = 0; p2 < 3; p2++) {
      z_hat[p2] += w_m[i] * Z[p2 + 3 * i];
    }
  }

  memset(&S[0], 0, 9U * sizeof(double));
  for (i = 0; i < 13; i++) {
    for (p2 = 0; p2 < 3; p2++) {
      b_w_c[p2] = w_c[i] * (Z[p2 + 3 * i] - z_hat[p2]);
      b_z[p2] = Z[p2 + 3 * i] - z_hat[p2];
    }

    for (p2 = 0; p2 < 3; p2++) {
      for (p3 = 0; p3 < 3; p3++) {
        S[p2 + 3 * p3] += b_w_c[p2] * b_z[p3];
      }
    }
  }

  for (p2 = 0; p2 < 9; p2++) {
    S[p2] += QE[p2];
  }

  memset(&P_bar[0], 0, 18U * sizeof(double));
  for (i = 0; i < 13; i++) {
    for (p2 = 0; p2 < 6; p2++) {
      c_w_c[p2] = w_c[i] * (X->data[p2 + X->size[0] * i] - x[p2]);
    }

    for (p2 = 0; p2 < 3; p2++) {
      b_z[p2] = Z[p2 + 3 * i] - z_hat[p2];
    }

    for (p2 = 0; p2 < 6; p2++) {
      for (p3 = 0; p3 < 3; p3++) {
        P_bar[p2 + 6 * p3] += c_w_c[p2] * b_z[p3];
      }
    }
  }

  emxFree_real_T(&X);
  memcpy(&b_x[0], &S[0], 9U * sizeof(double));
  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = fabs(S[0]);
  absx21 = fabs(S[1]);
  absx31 = fabs(S[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    b_x[0] = S[1];
    b_x[1] = S[0];
    b_x[3] = S[4];
    b_x[4] = S[3];
    b_x[6] = S[7];
    b_x[7] = S[6];
  } else {
    if (absx31 > absx11) {
      p1 = 6;
      p3 = 0;
      b_x[0] = S[2];
      b_x[2] = S[0];
      b_x[3] = S[5];
      b_x[5] = S[3];
      b_x[6] = S[8];
      b_x[8] = S[6];
    }
  }

  absx11 = b_x[1] / b_x[0];
  b_x[1] /= b_x[0];
  absx21 = b_x[2] / b_x[0];
  b_x[2] /= b_x[0];
  b_x[4] -= absx11 * b_x[3];
  b_x[5] -= absx21 * b_x[3];
  b_x[7] -= absx11 * b_x[6];
  b_x[8] -= absx21 * b_x[6];
  if (fabs(b_x[5]) > fabs(b_x[4])) {
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    b_x[1] = absx21;
    b_x[2] = absx11;
    absx11 = b_x[4];
    b_x[4] = b_x[5];
    b_x[5] = absx11;
    absx11 = b_x[7];
    b_x[7] = b_x[8];
    b_x[8] = absx11;
  }

  absx11 = b_x[5] / b_x[4];
  b_x[5] /= b_x[4];
  b_x[8] -= absx11 * b_x[7];
  absx11 = (b_x[5] * b_x[1] - b_x[2]) / b_x[8];
  absx21 = -(b_x[1] + b_x[7] * absx11) / b_x[4];
  y[p1] = ((1.0 - b_x[3] * absx21) - b_x[6] * absx11) / b_x[0];
  y[p1 + 1] = absx21;
  y[p1 + 2] = absx11;
  absx11 = -b_x[5] / b_x[8];
  absx21 = (1.0 - b_x[7] * absx11) / b_x[4];
  y[p2] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p2 + 1] = absx21;
  y[p2 + 2] = absx11;
  absx11 = 1.0 / b_x[8];
  absx21 = -b_x[7] * absx11 / b_x[4];
  y[p3] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p3 + 1] = absx21;
  y[p3 + 2] = absx11;
  for (p2 = 0; p2 < 6; p2++) {
    for (p3 = 0; p3 < 3; p3++) {
      K[p2 + 6 * p3] = 0.0;
      for (p1 = 0; p1 < 3; p1++) {
        K[p2 + 6 * p3] += P_bar[p2 + 6 * p1] * y[p1 + 3 * p3];
      }
    }
  }

  for (p2 = 0; p2 < 3; p2++) {
    b_z[p2] = z[p2] - z_hat[p2];
  }

  for (p2 = 0; p2 < 6; p2++) {
    absx11 = 0.0;
    for (p3 = 0; p3 < 3; p3++) {
      absx11 += K[p2 + 6 * p3] * b_z[p3];
      b_K[p2 + 6 * p3] = 0.0;
      for (p1 = 0; p1 < 3; p1++) {
        b_K[p2 + 6 * p3] += K[p2 + 6 * p1] * S[p1 + 3 * p3];
      }
    }

    x[p2] += absx11;
    for (p3 = 0; p3 < 6; p3++) {
      absx11 = 0.0;
      for (p1 = 0; p1 < 3; p1++) {
        absx11 += b_K[p2 + 6 * p1] * K[p3 + 6 * p1];
      }

      P[p2 + 6 * p3] -= absx11;
    }
  }
}

//
// File trailer for UKF_update.cpp
//
// [EOF]
//
