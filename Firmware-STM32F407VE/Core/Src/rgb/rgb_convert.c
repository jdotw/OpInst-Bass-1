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

/*
 * HSV to RGB conversion
 */

rgb_t _hsv_to_rgb(hsv_t hsv) {
  double hh, p, q, t, ff;
  long i;
  rgb_t rgb;

  if (hsv.s <= 0.0) { // < is bogus, just shuts up warnings
    rgb.r = hsv.v;
    rgb.g = hsv.v;
    rgb.b = hsv.v;
    return rgb;
  }
  hh = hsv.h;
  if (hh >= 360.0)
    hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = hsv.v * (1.0 - hsv.s);
  q = hsv.v * (1.0 - (hsv.s * ff));
  t = hsv.v * (1.0 - (hsv.s * (1.0 - ff)));

  switch (i) {
  case 0:
    rgb.r = hsv.v;
    rgb.g = t;
    rgb.b = p;
    break;
  case 1:
    rgb.r = q;
    rgb.g = hsv.v;
    rgb.b = p;
    break;
  case 2:
    rgb.r = p;
    rgb.g = hsv.v;
    rgb.b = t;
    break;

  case 3:
    rgb.r = p;
    rgb.g = q;
    rgb.b = hsv.v;
    break;
  case 4:
    rgb.r = t;
    rgb.g = p;
    rgb.b = hsv.v;
    break;
  case 5:
  default:
    rgb.r = hsv.v;
    rgb.g = p;
    rgb.b = q;
    break;
  }

  return rgb;
}

hsv_t _rgb_to_hsv(rgb_t in) {
  hsv_t out;
  double min, max, delta;

  min = in.r < in.g ? in.r : in.g;
  min = min < in.b ? min : in.b;

  max = in.r > in.g ? in.r : in.g;
  max = max > in.b ? max : in.b;

  out.v = max; // v
  delta = max - min;
  if (delta < 0.00001) {
    out.s = 0;
    out.h = 0; // undefined, maybe nan?
    return out;
  }
  if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max); // s
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, h is undefined
    out.s = 0.0;
    out.h = NAN; // its now undefined
    return out;
  }
  if (in.r >= max)                 // > is bogus, just keeps compilor happy
    out.h = (in.g - in.b) / delta; // between yellow & magenta
  else if (in.g >= max)
    out.h = 2.0 + (in.b - in.r) / delta; // between cyan & yellow
  else
    out.h = 4.0 + (in.r - in.g) / delta; // between magenta & cyan

  out.h *= 60.0; // degrees

  if (out.h < 0.0)
    out.h += 360.0;

  return out;
}

/*
 * Oklab Color Space
 */

lab_t rgb_to_oklab(rgb_t rgb) {
  float l =
      0.4122214708f * rgb.r + 0.5363325363f * rgb.g + 0.0514459929f * rgb.b;
  float m =
      0.2119034982f * rgb.r + 0.6806995451f * rgb.g + 0.1073969566f * rgb.b;
  float s =
      0.0883024619f * rgb.r + 0.2817188376f * rgb.g + 0.6299787005f * rgb.b;

  float l_ = cbrtf(l);
  float m_ = cbrtf(m);
  float s_ = cbrtf(s);

  lab_t out = {
      .L = 0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
      .a = 1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
      .b = 0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_,
  };

  return out;
}

rgb_t oklab_to_rgb(lab_t lab) {
  float l_ = lab.L + 0.3963377774f * lab.a + 0.2158037573f * lab.b;
  float m_ = lab.L - 0.1055613458f * lab.a - 0.0638541728f * lab.b;
  float s_ = lab.L - 0.0894841775f * lab.a - 1.2914855480f * lab.b;

  float l = l_ * l_ * l_;
  float m = m_ * m_ * m_;
  float s = s_ * s_ * s_;

  rgb_t out = {
      .r = +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
      .g = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
      .b = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
  };
  return out;
}

hsv_t oklab_to_hsv(lab_t in) { return _rgb_to_hsv(oklab_to_rgb(in)); }

lab_t hsv_to_oklab(hsv_t in) { return rgb_to_oklab(_hsv_to_rgb(in)); }
