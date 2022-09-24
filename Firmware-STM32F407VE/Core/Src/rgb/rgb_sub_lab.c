/*
 * rgb_sub_lab.c
 *
 *  Created on: 7 May 2021
 *      Author: jwilson
 */

#include "i2c.h"
#include "is32.h"
#include "main.h"
#include "param.h"
#include "rgb.h"
#include <math.h>

lab_t rgb_sub_squ_lab() {
  return rgb_primitive_lab(param_value(CTRL_SUB_LVL), 0.0, 1.0, 1.0);
}

lab_t rgb_sub_noise_lab() {
  return rgb_primitive_lab(param_value(CTRL_SUB_NOISE_LVL), 1.0, 0.0, 1.0);
}

lab_t rgb_sub_mix_lab() {
  lab_t squ = rgb_sub_squ_lab();
  lab_t noise = rgb_sub_noise_lab();
  lab_t out = interpolate_lab(squ, noise);
  return out;
}

lab_t rgb_sub_to_osc2_mix_lab() {
  lab_t out = rgb_sub_mix_lab();
  out.L *= ctrl_double(param_value(CTRL_SUB_TO_OSC2_MIX));
  out.v = param_value(CTRL_SUB_TO_OSC2_MIX);
  return out;
}
