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

#define SEQ_MAX_STEP 63
#define SEQ_MAX_PAGE 3
#define SEQ_STEPS_PER_PAGE 16

void seq_init() {
  memset(&seq_changed, 1, sizeof(seq_changed_t));
}

void seq_changed_reset() {
  memset(&seq_state.button_changed, 0, SEQ_STEPS_PER_PAGE);
  memset(&seq_changed, 0, sizeof(seq_changed_t));
}

void seq_start () {
  seq_reset();
  seq_state.running = true;
  seq_changed.running = true;
}

void seq_stop() {
  seq_state.running = false;
  seq_changed.running = true;
}

void seq_continue() {
  seq_state.running = true;
  seq_changed.running = true;
}

void seq_reset() {
  seq_state.active_step = 0;
  seq_changed.active_step = true;
}

void seq_set_step(uint8_t step) {
  if (seq_state.active_step != step) {
    seq_state.active_step = step;
    seq_changed.active_step = true;
    uint8_t page = step / SEQ_STEPS_PER_PAGE;
    if (seq_state.active_page != page) {
      seq_state.active_page = page;
      seq_changed.active_page = true;
    }
  }
}

void seq_advance_step() {
  if (!seq_state.running) {
    // Mustve booted mid-sequence!
    seq_state.running = true;
    seq_changed.running = true;
  }
  if (seq_state.active_step == SEQ_MAX_STEP) {
    seq_set_step(0);
  } else {
    seq_set_step(seq_state.active_step + 1);
  }
}

void seq_advance_selected_page() {
  if (seq_state.selected_page == SEQ_MAX_PAGE) {
    seq_state.selected_page = 0;
  } else {
    seq_state.selected_page++;
  }
  seq_changed.selected_page = true;
}
