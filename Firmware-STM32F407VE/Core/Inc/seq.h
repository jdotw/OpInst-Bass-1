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

typedef struct seq_state_struct {
  seq_button_state_t button_state[16];

  bool running;

  uint8_t last_step;

  uint8_t active_step;
  uint8_t active_page;

  uint8_t selected_page;
  uint8_t selected_step;

  ctrl_t step_ctrl[64];
} seq_state_t;

typedef struct {
  seq_state_t state;
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

ctrl_t *seq_get_active_step_ctrl(seq_t *seq);

uint16_t *seq_button_led_rgb(seq_t *seq, uint8_t i);
uint16_t *seq_start_button_led_rgb(seq_t *seq, bool pressed);

#endif /* INC_SEQ_H_ */
