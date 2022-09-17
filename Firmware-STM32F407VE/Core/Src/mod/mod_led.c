/*
 * mod_led.c
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

/*
 * RGB Calculations
 */

static uint16_t val[3];

#define MIN_PWM (uint16_t)0x0000
#define MAX_PWM (uint16_t)0xFFFF // 16bit

uint16_t *mod_start_button_led_rgb(seq_t *seq, bool pressed) {
  if (pressed) {
    val[0] = MAX_PWM;
    val[1] = MAX_PWM;
    val[2] = MAX_PWM;
  } else {
    val[0] = MIN_PWM;
    val[1] = seq->state.running ? MAX_PWM : MIN_PWM;
    val[2] = MIN_PWM;
  }
  return val;
}

uint16_t *mod_shift_button_led_rgb(bool pressed) {
  val[0] = pressed ? MAX_PWM : MIN_PWM;
  val[1] = pressed ? MAX_PWM : MIN_PWM;
  val[2] = pressed ? MAX_PWM : MIN_PWM;
  return val;
}
