/*
 * rgb_osc2_lab.c
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

lab_t rgb_osc2_saw_lab(ctrl_t *ctrl) {
  return rgb_primitive_lab(ctrl, 1.0, 0.0, 0.0, CTRL_OSC2_SAW_LVL);
}

lab_t rgb_osc2_squ_lab(ctrl_t *ctrl) {
  return rgb_primitive_lab(ctrl, 0.0, 1.0, 0.0, CTRL_OSC2_SQU_LVL);
}

lab_t rgb_osc2_noise_lab(ctrl_t *ctrl) {
  return rgb_primitive_lab(ctrl, 0.0, 0.0, 1.0, CTRL_OSC2_NOISE_LVL);
}

lab_t rgb_osc2_mix_lab(ctrl_t *ctrl) {
  lab_t saw_lab = rgb_osc2_saw_lab(ctrl);
  lab_t squ_lab = rgb_osc2_squ_lab(ctrl);
  lab_t noise_lab = rgb_osc2_noise_lab(ctrl);
  lab_t mix = interpolate_lab(interpolate_lab(saw_lab, squ_lab), noise_lab);
  return mix;
}

lab_t rgb_osc2_prefilt_lab(ctrl_t *ctrl) {
  lab_t osc1 = rgb_osc1_to_2_mix_lab(ctrl);
  lab_t osc2 = rgb_osc2_mix_lab(ctrl);
  lab_t sub = rgb_sub_to_osc2_mix_lab(ctrl);

  lab_t osc1_and_2 = interpolate_lab(osc2, osc1);
  lab_t osc_and_sub = interpolate_lab(osc1_and_2, sub);

  return osc_and_sub;
}
