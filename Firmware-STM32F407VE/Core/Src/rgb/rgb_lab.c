/*
 * commit_led_osc.c
 *
 *  Created on: 7 May 2021
 *      Author: jwilson
 */

#include "commit.h"
#include "ctrl.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"
#include "rgb.h"
#include <math.h>

// Defaults

#define MAX_HSV_V 1.0

#define ALWAYS_UPDATE 1

// utils

lab_t rgb_primitive_lab(ctrl_t *ctrl, double r, double g, double b,
                        ctrl_enum_t i) {
  rgb_t rgb = {.r = r, .g = g, .b = b};
  lab_t lab = rgb_to_oklab(rgb);
  lab.L *= ctrl_double(ctrl, i);
  lab.v = ctrl->value[i];
  return lab;
}
