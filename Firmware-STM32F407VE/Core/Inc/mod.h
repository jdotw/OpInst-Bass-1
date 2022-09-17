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
  struct mod_button_struct changed;
} mod_t;

mod_t *mod_get();

void mod_changed_reset(void);

uint16_t *mod_shift_button_led_rgb(bool pressed);

#endif /* INC_MOD_H_ */
