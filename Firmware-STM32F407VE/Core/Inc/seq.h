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
#include "ctrl.h"

typedef struct {
  bool pressed; // pressed = button is down
  bool active;  // active = this step is playing now
  bool set;     // set = this step has parameters set
} seq_button_state_t;

typedef struct {
  seq_button_state_t button_state[16];
  bool button_changed[16];

  bool running;
  uint8_t prev_step;
  uint8_t active_step;
  bool active_step_changed;
  uint8_t active_page;

  uint8_t selected_page;
  uint8_t selected_step;

  ctrl_value_t step_ctrl_value[64];
  ctrl_changed_t step_ctrl_changed[64];
} seq_state_t;

typedef struct {
  bool running;
  bool active_step;
  bool active_page;
  bool selected_page;
} seq_changed_t;

extern seq_state_t seq_state;
extern seq_changed_t seq_changed;

void seq_init();
void seq_start ();
void seq_stop();
void seq_continue();
void seq_reset(void);
void seq_set_step(uint8_t step);
void seq_advance_step(void);
void seq_advance_selected_page(void);

void seq_poll_gpio(uint8_t bus, uint8_t channel);
void seq_poll_mcu_gpio();
void seq_changed_reset();

void seq_apply_active_step_ctrl(seq_state_t *state_ptr, ctrl_value_t *ctrl_value_ptr, ctrl_changed_t *ctrl_changed_ptr);

#endif /* INC_SEQ_H_ */
