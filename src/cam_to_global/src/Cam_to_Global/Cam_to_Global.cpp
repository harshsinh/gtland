//
// File: Cam_to_Global.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 18-Oct-2017 11:55:15
//

// Include Files
#include "rt_nonfinite.h"
#include "Cam_to_Global.h"

// Function Definitions

//
// UNTITLED2 Summary of this function goes here
//    Detailed explanation goes here
// Arguments    : const double x[3]
//                const double v[3]
//                double y
//                double p
//                double r
//                double X[3]
//                double V[3]
// Return Type  : void
//
void Cam_to_Global(double x[3], double v[3], double y, double p,
                   double r, double X[3], double V[3])
{
  double st[3];
  double ct[3];
  int k;
  double R[9];
  double b_ct;
  int i0;
  st[0] = y;
  st[1] = p;
  st[2] = r;
  for (k = 0; k < 3; k++) {
    b_ct = cos(st[k]);
    st[k] = sin(st[k]);
    ct[k] = b_ct;
  }

  R[0] = ct[1] * ct[0];
  R[3] = st[2] * st[1] * ct[0] - ct[2] * st[0];
  R[6] = ct[2] * st[1] * ct[0] + st[2] * st[0];
  R[1] = ct[1] * st[0];
  R[4] = st[2] * st[1] * st[0] + ct[2] * ct[0];
  R[7] = ct[2] * st[1] * st[0] - st[2] * ct[0];
  R[2] = -st[1];
  R[5] = st[2] * ct[1];
  R[8] = ct[2] * ct[1];
  for (k = 0; k < 3; k++) {
    X[k] = 0.0;
    V[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      X[k] += R[k + 3 * i0] * x[i0];
      V[k] += R[k + 3 * i0] * v[i0];
    }
  }
}

//
// File trailer for Cam_to_Global.cpp
//
// [EOF]
//
