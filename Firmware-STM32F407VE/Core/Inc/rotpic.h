/*
 * rotpic.h
 *
 *  Created on: 17 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_ROTPIC_H_
#define INC_ROTPIC_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define DEFAULT_ROTPIC_ADDR 0x50

/* rotpic.c */

typedef struct {
  bool success;
  int8_t enc1_delta;
  int8_t enc2_delta;
  int8_t enc3_delta;
  int8_t enc4_delta;
  bool sw1_state;
  bool sw1_changed;
  bool sw2_state;
  bool sw2_changed;
} rotpic_state;

void rotpic_poll_all(uint8_t bus, uint8_t channel);

/* rotpic_led.c */

#define ROTPIC_LED1_BIT_SHIFT 0
#define ROTPIC_LED2_BIT_SHIFT 1
#define ROTPIC_LED3_BIT_SHIFT 2
#define ROTPIC_LED4_BIT_SHIFT 3
#define ROTPIC_LED5_BIT_SHIFT 4
#define ROTPIC_LED6_BIT_SHIFT 5
#define ROTPIC_LED7_BIT_SHIFT 6

bool rotpic_led_set_state(uint8_t bus, uint8_t channel, uint8_t pic,
                          uint8_t led_state, i2c_callback_t callback,
                          void *userdata);

#endif /* INC_ROTPIC_H_ */
