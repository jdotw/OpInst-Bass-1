/*
 * mod.h
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */

#ifndef INC_MOD_H_
#define INC_MOD_H_

#include "stdbool.h"
#include "stm32f4xx_hal.h"

struct mod_button_struct {
  bool start;
  bool shift;
  bool page;
  bool up;
  bool down;
};

typedef struct {
  struct mod_button_struct state;
} mod_t;

mod_t *mod_get();

uint16_t *mod_shift_button_led_rgb(bool pressed);

#endif /* INC_MOD_H_ */
