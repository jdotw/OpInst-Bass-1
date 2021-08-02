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

typedef struct {
  double r;       // a fraction between 0 and 1
  double g;       // a fraction between 0 and 1
  double b;       // a fraction between 0 and 1
} rgb_double;

rgb_14 _hsv_to_rgb(hsv hsv)
{
  double      hh, p, q, t, ff;
  long        i;
  rgb_double  rgb_d;  // internal 0.0-1.0 RGB struct
  rgb_14      out = { .r = 0, .g = 0, .b = 0 };    // 0-4095 RGB data returned

  if(hsv.s <= 0.0) {       // < is bogus, just shuts up warnings
    out.r = hsv.v * 4095.0;
    out.g = hsv.v * 4095.0;
    out.b = hsv.v * 4095.0;
    return out;
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
    rgb_d.r = hsv.v;
    rgb_d.g = t;
    rgb_d.b = p;
    break;
  case 1:
    rgb_d.r = q;
    rgb_d.g = hsv.v;
    rgb_d.b = p;
    break;
  case 2:
    rgb_d.r = p;
    rgb_d.g = hsv.v;
    rgb_d.b = t;
    break;

  case 3:
    rgb_d.r = p;
    rgb_d.g = q;
    rgb_d.b = hsv.v;
    break;
  case 4:
    rgb_d.r = t;
    rgb_d.g = p;
    rgb_d.b = hsv.v;
    break;
  case 5:
  default:
    rgb_d.r = hsv.v;
    rgb_d.g = p;
    rgb_d.b = q;
    break;
  }

  out.r = rgb_d.r * 4095.0;
  out.g = rgb_d.g * 4095.0;
  out.b = rgb_d.b * 4095.0;

  return out;
}

hsv _rgb_to_hsv(rgb_14 in14bit)
{
  rgb_double in = {
      .r = (double)in14bit.r / 4095.0,
      .g = (double)in14bit.g / 4095.0,
      .b = (double)in14bit.b / 4095.0,
  };
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
 * Pulse Width Modulation (Color)
 */


void _set_pwm_seq(uint16_t *rgb, uint8_t *pwm_seq, uint8_t len) {
  for (uint8_t i=0; i < len; i++) {
    pwm_seq[i] = _12_to_8(rgb[i%3]);
  }
}

void _set_pwm_seq_hsv(hsv in, uint8_t *pwm_seq, uint8_t len) {
  rgb_14 rgb = _hsv_to_rgb(in);
  for (uint8_t i=0; i < len; i++) {
    switch (i%3) {
    case 0:
      pwm_seq[i] = _12_to_8(rgb.r);
      break;
    case 1:
      pwm_seq[i] = _12_to_8(rgb.g);
      break;
    case 2:
      pwm_seq[i] = _12_to_8(rgb.b);
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
