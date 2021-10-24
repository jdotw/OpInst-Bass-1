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

typedef struct {
  bool start;
  bool shift;
  bool page;
  bool up;
  bool down;
} mod_button_state_t;

typedef struct {
  bool start;
  bool shift;
  bool page;
  bool up;
  bool down;
} mod_button_changed_t;

typedef struct {
  mod_button_state_t button_state;
  mod_button_changed_t button_changed;
} mod_state_t;

extern mod_state_t mod_state;

void mod_state_changed_reset(void);

#endif /* INC_MOD_H_ */
