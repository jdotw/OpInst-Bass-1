/*
 * rgb_filt_lab.c
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

/*
 * Common
 */

lab_t _filt_freq_lab(ctrl_t *ctrl, lab_t in, ctrl_enum_t cutoff_enum) {
  hsv_t hsv = oklab_to_hsv(in);
  double cutoff = ctrl_double(ctrl, cutoff_enum);
  hsv.s *= cutoff;
  hsv.v *= (cutoff * cutoff);
  lab_t lab = hsv_to_oklab(hsv);
  lab.v = in.v;
  return lab;
}

lab_t _filt_reso_lab(ctrl_t *ctrl, lab_t in, ctrl_enum_t res_enum) {
  hsv_t hsv = oklab_to_hsv(in);
  double res = ctrl_double(ctrl, res_enum);
  hsv.h += 90.0 * res;
  if (hsv.h >= 360.0)
    hsv.h -= 360.0;
  hsv.s += (MAX_HSV_V - hsv.s) * res;
  hsv.v += ((1.0 - hsv.v) * 0.75) * res;
  lab_t lab = hsv_to_oklab(hsv);
  lab.v = in.v;
  return lab;
}

/*
 * Osc1
 */

lab_t rgb_osc1_filt_freq_lab(ctrl_t *ctrl) {
  return _filt_freq_lab(ctrl, rgb_osc1_mix_lab(ctrl), CTRL_OSC1_FILT_CUTOFF);
}

lab_t rgb_osc1_filt_reso_lab(ctrl_t *ctrl) {
  return _filt_reso_lab(ctrl, rgb_osc1_filt_freq_lab(ctrl), CTRL_OSC1_FILT_RES);
}

/*
 * Osc2
 */

lab_t rgb_osc2_filt_freq_lab(ctrl_t *ctrl) {
  return _filt_freq_lab(ctrl, rgb_osc2_prefilt_lab(ctrl),
                        CTRL_OSC2_FILT_CUTOFF);
}

lab_t rgb_osc2_filt_reso_lab(ctrl_t *ctrl) {
  return _filt_reso_lab(ctrl, rgb_osc2_filt_freq_lab(ctrl), CTRL_OSC2_FILT_RES);
}

/*
 * Sub
 */

lab_t rgb_sub_filt_freq_lab(ctrl_t *ctrl) {
  return _filt_freq_lab(ctrl, rgb_sub_mix_lab(ctrl), CTRL_SUB_FILT_CUTOFF);
}

lab_t rgb_sub_filt_reso_lab(ctrl_t *ctrl) {
  return _filt_reso_lab(ctrl, rgb_sub_filt_freq_lab(ctrl), CTRL_SUB_FILT_RES);
}
