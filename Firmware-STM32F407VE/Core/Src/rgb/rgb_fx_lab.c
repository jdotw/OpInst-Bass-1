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
#include "param.h"
#include "rgb.h"
#include <math.h>

lab_t rgb_fx_dry_lab() {
  lab_t osc = rgb_osc_amp_out_lab();
  lab_t sub = rgb_sub_amp_out_lab();
  lab_t out = interpolate_lab(osc, sub);
  out.L *= ctrl_double_value_inverse(param_value(CTRL_FX_WETDRY));
  out.v = 4095 - param_value(CTRL_FX_WETDRY);
  return out;
}

lab_t rgb_fx_wet_lab() {
  lab_t out = interpolate_lab(rgb_osc_amp_out_lab(), rgb_sub_amp_out_lab());
  out.L *= ctrl_double_value(param_value(CTRL_FX_WETDRY));
  out.v = param_value(CTRL_FX_WETDRY);
  return out;
}

lab_t rgb_fx_feedback_lab() {
  lab_t out = rgb_fx_wet_lab();
  out.L *= ctrl_double_value(param_value(CTRL_FX_FEEDBACK));
  out.v = param_value(CTRL_FX_FEEDBACK);
  return out;
}
