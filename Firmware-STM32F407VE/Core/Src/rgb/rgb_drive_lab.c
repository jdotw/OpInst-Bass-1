/*
 * rgb_drive_lab.c
 *
 *  Created on: 7 May 2021
 *      Author: jwilson
 */

#include "ctrl.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"
#include "rgb.h"
#include <math.h>

lab_t _drive_lab(ctrl_t *ctrl, lab_t in, ctrl_enum_t drive_enum) {
  hsv_t hsv = oklab_to_hsv(in);
  double drive = ctrl_double(ctrl, drive_enum);
  hsv.h -= 120.0 * drive;
  if (hsv.h >= 360.0)
    hsv.h -= 360.0;
  if (hsv.h < 0.0)
    hsv.h += 360.0;
  hsv.s += (MAX_HSV_V - hsv.s) * drive;
  hsv.v += ((1.0 - hsv.v) * 0.75) * drive;
  lab_t lab = hsv_to_oklab(hsv);
  lab.v = in.v;
  return lab;
}

lab_t rgb_osc1_drive_lab(ctrl_t *ctrl) {
  return _drive_lab(ctrl, rgb_osc1_filt_reso_lab(ctrl), CTRL_OSC1_DRIVE_AMT);
}

lab_t rgb_osc2_drive_lab(ctrl_t *ctrl) {
  return _drive_lab(ctrl, rgb_osc2_filt_reso_lab(ctrl), CTRL_OSC2_DRIVE_AMT);
}
