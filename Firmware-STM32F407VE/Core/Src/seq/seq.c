/*
 * seq.c
 *
 *  Created on: 10 Jul 2021
 *      Author: jwilson
 */

#include "seq.h"
#include "string.h"

seq_t seq;

#define SEQ_MAX_STEP 63
#define SEQ_MAX_PAGE 3
#define SEQ_STEPS_PER_PAGE 16
#define SEQ_DEFAULT_LAST_STEP 15

seq_t *seq_get(void) { return &seq; }

void seq_init() {
  seq.state.last_step = SEQ_DEFAULT_LAST_STEP;
  seq.state.selected_step = UINT8_MAX;
  seq.state.prev_active_step = UINT8_MAX;
  for (uint8_t i = 0; i < SEQ_MAX_STEP; i++) {
    for (uint8_t j = 0; j < CTRL_ENUM_MAX; j++) {
      seq.state.step_ctrl[i].value[j] = UINT16_MAX;
    }
  }
}

void seq_start() {
  seq_reset();
  seq.state.running = true;
}

void seq_stop() { seq.state.running = false; }

void seq_continue() { seq.state.running = true; }

void seq_reset() { seq.state.active_step = 0; }

void seq_set_step(uint8_t step) {
  if (seq.state.active_step != step) {
    seq.state.prev_selected_step = UINT8_MAX;
    seq.state.prev_active_step = seq.state.active_step;
    seq.state.active_step = step;
    uint8_t page = step / SEQ_STEPS_PER_PAGE;
    if (seq.state.active_page != page) {
      seq.state.active_page = page;
    }
  }
}

void seq_advance_step() {
  if (!seq.state.running) {
    // Mustve booted mid-sequence!
    seq.state.running = true;
  }
  if (seq.state.active_step == seq.state.last_step) {
    seq_set_step(0);
  } else {
    seq_set_step(seq.state.active_step + 1);
  }
  i2c_resume_left_bus_reset();
}

void seq_advance_selected_page() {
  if (seq.state.selected_page == SEQ_MAX_PAGE) {
    seq.state.selected_page = 0;
  } else {
    seq.state.selected_page++;
  }
}

ctrl_t *seq_get_active_step_ctrl(seq_t *seq) {
  // Returns the ctrl_t for the step that is either
  // active or selected. That is, this is the 'overlay'
  // ctrl_t that is specified for this step
  ctrl_t *step_ctrl = NULL;

  if (seq->state.selected_step != UINT8_MAX) {
    // User is holding down a step sequence button
    // This takes precedence
    step_ctrl = &seq->state.step_ctrl[seq->state.selected_step *
                                      (seq->state.selected_page + 1)];
  } else if (seq->state.running) {
    // Step seqencer is running
    step_ctrl = &seq->state.step_ctrl[seq->state.active_step];
  }

  return step_ctrl;
}
