/*
 * rotpic_led.c
 *
 *  Created on: 30 Apr 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include <stdio.h>

#include "ctrl.h"
#include "i2c.h"
#include "main.h"
#include "osc.h"
#include "rotpic.h"

bool rotpic_led_set_state(uint8_t bus, uint8_t channel, uint8_t pic,
                          uint8_t led_state) {
  // led_state is a single-byte bit array:
  // 0 = LED 1 On=1 Off=0
  // 1 = LED 2 On=1 Off=0
  // 2 = LED 3 On=1 Off=0
  // 3 = LED 4 On=1 Off=0
  // 4 = LED 5 On=1 Off=0
  // 5 = LED 6 On=1 Off=0
  // 6 = Unused
  // 7 = Unused

  bool res;

  res = i2c_tx(bus, channel, (DEFAULT_ROTPIC_ADDR + pic), &led_state, 1, NULL,
               NULL);

  return res;
}
