//
// File: xhseqr.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:35:16
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_update.h"
#include "xhseqr.h"
#include "xdhseqr.h"

// Function Definitions

//
// Arguments    : double h[36]
//                double z[36]
// Return Type  : int
//
int xhseqr(double h[36], double z[36])
{
  int info;
  int istart;
  int j;
  int i;
  info = eml_dlahqr(h, z);
  istart = 4;
  for (j = 0; j < 3; j++) {
    for (i = istart; i < 7; i++) {
      h[(i + 6 * j) - 1] = 0.0;
    }

    istart++;
  }

  return info;
}

//
// File trailer for xhseqr.cpp
//
// [EOF]
//
