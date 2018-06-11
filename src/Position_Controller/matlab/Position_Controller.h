/*
 * File: Position_Controller.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 07-Jun-2018 16:29:26
 */

#ifndef POSITION_CONTROLLER_H
#define POSITION_CONTROLLER_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "Position_Controller_types.h"

/* Function Declarations */
extern void Position_Controller(const double e1_outer[3], const double x2[3],
  double psi_des, double dt, double kp_outer, double kv_outer, double G_outer,
  double m, boolean_T Adaptive_on, double *Thrust, double B_ref[4]);

#endif

/*
 * File trailer for Position_Controller.h
 *
 * [EOF]
 */
