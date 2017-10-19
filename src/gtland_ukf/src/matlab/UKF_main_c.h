//
// File: UKF_main_c.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 18-Oct-2017 21:20:19
//
#ifndef UKF_MAIN_C_H
#define UKF_MAIN_C_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "UKF_main_c_types.h"

// Function Declarations
extern void UKF_main_c(double x[6], double P[36], const double z[3], double f,
  double cx, double cy, double R_actual, const double QE[9], const double RE[36],
  double dt);

#endif

//
// File trailer for UKF_main_c.h
//
// [EOF]
//
