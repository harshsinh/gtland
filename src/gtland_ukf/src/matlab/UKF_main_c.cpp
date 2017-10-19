//
// File: UKF_main_c.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 18-Oct-2017 21:20:19
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_main_c.h"
#include "UKF_main_c_emxutil.h"
#include "sqrtm.h"

// Function Definitions

//
// UNTITLED Summary of this function goes here
//    Detailed explanation goes here
//  if (first_time == 1){
//  d=R_actual*f/R(1,1);
//  z=d*f/(((U(1,1)-cx)^2+(V(1,1)-cy)^2+f^2)^0.5);
//
//  x=[(U(1,1)-cx)*z/f;
//      (V(1,1)-cy)*z/f;
//      z;0;0;0];
//  first_time=0;
//  }
//  P=0.01*[eye(3) zeros(3)
//      zeros(3) eye(3)];
// Arguments    : double x[6]
//                double P[36]
//                const double z[3]
//                double f
//                double cx
//                double cy
//                double R_actual
//                const double QE[9]
//                const double RE[36]
//                double dt
// Return Type  : void
//
void UKF_main_c(double x[6], double P[36], const double z[3], double f, double
                cx, double cy, double R_actual, const double QE[9], const double
                RE[36], double dt)
{
  emxArray_real_T *X;
  int p2;
  double dv3[36];
  creal_T dcv0[36];
  double P_sqrt[36];
  emxArray_real_T *b_X;
  int i;
  emxArray_real_T *c_X;
  int p1;
  double w_m[13];
  double w_c[13];
  int p3;
  double x_star[78];
  static const signed char iv0[6] = { 0, 0, 0, 1, 0, 0 };

  static const signed char iv1[6] = { 0, 0, 0, 0, 1, 0 };

  static const signed char iv2[6] = { 0, 0, 0, 0, 0, 1 };

  double P_bar[36];
  double x_bar[6];
  double b_w_c[6];
  double b_x_star[6];
  emxArray_real_T *d_X;
  emxArray_real_T *e_X;
  double Z[39];
  double z_hat[3];
  double S[9];
  double c_w_c[3];
  double b_z[3];
  double b_P_bar[18];
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
  //    x, P - state and covariance
  //    v, g - control inputs: velocity and gamma (steer angle)
  //    Q - covariance matrix for velocity and gamma
  //    dt - timestep
  //
  //  Outputs:
  //    x, P - predicted state and covariance
  //  <------------------------- TO DO -------------------------->
  // generate sigma points
  //  lambda=0.1;
  //  x_aug=[x;0;0];
  //  X=x_aug;
  //  P_aug=[P,zeros(3,2);
  //      zeros(2,3),Q];
  p2 = X->size[0] * X->size[1];
  X->size[0] = 6;
  X->size[1] = 1;
  emxEnsureCapacity((emxArray__common *)X, p2, (int)sizeof(double));
  for (p2 = 0; p2 < 6; p2++) {
    X->data[p2] = x[p2];
  }

  for (p2 = 0; p2 < 36; p2++) {
    dv3[p2] = 5.9999999999504894E-6 * P[p2];
  }

  sqrtm(dv3, dcv0);
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
  for (p2 = 0; p2 < 6; p2++) {
    P_sqrt[3 + 6 * p2] = iv0[p2];
    P_sqrt[4 + 6 * p2] = iv1[p2];
    P_sqrt[5 + 6 * p2] = iv2[p2];
  }

  for (i = 0; i < 13; i++) {
    for (p2 = 0; p2 < 6; p2++) {
      x_star[p2 + 6 * i] = 0.0;
    }

    for (p2 = 0; p2 < 6; p2++) {
      x_star[p2 + 6 * i] = 0.0;
      for (p3 = 0; p3 < 6; p3++) {
        x_star[p2 + 6 * i] += P_sqrt[p2 + 6 * p3] * X->data[p3 + X->size[0] * i];
      }
    }
  }

  for (i = 0; i < 6; i++) {
    x_bar[i] = 0.0;
  }

  memset(&P_bar[0], 0, 36U * sizeof(double));
  for (i = 0; i < 13; i++) {
    for (p2 = 0; p2 < 6; p2++) {
      x_bar[p2] += w_m[i] * x_star[p2 + 6 * i];
    }
  }

  //  x=real(x_bar);
  //  x(3,1)=pi_to_pi(x(3,1));
  for (i = 0; i < 13; i++) {
    for (p2 = 0; p2 < 6; p2++) {
      b_w_c[p2] = w_c[i] * (x_star[p2 + 6 * i] - x_bar[p2]);
      b_x_star[p2] = x_star[p2 + 6 * i] - x_bar[p2];
    }

    for (p2 = 0; p2 < 6; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        P_bar[p2 + 6 * p3] += b_w_c[p2] * b_x_star[p3];
      }
    }
  }

  for (p2 = 0; p2 < 36; p2++) {
    P_bar[p2] += RE[p2];
  }

  //  correcting the predicted pose using Kalman gain.
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
    dv3[p2] = 5.9999999999504894E-6 * P_bar[p2];
  }

  sqrtm(dv3, dcv0);
  for (p2 = 0; p2 < 36; p2++) {
    P_sqrt[p2] = dcv0[p2].re;
  }

  emxInit_real_T(&d_X, 2);
  for (i = 0; i < 6; i++) {
    p2 = d_X->size[0] * d_X->size[1];
    d_X->size[0] = 6;
    d_X->size[1] = X->size[1] + 1;
    emxEnsureCapacity((emxArray__common *)d_X, p2, (int)sizeof(double));
    p1 = X->size[1];
    for (p2 = 0; p2 < p1; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        d_X->data[p3 + d_X->size[0] * p2] = X->data[p3 + X->size[0] * p2];
      }
    }

    for (p2 = 0; p2 < 6; p2++) {
      d_X->data[p2 + d_X->size[0] * X->size[1]] = x[p2] + P_sqrt[p2 + 6 * i];
    }

    p2 = X->size[0] * X->size[1];
    X->size[0] = 6;
    X->size[1] = d_X->size[1];
    emxEnsureCapacity((emxArray__common *)X, p2, (int)sizeof(double));
    p1 = d_X->size[1];
    for (p2 = 0; p2 < p1; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        X->data[p3 + X->size[0] * p2] = d_X->data[p3 + d_X->size[0] * p2];
      }
    }
  }

  emxFree_real_T(&d_X);
  emxInit_real_T(&e_X, 2);
  for (i = 0; i < 6; i++) {
    p2 = e_X->size[0] * e_X->size[1];
    e_X->size[0] = 6;
    e_X->size[1] = X->size[1] + 1;
    emxEnsureCapacity((emxArray__common *)e_X, p2, (int)sizeof(double));
    p1 = X->size[1];
    for (p2 = 0; p2 < p1; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        e_X->data[p3 + e_X->size[0] * p2] = X->data[p3 + X->size[0] * p2];
      }
    }

    for (p2 = 0; p2 < 6; p2++) {
      e_X->data[p2 + e_X->size[0] * X->size[1]] = x[p2] - P_sqrt[p2 + 6 * i];
    }

    p2 = X->size[0] * X->size[1];
    X->size[0] = 6;
    X->size[1] = e_X->size[1];
    emxEnsureCapacity((emxArray__common *)X, p2, (int)sizeof(double));
    p1 = e_X->size[1];
    for (p2 = 0; p2 < p1; p2++) {
      for (p3 = 0; p3 < 6; p3++) {
        X->data[p3 + X->size[0] * p2] = e_X->data[p3 + e_X->size[0] * p2];
      }
    }
  }

  emxFree_real_T(&e_X);

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
      c_w_c[p2] = w_c[i] * (Z[p2 + 3 * i] - z_hat[p2]);
      b_z[p2] = Z[p2 + 3 * i] - z_hat[p2];
    }

    for (p2 = 0; p2 < 3; p2++) {
      for (p3 = 0; p3 < 3; p3++) {
        S[p2 + 3 * p3] += c_w_c[p2] * b_z[p3];
      }
    }
  }

  for (p2 = 0; p2 < 9; p2++) {
    S[p2] += QE[p2];
  }

  memset(&b_P_bar[0], 0, 18U * sizeof(double));
  for (i = 0; i < 13; i++) {
    for (p2 = 0; p2 < 6; p2++) {
      b_w_c[p2] = w_c[i] * (X->data[p2 + X->size[0] * i] - x[p2]);
    }

    for (p2 = 0; p2 < 3; p2++) {
      b_z[p2] = Z[p2 + 3 * i] - z_hat[p2];
    }

    for (p2 = 0; p2 < 6; p2++) {
      for (p3 = 0; p3 < 3; p3++) {
        b_P_bar[p2 + 6 * p3] += b_w_c[p2] * b_z[p3];
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
        K[p2 + 6 * p3] += b_P_bar[p2 + 6 * p1] * y[p1 + 3 * p3];
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

      P[p2 + 6 * p3] = P_bar[p2 + 6 * p3] - absx11;
    }
  }
}

//
// File trailer for UKF_main_c.cpp
//
// [EOF]
//
