/*
 * rgb_osc1_lab.c
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

lab_t rgb_osc1_saw_lab(ctrl_t *ctrl) {
  return rgb_primitive_lab(ctrl, 1.0, 0.0, 0.0, CTRL_OSC1_SAW_LVL);
}

lab_t rgb_osc1_squ_lab(ctrl_t *ctrl) {
  return rgb_primitive_lab(ctrl, 0.0, 1.0, 0.0, CTRL_OSC1_SQU_LVL);
}

lab_t rgb_osc1_mix_lab(ctrl_t *ctrl) {
  lab_t saw = rgb_osc1_saw_lab(ctrl);
  lab_t squ = rgb_osc1_squ_lab(ctrl);
  lab_t mix = interpolate_lab(saw, squ);
  return mix;
}

lab_t rgb_osc1_to_2_mix_lab(ctrl_t *ctrl) {
  lab_t out = rgb_osc1_mix_lab(ctrl);
  out.L *= ctrl_double(ctrl, CTRL_OSC1_TO_OSC2_MIX);
  out.v = ctrl->value[CTRL_OSC1_TO_OSC2_MIX];
  return out;
}
