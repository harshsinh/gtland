//
// File: xnrm2.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:32:57
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_predict.h"
#include "xnrm2.h"

// Function Definitions

//
// Arguments    : int n
//                const double x[3]
// Return Type  : double
//
double b_xnrm2(int n, const double x[3])
{
  double y;
  double scale;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    y = fabs(x[1]);
  } else {
    scale = 2.2250738585072014E-308;
    for (k = 2; k <= n + 1; k++) {
      absxk = fabs(x[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

//
// Arguments    : int n
//                const double x[36]
//                int ix0
// Return Type  : double
//
double xnrm2(int n, const double x[36], int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    y = fabs(x[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

//
// File trailer for xnrm2.cpp
//
// [EOF]
//
