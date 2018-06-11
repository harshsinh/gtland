/*
 * File: _coder_Position_Controller_api.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 07-Jun-2018 16:29:26
 */

#ifndef _CODER_POSITION_CONTROLLER_API_H
#define _CODER_POSITION_CONTROLLER_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_Position_Controller_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void Position_Controller(real_T e1_outer[3], real_T x2[3], real_T psi_des,
  real_T dt, real_T kp_outer, real_T kv_outer, real_T G_outer, real_T m,
  boolean_T Adaptive_on, real_T *Thrust, real_T B_ref[4]);
extern void Position_Controller_api(const mxArray *prhs[9], const mxArray *plhs
  [2]);
extern void Position_Controller_atexit(void);
extern void Position_Controller_initialize(void);
extern void Position_Controller_terminate(void);
extern void Position_Controller_xil_terminate(void);

#endif

/*
 * File trailer for _coder_Position_Controller_api.h
 *
 * [EOF]
 */
