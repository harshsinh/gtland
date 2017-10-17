//
// File: UKF_update.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:35:16
//
#ifndef UKF_UPDATE_H
#define UKF_UPDATE_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "UKF_update_types.h"

// Function Declarations
extern void UKF_update(double x[6], double P[36], const double z[3], const
  double QE[9], double R_actual, double f, double cx, double cy);

#endif

//
// File trailer for UKF_update.h
//
// [EOF]
//
