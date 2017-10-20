//
// File: xdhseqr.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 17-Oct-2017 11:35:16
//

// Include Files
#include "rt_nonfinite.h"
#include "../update/UKF_update.h"
#include "xdhseqr.h"
#include "xrot.h"
#include "xdlanv2.h"
#include "xzlarfg.h"

// Function Definitions

//
// Arguments    : double h[36]
//                double z[36]
// Return Type  : int
//
int eml_dlahqr(double h[36], double z[36])
{
  int info;
  double v[3];
  int j;
  int i;
  boolean_T exitg1;
  int L;
  boolean_T goto150;
  int its;
  boolean_T exitg2;
  int k;
  boolean_T exitg4;
  double tst;
  double htmp1;
  double aa;
  double ab;
  double ba;
  double rt2r;
  double rt1r;
  double s;
  double sn;
  boolean_T guard1 = false;
  int nr;
  int hoffset;
  int m;
  boolean_T exitg3;
  double d1;
  int b_k;
  info = 0;
  for (j = 0; j < 3; j++) {
    h[(j + 6 * j) + 2] = 0.0;
    h[(j + 6 * j) + 3] = 0.0;
  }

  h[23] = 0.0;
  i = 5;
  exitg1 = false;
  while ((!exitg1) && (i + 1 >= 1)) {
    L = 1;
    goto150 = false;
    its = 0;
    exitg2 = false;
    while ((!exitg2) && (its < 31)) {
      k = i;
      exitg4 = false;
      while ((!exitg4) && ((k + 1 > L) && (!(fabs(h[k + 6 * (k - 1)]) <=
                6.0125050800269183E-292)))) {
        tst = fabs(h[(k + 6 * (k - 1)) - 1]) + fabs(h[k + 6 * k]);
        if (tst == 0.0) {
          if (k - 1 >= 1) {
            tst = fabs(h[(k + 6 * (k - 2)) - 1]);
          }

          if (k + 2 <= 6) {
            tst += fabs(h[(k + 6 * k) + 1]);
          }
        }

        guard1 = false;
        if (fabs(h[k + 6 * (k - 1)]) <= 2.2204460492503131E-16 * tst) {
          htmp1 = fabs(h[k + 6 * (k - 1)]);
          tst = fabs(h[(k + 6 * k) - 1]);
          if (htmp1 > tst) {
            ab = htmp1;
            ba = tst;
          } else {
            ab = tst;
            ba = htmp1;
          }

          htmp1 = fabs(h[k + 6 * k]);
          tst = fabs(h[(k + 6 * (k - 1)) - 1] - h[k + 6 * k]);
          if (htmp1 > tst) {
            aa = htmp1;
            htmp1 = tst;
          } else {
            aa = tst;
          }

          s = aa + ab;
          tst = 2.2204460492503131E-16 * (htmp1 * (aa / s));
          if ((6.0125050800269183E-292 >= tst) || rtIsNaN(tst)) {
            d1 = 6.0125050800269183E-292;
          } else {
            d1 = tst;
          }

          if (ba * (ab / s) <= d1) {
            exitg4 = true;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }

        if (guard1) {
          k--;
        }
      }

      L = k + 1;
      if (k + 1 > 1) {
        h[k + 6 * (k - 1)] = 0.0;
      }

      if (k + 1 >= i) {
        goto150 = true;
        exitg2 = true;
      } else {
        if (its == 10) {
          s = fabs(h[(k + 6 * k) + 1]) + fabs(h[(k + 6 * (k + 1)) + 2]);
          tst = 0.75 * s + h[k + 6 * k];
          aa = -0.4375 * s;
          htmp1 = s;
          ba = tst;
        } else if (its == 20) {
          s = fabs(h[i + 6 * (i - 1)]) + fabs(h[(i + 6 * (i - 2)) - 1]);
          tst = 0.75 * s + h[i + 6 * i];
          aa = -0.4375 * s;
          htmp1 = s;
          ba = tst;
        } else {
          tst = h[(i + 6 * (i - 1)) - 1];
          htmp1 = h[i + 6 * (i - 1)];
          aa = h[(i + 6 * i) - 1];
          ba = h[i + 6 * i];
        }

        s = ((fabs(tst) + fabs(aa)) + fabs(htmp1)) + fabs(ba);
        if (s == 0.0) {
          rt1r = 0.0;
          ab = 0.0;
          rt2r = 0.0;
          ba = 0.0;
        } else {
          tst /= s;
          htmp1 /= s;
          aa /= s;
          ba /= s;
          ab = (tst + ba) / 2.0;
          tst = (tst - ab) * (ba - ab) - aa * htmp1;
          htmp1 = sqrt(fabs(tst));
          if (tst >= 0.0) {
            rt1r = ab * s;
            rt2r = rt1r;
            ab = htmp1 * s;
            ba = -ab;
          } else {
            rt1r = ab + htmp1;
            rt2r = ab - htmp1;
            if (fabs(rt1r - ba) <= fabs(rt2r - ba)) {
              rt1r *= s;
              rt2r = rt1r;
            } else {
              rt2r *= s;
              rt1r = rt2r;
            }

            ab = 0.0;
            ba = 0.0;
          }
        }

        m = i - 1;
        exitg3 = false;
        while ((!exitg3) && (m >= k + 1)) {
          s = (fabs(h[(m + 6 * (m - 1)) - 1] - rt2r) + fabs(ba)) + fabs(h[m + 6 *
            (m - 1)]);
          tst = h[m + 6 * (m - 1)] / s;
          v[0] = (tst * h[(m + 6 * m) - 1] + (h[(m + 6 * (m - 1)) - 1] - rt1r) *
                  ((h[(m + 6 * (m - 1)) - 1] - rt2r) / s)) - ab * (ba / s);
          v[1] = tst * (((h[(m + 6 * (m - 1)) - 1] + h[m + 6 * m]) - rt1r) -
                        rt2r);
          v[2] = tst * h[(m + 6 * m) + 1];
          s = (fabs(v[0]) + fabs(v[1])) + fabs(v[2]);
          tst = v[0] / s;
          v[0] /= s;
          htmp1 = v[1] / s;
          v[1] /= s;
          aa = v[2] / s;
          v[2] /= s;
          if ((m == k + 1) || (fabs(h[(m + 6 * (m - 2)) - 1]) * (fabs(htmp1) +
                fabs(aa)) <= 2.2204460492503131E-16 * fabs(tst) * ((fabs(h[(m +
                   6 * (m - 2)) - 2]) + fabs(h[(m + 6 * (m - 1)) - 1])) + fabs
                (h[m + 6 * m])))) {
            exitg3 = true;
          } else {
            m--;
          }
        }

        for (b_k = m; b_k <= i; b_k++) {
          nr = (i - b_k) + 2;
          if (3 <= nr) {
            nr = 3;
          }

          if (b_k > m) {
            hoffset = b_k + 6 * (b_k - 2);
            for (j = -1; j + 2 <= nr; j++) {
              v[j + 1] = h[j + hoffset];
            }
          }

          tst = v[0];
          rt2r = xzlarfg(nr, &tst, v);
          v[0] = tst;
          if (b_k > m) {
            h[(b_k + 6 * (b_k - 2)) - 1] = tst;
            h[b_k + 6 * (b_k - 2)] = 0.0;
            if (b_k < i) {
              h[(b_k + 6 * (b_k - 2)) + 1] = 0.0;
            }
          } else {
            if (m > k + 1) {
              h[(b_k + 6 * (b_k - 2)) - 1] *= 1.0 - rt2r;
            }
          }

          tst = v[1];
          htmp1 = rt2r * v[1];
          if (nr == 3) {
            ab = v[2];
            ba = rt2r * v[2];
            for (j = b_k - 1; j + 1 < 7; j++) {
              aa = (h[(b_k + 6 * j) - 1] + tst * h[b_k + 6 * j]) + ab * h[(b_k +
                6 * j) + 1];
              h[(b_k + 6 * j) - 1] -= aa * rt2r;
              h[b_k + 6 * j] -= aa * htmp1;
              h[(b_k + 6 * j) + 1] -= aa * ba;
            }

            if (b_k + 3 <= i + 1) {
              nr = b_k;
            } else {
              nr = i - 2;
            }

            for (j = 0; j + 1 <= nr + 3; j++) {
              aa = (h[j + 6 * (b_k - 1)] + tst * h[j + 6 * b_k]) + ab * h[j + 6 *
                (b_k + 1)];
              h[j + 6 * (b_k - 1)] -= aa * rt2r;
              h[j + 6 * b_k] -= aa * htmp1;
              h[j + 6 * (b_k + 1)] -= aa * ba;
            }

            for (j = 0; j < 6; j++) {
              aa = (z[j + 6 * (b_k - 1)] + tst * z[j + 6 * b_k]) + ab * z[j + 6 *
                (b_k + 1)];
              z[j + 6 * (b_k - 1)] -= aa * rt2r;
              z[j + 6 * b_k] -= aa * htmp1;
              z[j + 6 * (b_k + 1)] -= aa * ba;
            }
          } else {
            if (nr == 2) {
              for (j = b_k - 1; j + 1 < 7; j++) {
                aa = h[(b_k + 6 * j) - 1] + tst * h[b_k + 6 * j];
                h[(b_k + 6 * j) - 1] -= aa * rt2r;
                h[b_k + 6 * j] -= aa * htmp1;
              }

              for (j = 0; j + 1 <= i + 1; j++) {
                aa = h[j + 6 * (b_k - 1)] + tst * h[j + 6 * b_k];
                h[j + 6 * (b_k - 1)] -= aa * rt2r;
                h[j + 6 * b_k] -= aa * htmp1;
              }

              for (j = 0; j < 6; j++) {
                aa = z[j + 6 * (b_k - 1)] + tst * z[j + 6 * b_k];
                z[j + 6 * (b_k - 1)] -= aa * rt2r;
                z[j + 6 * b_k] -= aa * htmp1;
              }
            }
          }
        }

        its++;
      }
    }

    if (!goto150) {
      info = i + 1;
      exitg1 = true;
    } else {
      if ((L == i + 1) || (!(L == i))) {
      } else {
        tst = h[(i + 6 * i) - 1];
        htmp1 = h[i + 6 * (i - 1)];
        aa = h[i + 6 * i];
        xdlanv2(&h[(i + 6 * (i - 1)) - 1], &tst, &htmp1, &aa, &ab, &ba, &rt2r,
                &rt1r, &s, &sn);
        h[(i + 6 * i) - 1] = tst;
        h[i + 6 * (i - 1)] = htmp1;
        h[i + 6 * i] = aa;
        if (6 > i + 1) {
          xrot(5 - i, h, i + (i + 1) * 6, (i + (i + 1) * 6) + 1, s, sn);
        }

        if (i - 1 < 1) {
        } else {
          nr = (i - 1) * 6;
          hoffset = i * 6;
          for (k = 1; k < i; k++) {
            tst = s * h[nr] + sn * h[hoffset];
            h[hoffset] = s * h[hoffset] - sn * h[nr];
            h[nr] = tst;
            hoffset++;
            nr++;
          }
        }

        nr = (i - 1) * 6;
        hoffset = i * 6;
        for (k = 0; k < 6; k++) {
          tst = s * z[nr] + sn * z[hoffset];
          z[hoffset] = s * z[hoffset] - sn * z[nr];
          z[nr] = tst;
          hoffset++;
          nr++;
        }
      }

      i = L - 2;
    }
  }

  return info;
}

//
// File trailer for xdhseqr.cpp
//
// [EOF]
//
