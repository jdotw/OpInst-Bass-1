/*
 * rgb_led.c
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */

#include "ctrl.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"
#include "rgb.h"
#include <math.h>

lab_t interpolate_lab(lab_t c0, lab_t c1) {
  double a = c0.v / 4095.0;
  double b = c1.v / 4095.0;
  double t;

  if (a <= 0.0)
    t = 1.0;
  else if (b <= 0.0)
    t = 0.0;
  else if (a > b)
    t = 0.5 * (b / a);
  else
    t = 1.0 - (0.5 * (a / b));

  lab_t out = {
    L : (1 - t) * c0.L + t * c1.L,
    a : (1 - t) * c0.a + t * c1.a,
    b : (1 - t) * c0.b + t * c1.b,
    v : (1 - t) * c0.v + t * c1.v,
  };

  return out;
}
