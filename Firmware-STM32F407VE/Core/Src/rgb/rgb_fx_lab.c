/*
 * rgb_fx_lab.c
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

lab_t rgb_fx_dry_lab(ctrl_t *ctrl) {
  lab_t osc = rgb_osc_amp_out_lab(ctrl);
  lab_t sub = rgb_sub_amp_out_lab(ctrl);
  lab_t out = interpolate_lab(osc, sub);
  out.L *= ctrl_double_inverse(ctrl, CTRL_FX_WETDRY);
  out.v = 4095 - ctrl->value[CTRL_FX_WETDRY];
  return out;
}

lab_t rgb_fx_wet_lab(ctrl_t *ctrl) {
  lab_t out =
      interpolate_lab(rgb_osc_amp_out_lab(ctrl), rgb_sub_amp_out_lab(ctrl));
  out.L *= ctrl_double(ctrl, CTRL_FX_WETDRY);
  out.v = ctrl->value[CTRL_FX_WETDRY];
  return out;
}

lab_t rgb_fx_feedback_lab(ctrl_t *ctrl) {
  lab_t out = rgb_fx_wet_lab(ctrl);
  out.L *= ctrl_double(ctrl, CTRL_FX_FEEDBACK);
  out.v = ctrl->value[CTRL_FX_FEEDBACK];
  return out;
}
