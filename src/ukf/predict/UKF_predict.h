//
// File: UKF_predict.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:32:57
//
#ifndef UKF_PREDICT_H
#define UKF_PREDICT_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "UKF_predict_types.h"

// Function Declarations
extern void UKF_predict(const double x[6], double P[36], double dt, const double
  RE[36]);

#endif

//
// File trailer for UKF_predict.h
//
// [EOF]
//
