/*
 * seq.h
 *
 *  Created on: 10 Jul 2021
 *      Author: jwilson
 */

#ifndef INC_SEQ_H_
#define INC_SEQ_H_

#include "ctrl.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"

#define SEQ_MAX_STEPS 64

typedef struct {
  bool pressed; // pressed = button is down
  bool active;  // active = this step is playing now
  bool set;     // set = this step has parameters set
} seq_button_state_t;

struct seq_state_struct {

  seq_button_state_t button_state[16];
  bool button_changed[16];

  bool running;

  uint8_t last_step;

  uint8_t prev_active_step;
  uint8_t active_step;
  uint8_t active_page;

  uint8_t selected_page;
  uint8_t selected_step;
  uint8_t prev_selected_page;
  uint8_t prev_selected_step;

  ctrl_t step_ctrl[64];
};

struct seq_changed_struct {
  bool running;
  bool last_step;
  bool active_step;
  bool active_page;
  bool selected_step;
  bool selected_page;
};

typedef struct {
  struct seq_state_struct state;
  struct seq_changed_struct changed;
} seq_t;

seq_t *seq_get(void);

void seq_init();
void seq_start();
void seq_stop();
void seq_continue();
void seq_reset(void);
void seq_set_step(uint8_t step);
void seq_advance_step(void);
void seq_advance_selected_page(void);

void seq_poll_gpio(uint8_t bus, uint8_t channel);
void seq_poll_mcu_gpio();
void seq_changed_reset();

void seq_apply_active_step_ctrl(seq_t *seq, ctrl_t *ctrl);

#endif /* INC_SEQ_H_ */
