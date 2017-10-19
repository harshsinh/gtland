//
// File: UKF_main_c_emxutil.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 18-Oct-2017 21:20:19
//
#ifndef UKF_MAIN_C_EMXUTIL_H
#define UKF_MAIN_C_EMXUTIL_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "UKF_main_c_types.h"

// Function Declarations
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for UKF_main_c_emxutil.h
//
// [EOF]
//
