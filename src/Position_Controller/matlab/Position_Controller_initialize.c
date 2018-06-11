/*
 * File: Position_Controller_initialize.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 07-Jun-2018 16:29:26
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "Position_Controller.h"
#include "Position_Controller_initialize.h"
#include "Position_Controller_data.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void Position_Controller_initialize(void)
{
  int i;
  static const double dv0[3] = { 0.0, 0.0, 0.19423800000000002 };

  for (i = 0; i < 3; i++) {
    Wf_outer[i] = dv0[i];
  }

  rt_InitInfAndNaN(8U);
  for (i = 0; i < 3; i++) {
    e2f_outer[i] = 0.0;
    theta_est_outer_int[i] = 0.0;
    thetaf_est_outer[i] = 0.0;
    thetaf_est_dot_outer[i] = 0.0;
  }
}

/*
 * File trailer for Position_Controller_initialize.c
 *
 * [EOF]
 */
