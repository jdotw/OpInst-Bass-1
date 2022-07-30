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
  memset(&seq.changed, 1, sizeof(seq.changed));
  seq.state.last_step = SEQ_DEFAULT_LAST_STEP;
  seq.state.selected_step = UINT8_MAX;
  seq.state.prev_active_step = UINT8_MAX;
}

void seq_changed_reset() { memset(&seq.changed, 0, sizeof(seq.changed)); }

void seq_start() {
  seq_reset();
  seq.state.running = true;
  seq.changed.running = true;
}

void seq_stop() {
  seq.state.running = false;
  seq.changed.running = true;
}

void seq_continue() {
  seq.state.running = true;
  seq.changed.running = true;
}

void seq_reset() {
  seq.state.active_step = 0;
  seq.changed.active_step = true;
}

void seq_set_step(uint8_t step) {
  if (seq.state.active_step != step) {
    seq.state.prev_selected_step = UINT8_MAX;
    seq.state.prev_active_step = seq.state.active_step;
    seq.state.active_step = step;
    seq.changed.active_step = true;
    uint8_t page = step / SEQ_STEPS_PER_PAGE;
    if (seq.state.active_page != page) {
      seq.state.active_page = page;
      seq.changed.active_page = true;
    }
  }
}

void seq_advance_step() {
  if (!seq.state.running) {
    // Mustve booted mid-sequence!
    seq.state.running = true;
    seq.changed.running = true;
  }
  if (seq.state.active_step == seq.state.last_step) {
    seq_set_step(0);
  } else {
    seq_set_step(seq.state.active_step + 1);
  }
}

void seq_advance_selected_page() {
  if (seq.state.selected_page == SEQ_MAX_PAGE) {
    seq.state.selected_page = 0;
  } else {
    seq.state.selected_page++;
  }
  seq.changed.selected_page = true;
}

void seq_apply_active_step_ctrl(seq_t *seq, ctrl_t *ctrl) {
  // Using the state in state_ptr, overlays the control values
  // that are set for that step (p-locks) onto the
  // ctrl_value_ptr and ctrl_changed_ptr
  // That is, if the step has a value set for any control
  // (noted by _changed = true flag for that value) then it
  // overrides the state in the ctrl_value_ptr struct and causes
  // the corresponding ctrl_changed_ptr flag to be set.
  //
  // This is gonna be a lot of code :|

  ctrl_t *step_ctrl = NULL;
  static ctrl_t *prev_ctrl = NULL;

  if (seq->state.selected_step != UINT8_MAX) {
    // User is holding down a step sequence button
    // This takes precedence
    step_ctrl = &seq->state.step_ctrl[seq->state.selected_step *
                                      (seq->state.selected_page + 1)];
  } else if (seq->state.running) {
    // Step seqencer is running
    step_ctrl = &seq->state.step_ctrl[seq->state.active_step];
  }

  for (uint8_t i = 0; i < CTRL_ENUM_MAX; i++) {
    if (step_ctrl && step_ctrl->changed[i]) {
      ctrl->value[i] = step_ctrl->value[i];
      ctrl->changed[i] = step_ctrl->changed[i];
    } else if (prev_ctrl && prev_ctrl->changed[i]) {
      ctrl->changed[i] = true;
    }
  }

  prev_ctrl = step_ctrl;
}
