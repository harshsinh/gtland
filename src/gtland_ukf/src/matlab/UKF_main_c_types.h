//
// File: UKF_main_c_types.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 18-Oct-2017 21:20:19
//
#ifndef UKF_MAIN_C_TYPES_H
#define UKF_MAIN_C_TYPES_H

// Include Files
#include "rtwtypes.h"

// Type Definitions
#ifndef struct_emxArray__common
#define struct_emxArray__common

struct emxArray__common
{
  void *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray__common

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real_T
#endif

//
// File trailer for UKF_main_c_types.h
//
// [EOF]
//
