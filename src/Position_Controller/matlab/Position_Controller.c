/*
 * File: Position_Controller.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 07-Jun-2018 16:29:26
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "Position_Controller.h"
#include "Position_Controller_data.h"

/* Function Definitions */

/*
 * Arguments    : const double e1_outer[3]
 *                const double x2[3]
 *                double psi_des
 *                double dt
 *                double kp_outer
 *                double kv_outer
 *                double G_outer
 *                double m
 *                boolean_T Adaptive_on
 *                double *Thrust
 *                double B_ref[4]
 * Return Type  : void
 */
void Position_Controller(const double e1_outer[3], const double x2[3], double
  psi_des, double dt, double kp_outer, double kv_outer, double G_outer, double m,
  boolean_T Adaptive_on, double *Thrust, double B_ref[4])
{
  double B_outer;
  double y;
  double e2_outer[3];
  double Wf_dot_outer[3];
  double e2f_dot_outer[3];
  int k;
  double b_e2_outer;
  double b_Wf_dot_outer;
  static const double W_outer[3] = { 0.0, 0.0, 9.81 };

  double b_e2f_dot_outer;
  double scale;
  double absxk;
  double t;
  static const signed char iv0[3] = { 0, 0, 1 };

  double dv1[3];
  double dv2[4];
  double q[4];
  double qout[4];
  B_outer = kp_outer + kv_outer;

  /* calculate theta and theta_integrated */
  /* calculate the desired acceleration and velocity */
  /* calculate the errors */
  /* error in velocity */
  /* calculate the derivatives of filter variables */
  y = (double)Adaptive_on * G_outer;

  /*  Calculate the control (u_outer) */
  for (k = 0; k < 3; k++) {
    theta_est_outer_int[k] += (thetaf_est_dot_outer[k] + B_outer *
      thetaf_est_outer[k]) * dt;
    b_e2_outer = x2[k] - (-theta_est_outer_int[k]);
    b_Wf_dot_outer = -B_outer * Wf_outer[k] + W_outer[k];
    b_e2f_dot_outer = -B_outer * e2f_outer[k] + b_e2_outer;
    thetaf_est_dot_outer[k] = y * (e2f_outer[k] + e1_outer[k]);
    b_e2_outer = m * (((b_Wf_dot_outer - kp_outer * b_e2_outer) - kv_outer *
                       b_e2f_dot_outer) - thetaf_est_dot_outer[k]) + B_outer *
      (m * (((Wf_outer[k] - kp_outer * e1_outer[k]) - kv_outer * e2f_outer[k]) -
            thetaf_est_outer[k]));
    e2_outer[k] = b_e2_outer;
    Wf_dot_outer[k] = b_Wf_dot_outer;
    e2f_dot_outer[k] = b_e2f_dot_outer;
  }

  *Thrust = sqrt((e2_outer[0] * e2_outer[0] + e2_outer[1] * e2_outer[1]) +
                 e2_outer[2] * e2_outer[2]);

  /* this is the desired thrust in body frame */
  /* Calculate the desired/reference attitude quaternion (B_ref) to be sent to the Attitude controller (inner loop) */
  y = 0.0;
  scale = 2.2250738585072014E-308;
  for (k = 0; k < 3; k++) {
    absxk = fabs(e2_outer[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0 + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  y = scale * sqrt(y);
  absxk = 0.0;
  for (k = 0; k < 3; k++) {
    b_e2_outer = e2_outer[k] / y;
    absxk += (double)iv0[k] * b_e2_outer;
    e2_outer[k] = b_e2_outer;
  }

  y = 1.0 / sqrt(2.0 * (1.0 + absxk));
  absxk = 0.0;
  dv1[0] = 0.0 * e2_outer[2] - e2_outer[1];
  dv1[1] = e2_outer[0] - 0.0 * e2_outer[2];
  dv1[2] = 0.0 * e2_outer[1] - 0.0 * e2_outer[0];
  for (k = 0; k < 3; k++) {
    absxk += (double)iv0[k] * e2_outer[k];
    dv2[k + 1] = -dv1[k];
  }

  dv2[0] = 1.0 + absxk;
  for (k = 0; k < 4; k++) {
    q[k] = y * dv2[k];
  }

  absxk = cos(psi_des / 2.0);
  scale = sin(psi_des / 2.0);
  qout[0] = ((q[0] * absxk - q[1] * 0.0) - q[2] * 0.0) - q[3] * scale;
  qout[1] = (q[0] * 0.0 + absxk * q[1]) + (q[2] * scale - q[3] * 0.0);
  qout[2] = (q[0] * 0.0 + absxk * q[2]) + (q[3] * 0.0 - q[1] * scale);
  qout[3] = (q[0] * scale + absxk * q[3]) + (q[1] * 0.0 - q[2] * 0.0);
  y = 0.0;
  scale = 2.2250738585072014E-308;
  for (k = 0; k < 4; k++) {
    absxk = fabs(qout[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0 + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  y = scale * sqrt(y);
  for (k = 0; k < 4; k++) {
    B_ref[k] = qout[k] / y;
  }

  /* Filter variables update */
  for (k = 0; k < 3; k++) {
    Wf_outer[k] += Wf_dot_outer[k] * dt;
    e2f_outer[k] += e2f_dot_outer[k] * dt;
    thetaf_est_outer[k] += thetaf_est_dot_outer[k] * dt;
  }
}

/*
 * File trailer for Position_Controller.c
 *
 * [EOF]
 */
