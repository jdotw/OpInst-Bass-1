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
#define SEQ_DEFAULT_LAST_STEP 15

void seq_init() {
  memset(&seq_changed, 1, sizeof(seq_changed_t));
  seq_state.last_step = SEQ_DEFAULT_LAST_STEP;
  seq_state.selected_step = UINT8_MAX;
  seq_state.prev_active_step = UINT8_MAX;
}

void seq_changed_reset() {
  memset(&seq_state.button_changed, 0, SEQ_STEPS_PER_PAGE);
  memset(&seq_changed, 0, sizeof(seq_changed_t));
}

void seq_start() {
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
    seq_state.prev_active_step = seq_state.active_step;
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
  if (seq_state.active_step == seq_state.last_step) {
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

void seq_apply_active_step_ctrl(seq_state_t *state_ptr,
                                seq_changed_t *change_ptr, ctrl_t *ctrl_ptr) {
  // Using the state in state_ptr, overlays the control values
  // that are set for that step (p-locks) onto the
  // ctrl_value_ptr and ctrl_changed_ptr
  // That is, if the step has a value set for any control
  // (noted by _changed = true flag for that value) then it
  // overrides the state in the ctrl_value_ptr struct and causes
  // the corresponding ctrl_changed_ptr flag to be set.
  //
  // This is gonna be a lot of code :|

  ctrl_t *step_ctrl_ptr = NULL;
  ctrl_t *prev_ctrl_ptr = NULL;

  if (state_ptr->selected_step != UINT8_MAX) {
    // User is holding down a step sequence button
    // This takes precedence
    step_ctrl_ptr = &state_ptr->step_ctrl[state_ptr->selected_step *
                                          (state_ptr->selected_page + 1)];
  } else if (state_ptr->running) {
    // Step seqencer is running
    step_ctrl_ptr = &state_ptr->step_ctrl[state_ptr->active_step];
  }

  if (change_ptr->selected_step && state_ptr->prev_selected_step != UINT8_MAX) {
    // There was a change in selected (user pressed) step
    prev_ctrl_ptr = &state_ptr->step_ctrl[state_ptr->prev_selected_step *
                                          (state_ptr->prev_selected_page + 1)];
  } else if (change_ptr->active_step &&
             state_ptr->prev_active_step != UINT8_MAX) {
    // The step sequencer has advance
    prev_ctrl_ptr = &state_ptr->step_ctrl[state_ptr->prev_active_step];
  }

  for (uint8_t i = 0; i < CTRL_ENUM_MAX; i++) {
    if (step_ctrl_ptr && step_ctrl_ptr->changed[i]) {
      ctrl_ptr->value[i] = step_ctrl_ptr->value[i];
      ctrl_ptr->changed[i] =
          (change_ptr->active_step || change_ptr->selected_step ||
           step_ctrl_ptr->changed[i]);
    } else if (prev_ctrl_ptr && prev_ctrl_ptr->changed[i]) {
      ctrl_ptr->changed[i] = true;
    }
  }
}
