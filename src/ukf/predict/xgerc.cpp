//
// File: xgerc.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:32:57
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_predict.h"
#include "xgerc.h"

// Function Definitions

//
// Arguments    : int m
//                int n
//                double alpha1
//                int ix0
//                const double y[6]
//                double A[36]
//                int ia0
// Return Type  : void
//
void xgerc(int m, int n, double alpha1, int ix0, const double y[6], double A[36],
           int ia0)
{
  int jA;
  int jy;
  int j;
  double temp;
  int ix;
  int i3;
  int ijA;
  if (alpha1 == 0.0) {
  } else {
    jA = ia0 - 1;
    jy = 0;
    for (j = 1; j <= n; j++) {
      if (y[jy] != 0.0) {
        temp = y[jy] * alpha1;
        ix = ix0;
        i3 = m + jA;
        for (ijA = jA; ijA + 1 <= i3; ijA++) {
          A[ijA] += A[ix - 1] * temp;
          ix++;
        }
      }

      jy++;
      jA += 6;
    }
  }
}

//
// File trailer for xgerc.cpp
//
// [EOF]
//
