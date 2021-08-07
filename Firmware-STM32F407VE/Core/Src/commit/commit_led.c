/*
 * commit_led.c
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */


#include "main.h"
#include "commit.h"
#include "is32.h"
#include "i2c.h"
#include "ctrl.h"
#include <math.h>

/*
 * min/max utils
 */

uint8_t _max(uint16_t a, uint16_t b) {
  if (a > b) return a;
  else return b;
}

uint8_t _min(uint16_t a, uint16_t b) {
  if (a < b) return a;
  else return b;
}

/*
 * 12bit to 8bit conversion
 */

uint8_t _12_to_8(uint16_t a) {
  return a >> 4;
}

/*
 * Misc
 */

void _rgb_copy(uint16_t *dst, uint16_t *src) {
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
}


/*
 * HSV to RGB conversion
 */

rgb_t _hsv_to_rgb(hsv hsv)
{
  double hh, p, q, t, ff;
  long i;
  rgb_t rgb;

  if(hsv.s <= 0.0) {       // < is bogus, just shuts up warnings
    rgb.r = hsv.v;
    rgb.g = hsv.v;
    rgb.b = hsv.v;
    return rgb;
  }
  hh = hsv.h;
  if(hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = hsv.v * (1.0 - hsv.s);
  q = hsv.v * (1.0 - (hsv.s * ff));
  t = hsv.v * (1.0 - (hsv.s * (1.0 - ff)));

  switch(i) {
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

hsv _rgb_to_hsv(rgb_t in)
{
  hsv out;
  double min, max, delta;

  min = in.r < in.g ? in.r : in.g;
  min = min  < in.b ? min  : in.b;

  max = in.r > in.g ? in.r : in.g;
  max = max  > in.b ? max  : in.b;

  out.v = max;                                // v
  delta = max - min;
  if (delta < 0.00001)
  {
    out.s = 0;
    out.h = 0; // undefined, maybe nan?
    return out;
  }
  if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max);                  // s
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, h is undefined
    out.s = 0.0;
    out.h = NAN;                            // its now undefined
    return out;
  }
  if( in.r >= max )                           // > is bogus, just keeps compilor happy
    out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
  else
    if( in.g >= max )
      out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
      out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

  out.h *= 60.0;                              // degrees

  if( out.h < 0.0 )
    out.h += 360.0;

  return out;
}


/*
 * Interpolation
 */

hsv _interpolate_hsv(hsv in1, hsv in2) {
  hsv out;
  out.h = _interpolate_h(in1, in2);
  out.s = _interpolate_s(in1, in2);
  out.v = _interpolate_v(in1, in2);
  return out;
}

double _interpolate_h(hsv in1, hsv in2) {

  // Hue interpolation
  hsv a;
  hsv b;

  // Determine which hue is great
  // Make sure a <= b
  if (in1.h > in2.h) {
    a = in2;
    b = in1;
  } else {
    a = in1;
    b = in2;
  }

  // Calculate hue delta
  double h_delta = b.h - a.h;

  // Check for delta > 180 degrees
  if (h_delta > 180.0) {
    // Swap a & b
    // Then bump b.h by 360 degree so that it wraps
    // The goal here is to arrive at b.h > a.h but with
    // a delta <= 180 tp ensure proper color interpolation/mix
    hsv tmp = a;
    a = b;
    b = tmp;
    b.h += 360.0;
    h_delta = b.h - a.h;
  }

  double t;
  if (a.v <= 0.0) {
    t = 1.0;
  } else if (b.v <= 0.0) {
    t = 0.0;
  } else if (a.v > b.v) {
    // a is dominant color
    t = 0.5 * (b.v / a.v);
  } else {
    // b is dominant color
    t = 1.0-(0.5 * (a.v / b.v));
  }

  double h = (h_delta * t) + a.h;
  while (h > 360.0) {
    h -= 360.0;
  }
  return h;
}

double _interpolate_v(hsv in1, hsv in2) {
  return fmax(in1.v, in2.v);
//
//  hsv a;
//  hsv b;
//
//  if (in1.v > in2.v) {
//    a = in2;
//    b = in1;
//  } else {
//    a = in1;
//    b = in2;
//  }
//
//  double v_delta = b.v - a.v;
//
//  double t = 0.5;
//  double v = (v_delta * t) + a.v;
//  return v;
}

double _interpolate_s(hsv in1, hsv in2) {

  hsv a;
  hsv b;

  if (in1.s > in2.s) {
    a = in2;
    b = in1;
  } else {
    a = in1;
    b = in2;
  }

  double s_delta = b.s - a.s;

  double t = 0.5;
  double s = (s_delta * t) + a.s;
  return s;
}

rgb_14 _interpolate_rgb(rgb_14 in1, rgb_14 in2) {
  double t = 0.5;
  rgb_14 out = {
      .r = in1.r > in2.r ? in2.r+((in1.r - in2.r)*t) : in1.r+((in2.r - in1.r)*t),
      .g = in1.g > in2.g ? in2.g+((in1.g - in2.g)*t) : in1.g+((in2.g - in1.g)*t),
      .b = in1.b > in2.b ? in2.b+((in1.b - in2.b)*t) : in1.b+((in2.b - in1.b)*t),
  };
  return out;
}

/*
 * Oklab Color Space
 */

lab_t _rgb_to_oklab(rgb_t rgb)
{
  float l = 0.4122214708f * rgb.r + 0.5363325363f * rgb.g + 0.0514459929f * rgb.b;
  float m = 0.2119034982f * rgb.r + 0.6806995451f * rgb.g + 0.1073969566f * rgb.b;
  float s = 0.0883024619f * rgb.r + 0.2817188376f * rgb.g + 0.6299787005f * rgb.b;

  float l_ = cbrtf(l);
  float m_ = cbrtf(m);
  float s_ = cbrtf(s);

  lab_t out = {
    .L = 0.2104542553f*l_ + 0.7936177850f*m_ - 0.0040720468f*s_,
    .a = 1.9779984951f*l_ - 2.4285922050f*m_ + 0.4505937099f*s_,
    .b = 0.0259040371f*l_ + 0.7827717662f*m_ - 0.8086757660f*s_,
  };

  return out;
}

rgb_t _oklab_to_rgb(lab_t lab)
{
  float l_ = lab.L + 0.3963377774f * lab.a + 0.2158037573f * lab.b;
  float m_ = lab.L - 0.1055613458f * lab.a - 0.0638541728f * lab.b;
  float s_ = lab.L - 0.0894841775f * lab.a - 1.2914855480f * lab.b;

  float l = l_*l_*l_;
  float m = m_*m_*m_;
  float s = s_*s_*s_;

  rgb_t out = {
      .r = +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
      .g = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
      .b = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
  };
  return out;
}

lab_t _interpolate_lab(lab_t c0, lab_t c1) {
  double a = c0.v / 4095.0;
  double b = c1.v / 4095.0;
  double t;

  if (a <= 0.0) t = 1.0;
  else if (b <= 0.0) t = 0.0;
  else if (a > b) t = 0.5 * (b / a);
  else t = 1.0-(0.5 * (a / b));

  lab_t out = {
      L: (1 - t) * c0.L + t * c1.L,
      a: (1 - t) * c0.a + t * c1.a,
      b: (1 - t) * c0.b + t * c1.b,
      v: (1 - t) * c0.v + t * c1.v,
  };

  return out;
}

hsv _oklab_to_hsv(lab_t in) {
  return _rgb_to_hsv(_oklab_to_rgb(in));
}

lab_t _hsv_to_oklab(hsv in) {
  return _rgb_to_oklab(_hsv_to_rgb(in));
}

/*
 * Pulse Width Modulation (Color)
 */


void _set_pwm_seq(uint16_t *rgb, uint8_t *pwm_seq, uint8_t len) {
  for (uint8_t i=0; i < len; i++) {
    pwm_seq[i] = _12_to_8(rgb[i%3]);
  }
}

void _set_pwm_seq_hsv(hsv in, uint8_t *pwm_seq, uint8_t len) {
  rgb_t rgb = _hsv_to_rgb(in);
  rgb_14 rgb14 = {
      .r = rgb.r * 4095.0,
      .b = rgb.b * 4095.0,
      .g = rgb.g * 4095.0,
  };
  for (uint8_t i=0; i < len; i++) {
    switch (i%3) {
    case 0:
      pwm_seq[i] = _12_to_8(rgb14.r);
      break;
    case 1:
      pwm_seq[i] = _12_to_8(rgb14.g);
      break;
    case 2:
      pwm_seq[i] = _12_to_8(rgb14.b);
      break;
    }
  }
}

void _set_pwm_seq_lab(lab_t in, uint8_t *pwm_seq, uint8_t len) {
  rgb_t rgb = _oklab_to_rgb(in);
  if (rgb.r < 0.0) rgb.r = 0.0;
  if (rgb.g < 0.0) rgb.g = 0.0;
  if (rgb.b < 0.0) rgb.b = 0.0;
  if (rgb.r > 1.0) rgb.r = 1.0;
  if (rgb.g > 1.0) rgb.g = 1.0;
  if (rgb.b > 1.0) rgb.b = 1.0;
  rgb_14 rgb14 = {
      .r = rgb.r * 4095.0,
      .b = rgb.b * 4095.0,
      .g = rgb.g * 4095.0,
  };
  for (uint8_t i=0; i < len; i++) {
    switch (i%3) {
    case 0:
      pwm_seq[i] = _12_to_8(rgb14.r);
      break;
    case 1:
      pwm_seq[i] = _12_to_8(rgb14.g);
      break;
    case 2:
      pwm_seq[i] = _12_to_8(rgb14.b);
      break;
    }
  }
}


/*
 * Scaling (current / brightness)
 */

#define DEFAULT_SCALE 0x15
//#define DEFAULT_SCALE_R 0x27
//#define DEFAULT_SCALE_G 0x17
//#define DEFAULT_SCALE_B 0x26
#define DEFAULT_SCALE_R 0x37
#define DEFAULT_SCALE_G 0x27
#define DEFAULT_SCALE_B 0x36

//#define DEFAULT_SCALE_R 0x37
//#define DEFAULT_SCALE_G 0x37
//#define DEFAULT_SCALE_B 0x37

#define PATTERN_STEPS 10
double pattern[PATTERN_STEPS] = { 0.5, 0.5, 0.5, 0.5, 0.6, 0.7, 1.0, 0.5, 0.5, 0.5 };
uint8_t pattern_step = 0;
void increment_pattern_step() {
  pattern_step--;
}

void _set_scale_seq_animated(uint8_t *pwm_seq, uint8_t *scale_seq, uint8_t len, uint8_t offset, bool invert) {
  uint8_t pattern_offset = pattern_step + offset;
  for (uint8_t i=0; i < len; i++) {
    uint8_t bank_offset = (pattern_offset+(i/3)) % PATTERN_STEPS;
    double pattern_modulation = pattern[bank_offset];
    switch(i%3) {
    case 0:
      scale_seq[invert ? (len-1)-i : i] = ((double)DEFAULT_SCALE_R * pattern_modulation);
      break;
    case 1:
      scale_seq[invert ? (len-1)-i : i] = ((double)DEFAULT_SCALE_G * pattern_modulation);
      break;
    case 2:
      scale_seq[invert ? (len-1)-i : i] = ((double)DEFAULT_SCALE_B * pattern_modulation);
      break;
    }
  }
}

void _set_scale_seq(uint8_t *pwm_seq, uint8_t *scale_seq, uint8_t len) {
  _set_scale_seq_animated(pwm_seq, scale_seq, len, 0, false);
}
