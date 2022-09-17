/*
 * adsr.h
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */

#ifndef INC_ADSR_H_
#define INC_ADSR_H_

#include "stdbool.h"
#include "stm32f4xx_hal.h"

typedef struct adsr_s {
  uint16_t attack;
  uint16_t decay;
  uint16_t sustain;
  uint16_t release;
} adsr;

typedef struct adsr_grid_s {
  uint16_t led[3][3];
} adsr_grid_t;

// adsr.c
uint16_t adsr_control_voltage(adsr envelope, bool note_on,
                              uint32_t note_time_ticks);

// adsr_led.c

adsr_grid_t *adsr_led_set_grid_curve(uint16_t val);
adsr_grid_t *adsr_led_set_grid_stack(uint16_t val);
adsr_grid_t *adsr_led_set_grid_bar(uint16_t val);

#endif /* INC_ADSR_H_ */