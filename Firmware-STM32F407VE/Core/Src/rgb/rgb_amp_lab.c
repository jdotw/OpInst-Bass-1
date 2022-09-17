/*
 * rgb_amp_lab.c
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

lab_t rgb_osc_amp_out_lab(ctrl_t *ctrl) {
  lab_t osc1 = rgb_osc1_drive_lab(ctrl);
  lab_t osc2 = rgb_osc2_drive_lab(ctrl);
  return interpolate_lab(osc1, osc2);
}

lab_t rgb_sub_amp_out_lab(ctrl_t *ctrl) { return rgb_sub_filt_reso_lab(ctrl); }
