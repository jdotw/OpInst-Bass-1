/*
 * seq.h
 *
 *  Created on: 10 Jul 2021
 *      Author: jwilson
 */

#ifndef INC_SEQ_H_
#define INC_SEQ_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"

typedef struct {
  bool pressed; // pressed = button is down
  bool active;  // active = this step is playing now
  bool set;     // set = this step has parameters set
} seq_button_state_t;

typedef struct {
  seq_button_state_t button_state[16];
  bool button_changed[16];
} seq_state_t;

extern seq_state_t seq_state;

void seq_poll_gpio(uint8_t bus, uint8_t channel);
void seq_poll_mcu_gpio();

#endif /* INC_SEQ_H_ */
