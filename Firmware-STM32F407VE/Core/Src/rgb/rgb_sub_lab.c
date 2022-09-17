/*
 * rgb_sub_lab.c
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

lab_t rgb_sub_squ_lab(ctrl_t *ctrl) {
  return rgb_primitive_lab(ctrl, 0.0, 1.0, 1.0, CTRL_SUB_LVL);
}

lab_t rgb_sub_noise_lab(ctrl_t *ctrl) {
  return rgb_primitive_lab(ctrl, 1.0, 0.0, 1.0, CTRL_SUB_NOISE_LVL);
}

lab_t rgb_sub_mix_lab(ctrl_t *ctrl) {
  lab_t squ = rgb_sub_squ_lab(ctrl);
  lab_t noise = rgb_sub_noise_lab(ctrl);
  lab_t out = interpolate_lab(squ, noise);
  return out;
}

lab_t rgb_sub_to_osc2_mix_lab(ctrl_t *ctrl) {
  lab_t out = rgb_sub_mix_lab(ctrl);
  out.L *= ctrl_double(ctrl, CTRL_SUB_TO_OSC2_MIX);
  out.v = ctrl->value[CTRL_SUB_TO_OSC2_MIX];
  return out;
}
