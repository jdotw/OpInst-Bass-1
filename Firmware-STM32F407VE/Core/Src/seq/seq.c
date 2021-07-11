/*
 * seq.c
 *
 *  Created on: 10 Jul 2021
 *      Author: jwilson
 */

#include "seq.h"
#include "string.h"

seq_state_t seq_state;
seq_changed_t seq_changed;

#define SEQ_MAX_STEP 15

void seq_changed_reset() {
  memset(&seq_state.button_changed, 0, 16);
  memset(&seq_changed, 0, sizeof(seq_changed_t));
}

void seq_reset() {
  seq_state.active_step = 0;
  seq_changed.active_step = true;
}

void seq_set_step(uint8_t step) {
  seq_state.active_step = step;
  seq_changed.active_step = true;
}

void seq_advance_step() {
  if (seq_state.active_step == SEQ_MAX_STEP) {
    seq_state.active_step = 0;
  } else {
    seq_state.active_step++;
  }
  seq_changed.active_step = true;
}
