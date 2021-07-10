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

uint8_t _max(uint16_t a, uint16_t b) {
  if (a > b) return a;
  else return b;
}

uint8_t _min(uint16_t a, uint16_t b) {
  if (a < b) return a;
  else return b;
}


uint8_t _12_to_8(uint16_t a) {
  return a >> 4;
}

#define DEFAULT_SCALE 0x15
#define DEFAULT_SCALE_R 0x17
#define DEFAULT_SCALE_G 0x07
#define DEFAULT_SCALE_B 0x16

void _rgb_copy(uint16_t *dst, uint16_t *src) {
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
}

void _set_pwm_seq(uint16_t *rgb, uint8_t *pwm_seq, uint8_t len) {
  for (uint8_t i=0; i < len; i++) {
    pwm_seq[i] = _12_to_8(rgb[i%3]);
  }
}

void _set_scale_seq(uint8_t *pwm_seq, uint8_t *scale_seq, uint8_t len) {
  for (uint8_t i=0; i < len; i++) {
    switch(i%3) {
    case 0:
      scale_seq[i] = DEFAULT_SCALE_R;
      break;
    case 1:
      scale_seq[i] = DEFAULT_SCALE_G;
      break;
    case 2:
      scale_seq[i] = DEFAULT_SCALE_B;
      break;
    }
  }
}
