//
// File: xgehrd.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 18-Oct-2017 21:20:19
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_main_c.h"
#include "xgehrd.h"
#include "xgerc.h"
#include "sqrt.h"
#include "xnrm2.h"
#include "UKF_main_c_rtwutil.h"

// Function Definitions

//
// Arguments    : double a[36]
//                double tau[5]
// Return Type  : void
//
void xgehrd(double a[36], double tau[5])
{
  double work[6];
  int i;
  int im1n;
  int in;
  int b_i;
  int ia0;
  double alpha1;
  double d0;
  double xnorm;
  int jy;
  int knt;
  int lastv;
  int lastc;
  int i0;
  int k;
  boolean_T exitg4;
  int ix;
  int ia;
  int exitg3;
  boolean_T exitg2;
  int i1;
  int exitg1;
  for (i = 0; i < 6; i++) {
    work[i] = 0.0;
  }

  for (i = 0; i < 5; i++) {
    im1n = i * 6 + 2;
    in = (i + 1) * 6;
    if (i + 3 <= 6) {
      b_i = i + 3;
    } else {
      b_i = 6;
    }

    ia0 = b_i + i * 6;
    alpha1 = a[(i + 6 * i) + 1];
    d0 = 0.0;
    xnorm = xnrm2(4 - i, a, ia0);
    if (xnorm != 0.0) {
      xnorm = rt_hypotd_snf(a[(i + 6 * i) + 1], xnorm);
      if (a[(i + 6 * i) + 1] >= 0.0) {
        xnorm = -xnorm;
      }

      if (fabs(xnorm) < 1.0020841800044864E-292) {
        knt = 0;
        do {
          knt++;
          i0 = (ia0 - i) + 3;
          for (k = ia0; k <= i0; k++) {
            a[k - 1] *= 9.9792015476736E+291;
          }

          xnorm *= 9.9792015476736E+291;
          alpha1 *= 9.9792015476736E+291;
        } while (!(fabs(xnorm) >= 1.0020841800044864E-292));

        xnorm = rt_hypotd_snf(alpha1, xnrm2(4 - i, a, ia0));
        if (alpha1 >= 0.0) {
          xnorm = -xnorm;
        }

        d0 = (xnorm - alpha1) / xnorm;
        alpha1 = 1.0 / (alpha1 - xnorm);
        i0 = (ia0 - i) + 3;
        while (ia0 <= i0) {
          a[ia0 - 1] *= alpha1;
          ia0++;
        }

        for (k = 1; k <= knt; k++) {
          xnorm *= 1.0020841800044864E-292;
        }

        alpha1 = xnorm;
      } else {
        d0 = (xnorm - a[(i + 6 * i) + 1]) / xnorm;
        alpha1 = 1.0 / (a[(i + 6 * i) + 1] - xnorm);
        i0 = (ia0 - i) + 3;
        while (ia0 <= i0) {
          a[ia0 - 1] *= alpha1;
          ia0++;
        }

        alpha1 = xnorm;
      }
    }

    tau[i] = d0;
    a[(i + 6 * i) + 1] = 1.0;
    jy = (i + im1n) - 1;
    if (tau[i] != 0.0) {
      lastv = 5 - i;
      ia0 = (jy - i) + 4;
      while ((lastv > 0) && (a[ia0] == 0.0)) {
        lastv--;
        ia0--;
      }

      lastc = 6;
      exitg4 = false;
      while ((!exitg4) && (lastc > 0)) {
        ia0 = in + lastc;
        ia = ia0;
        do {
          exitg3 = 0;
          if (ia <= ia0 + (lastv - 1) * 6) {
            if (a[ia - 1] != 0.0) {
              exitg3 = 1;
            } else {
              ia += 6;
            }
          } else {
            lastc--;
            exitg3 = 2;
          }
        } while (exitg3 == 0);

        if (exitg3 == 1) {
          exitg4 = true;
        }
      }
    } else {
      lastv = 0;
      lastc = 0;
    }

    if (lastv > 0) {
      if (lastc == 0) {
      } else {
        for (ia0 = 1; ia0 <= lastc; ia0++) {
          work[ia0 - 1] = 0.0;
        }

        ix = jy;
        i0 = (in + 6 * (lastv - 1)) + 1;
        for (k = in + 1; k <= i0; k += 6) {
          ia0 = 0;
          i1 = (k + lastc) - 1;
          for (ia = k; ia <= i1; ia++) {
            work[ia0] += a[ia - 1] * a[ix];
            ia0++;
          }

          ix++;
        }
      }

      if (-tau[i] == 0.0) {
      } else {
        ia0 = in;
        for (knt = 1; knt <= lastv; knt++) {
          if (a[jy] != 0.0) {
            xnorm = a[jy] * -tau[i];
            ix = 0;
            i0 = lastc + ia0;
            for (k = ia0; k + 1 <= i0; k++) {
              a[k] += work[ix] * xnorm;
              ix++;
            }
          }

          jy++;
          ia0 += 6;
        }
      }
    }

    jy = i + im1n;
    knt = (i + in) + 2;
    if (tau[i] != 0.0) {
      lastv = 5 - i;
      ia0 = (jy - i) + 3;
      while ((lastv > 0) && (a[ia0] == 0.0)) {
        lastv--;
        ia0--;
      }

      lastc = 5 - i;
      exitg2 = false;
      while ((!exitg2) && (lastc > 0)) {
        ia0 = knt + (lastc - 1) * 6;
        ia = ia0;
        do {
          exitg1 = 0;
          if (ia <= (ia0 + lastv) - 1) {
            if (a[ia - 1] != 0.0) {
              exitg1 = 1;
            } else {
              ia++;
            }
          } else {
            lastc--;
            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = true;
        }
      }
    } else {
      lastv = 0;
      lastc = 0;
    }

    if (lastv > 0) {
      if (lastc == 0) {
      } else {
        for (ia0 = 1; ia0 <= lastc; ia0++) {
          work[ia0 - 1] = 0.0;
        }

        ia0 = 0;
        i0 = knt + 6 * (lastc - 1);
        for (k = knt; k <= i0; k += 6) {
          ix = jy;
          xnorm = 0.0;
          i1 = (k + lastv) - 1;
          for (ia = k; ia <= i1; ia++) {
            xnorm += a[ia - 1] * a[ix - 1];
            ix++;
          }

          work[ia0] += xnorm;
          ia0++;
        }
      }

      xgerc(lastv, lastc, -tau[i], jy, work, a, knt);
    }

    a[(i + 6 * i) + 1] = alpha1;
  }
}

//
// File trailer for xgehrd.cpp
//
// [EOF]
//
