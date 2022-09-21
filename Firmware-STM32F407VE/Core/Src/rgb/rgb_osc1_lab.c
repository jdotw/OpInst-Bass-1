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
#include "param.h"
#include "rgb.h"
#include <math.h>

lab_t rgb_osc1_saw_lab() {
  return rgb_primitive_lab_value(param_value(CTRL_OSC1_SAW_LVL), 1.0, 0.0, 0.0);
}

lab_t rgb_osc1_squ_lab() {
  return rgb_primitive_lab_value(param_value(CTRL_OSC1_SQU_LVL), 0.0, 1.0, 0.0);
}

lab_t rgb_osc1_mix_lab() {
  lab_t saw = rgb_osc1_saw_lab();
  lab_t squ = rgb_osc1_squ_lab();
  lab_t mix = interpolate_lab(saw, squ);
  return mix;
}

lab_t rgb_osc1_to_2_mix_lab() {
  lab_t out = rgb_osc1_mix_lab();
  uint16_t mix = param_value(CTRL_OSC1_TO_OSC2_MIX);
  out.L *= ctrl_double_value(mix);
  out.v = mix;
  return out;
}
