//
// File: xzlarfg.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:35:16
//

// Include Files
#include "rt_nonfinite.h"
#include "../update/UKF_update.h"
#include "xzlarfg.h"
#include "sqrt.h"
#include "xnrm2.h"
#include "rtwutil.h"

// Function Definitions

//
// Arguments    : int n
//                double *alpha1
//                double x[3]
// Return Type  : double
//
double xzlarfg(int n, double *alpha1, double x[3])
{
  double tau;
  double xnorm;
  int knt;
  int k;
  tau = 0.0;
  if (n <= 0) {
  } else {
    xnorm = b_xnrm2(n - 1, x);
    if (xnorm != 0.0) {
      xnorm = rt_hypotd_snf(*alpha1, xnorm);
      if (*alpha1 >= 0.0) {
        xnorm = -xnorm;
      }

      if (fabs(xnorm) < 1.0020841800044864E-292) {
        knt = 0;
        do {
          knt++;
          for (k = 1; k + 1 <= n; k++) {
            x[k] *= 9.9792015476736E+291;
          }

          xnorm *= 9.9792015476736E+291;
          *alpha1 *= 9.9792015476736E+291;
        } while (!(fabs(xnorm) >= 1.0020841800044864E-292));

        xnorm = rt_hypotd_snf(*alpha1, b_xnrm2(n - 1, x));
        if (*alpha1 >= 0.0) {
          xnorm = -xnorm;
        }

        tau = (xnorm - *alpha1) / xnorm;
        *alpha1 = 1.0 / (*alpha1 - xnorm);
        for (k = 1; k + 1 <= n; k++) {
          x[k] *= *alpha1;
        }

        for (k = 1; k <= knt; k++) {
          xnorm *= 1.0020841800044864E-292;
        }

        *alpha1 = xnorm;
      } else {
        tau = (xnorm - *alpha1) / xnorm;
        *alpha1 = 1.0 / (*alpha1 - xnorm);
        for (k = 1; k + 1 <= n; k++) {
          x[k] *= *alpha1;
        }

        *alpha1 = xnorm;
      }
    }
  }

  return tau;
}

//
// File trailer for xzlarfg.cpp
//
// [EOF]
//
