/*
 * seq_led.c
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

uint16_t *seq_button_led_rgb(seq_t *seq, uint8_t i) {
  if (seq->state.button_state[i].pressed) {
    val[0] = MAX_PWM;
    val[1] = MAX_PWM;
    val[2] = MAX_PWM;
  } else if (seq->state.active_page == seq->state.selected_page) {
    bool isActive = i == seq->state.active_step % 16 && seq->state.running;
    val[0] = MIN_PWM;
    val[1] = isActive ? MAX_PWM : MIN_PWM;
    val[2] = MIN_PWM;
  } else {
    val[0] = MIN_PWM;
    val[1] = MIN_PWM;
    val[2] = MIN_PWM;
  }
  return val;
}
