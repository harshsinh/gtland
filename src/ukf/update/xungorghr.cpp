//
// File: xungorghr.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:35:16
//

// Include Files
#include "rt_nonfinite.h"
#include "UKF_update.h"
#include "xungorghr.h"
#include "xgerc.h"

// Function Definitions

//
// Arguments    : double A[36]
//                const double tau[5]
// Return Type  : void
//
void xungorghr(double A[36], const double tau[5])
{
  int coltop;
  double work[6];
  int i;
  int b_i;
  int itau;
  int iaii;
  int i3;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int ia;
  int exitg1;
  int ix;
  double c;
  int i4;
  for (coltop = 4; coltop >= 0; coltop += -1) {
    i = (coltop + 1) * 6 - 1;
    for (b_i = 1; b_i <= coltop + 1; b_i++) {
      A[i + b_i] = 0.0;
    }

    for (b_i = coltop + 3; b_i < 7; b_i++) {
      A[i + b_i] = A[(i + b_i) - 6];
    }
  }

  for (b_i = 0; b_i < 6; b_i++) {
    A[b_i] = 0.0;
    work[b_i] = 0.0;
  }

  A[0] = 1.0;
  itau = 4;
  for (b_i = 4; b_i >= 0; b_i += -1) {
    iaii = (b_i + b_i * 6) + 8;
    if (b_i + 1 < 5) {
      A[iaii - 1] = 1.0;
      if (tau[itau] != 0.0) {
        lastv = 5 - b_i;
        i = (iaii - b_i) + 3;
        while ((lastv > 0) && (A[i] == 0.0)) {
          lastv--;
          i--;
        }

        lastc = 4 - b_i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          coltop = (iaii + (lastc - 1) * 6) + 5;
          ia = coltop;
          do {
            exitg1 = 0;
            if (ia + 1 <= coltop + lastv) {
              if (A[ia] != 0.0) {
                exitg1 = 1;
              } else {
                ia++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc == 0) {
        } else {
          for (i = 1; i <= lastc; i++) {
            work[i - 1] = 0.0;
          }

          i = 0;
          i3 = (iaii + 6 * (lastc - 1)) + 6;
          for (coltop = iaii + 6; coltop <= i3; coltop += 6) {
            ix = iaii;
            c = 0.0;
            i4 = (coltop + lastv) - 1;
            for (ia = coltop; ia <= i4; ia++) {
              c += A[ia - 1] * A[ix - 1];
              ix++;
            }

            work[i] += c;
            i++;
          }
        }

        xgerc(lastv, lastc, -tau[itau], iaii, work, A, iaii + 6);
      }
    }

    if (b_i + 1 < 5) {
      i3 = (iaii - b_i) + 4;
      for (coltop = iaii; coltop + 1 <= i3; coltop++) {
        A[coltop] *= -tau[itau];
      }
    }

    A[iaii - 1] = 1.0 - tau[itau];
    for (coltop = 1; coltop <= b_i; coltop++) {
      A[(iaii - coltop) - 1] = 0.0;
    }

    itau--;
  }
}

//
// File trailer for xungorghr.cpp
//
// [EOF]
//
