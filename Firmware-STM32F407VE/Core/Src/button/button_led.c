/*
 * button_led.c
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */

#include "blink.h"
#include "commit.h"
#include "ctrl.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"
#include "string.h"

#define DEFAULT_BUTTON_SCALE 0x15
#define DEFAULT_BUTTON_SCALE_R 0x37
#define DEFAULT_BUTTON_SCALE_G 0x27
#define DEFAULT_BUTTON_SCALE_B 0x36

void button_led_set_scale_seq(uint16_t *pwm_seq, uint8_t *scale_seq,
                              uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    switch (i % 3) {
    case 0:
      scale_seq[i] = DEFAULT_BUTTON_SCALE_R;
      break;
    case 1:
      scale_seq[i] = DEFAULT_BUTTON_SCALE_G;
      break;
    case 2:
      scale_seq[i] = DEFAULT_BUTTON_SCALE_B;
      break;
    }
  }
}

void button_led_set_pwm_seq(uint16_t *seqptr, uint16_t *val) {
  memcpy(seqptr, val, 3 * (sizeof(uint16_t)));
}
