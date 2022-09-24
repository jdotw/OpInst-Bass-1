/*
 * seq_poll.c
 *
 *  Created on: 10 Jul 2021
 *      Author: jwilson
 */

#include "i2c.h"
#include "main.h"
#include "mod.h"
#include "pca9555.h"
#include "seq.h"

// LEFT2:0 PCA9555 IO Mux
#define START_PIN 1 << 0
#define STEP_1_PIN 1 << 1
#define STEP_2_PIN 1 << 2
#define STEP_3_PIN 1 << 3
#define STEP_4_PIN 1 << 4
#define STEP_5_PIN 1 << 5
#define STEP_6_PIN 1 << 6
#define STEP_7_PIN 1 << 7
#define STEP_8_PIN 1 << 8
#define STEP_9_PIN 1 << 9
#define STEP_10_PIN 1 << 10
#define STEP_11_PIN 1 << 11
#define STEP_12_PIN 1 << 12
#define STEP_13_PIN 1 << 13
#define STEP_14_PIN 1 << 14
#define STEP_15_PIN 1 << 15

// RIGHT2:0 PCA9555 IO Mux
#define STEP_16_PIN 1 << 0
#define AUX_PIN 1 << 1
#define UP_PIN 1 << 14
#define DOWN_PIN 1 << 15

void seq_poll_gpio(uint8_t bus, uint8_t channel) {
  seq_t *seq = seq_get();
  mod_t *mod = mod_get();
  uint16_t pin_state;
  bool res = pca9555_read_pin_state(bus, channel, 0, &pin_state);
  if (!res) {
    Error_Handler();
  }

  if (channel == 2) {
    // Handle step sequencer switches
    // The steps are awkwardly wired across both the LEFT and RIGH I/O
    // Steps 1-15 are on LEFT:2, but 16 is on RIGHT:2
    // Hence the masking depending on which bus we're talking to
    // And also the off-by-one bit shifting -- fix it in software, i said. @_@
    uint16_t step_pin_state =
        (bus == I2C_LEFT) ? pin_state | 0x0001 : pin_state | 0xFFFE;
    for (uint8_t i = 0; i < 16; i++) {
      uint16_t pin_mask = i == 15 ? 1 << 0 : 1 << (i + 1);
      bool pressed = !(step_pin_state & pin_mask);
      if (seq->state.button_state[i].pressed != pressed) {
        // State change for this button
        seq->state.button_state[i].pressed = pressed;
      }
    }

    // Handle modifier switches
    switch (bus) {
    case I2C_LEFT:
      if (mod->state.start != !(pin_state & START_PIN)) {
        mod->state.start = !(pin_state & START_PIN);
      }
      break;
    case I2C_RIGHT:
      if (mod->state.page != !(pin_state & AUX_PIN)) {
        mod->state.page = !(pin_state & AUX_PIN);
        if (mod->state.page) {
          seq_advance_selected_page();
        }
      }
      if (mod->state.up != !(pin_state & UP_PIN)) {
        mod->state.up = !(pin_state & UP_PIN);
      }
      if (mod->state.down != !(pin_state & DOWN_PIN)) {
        mod->state.down = !(pin_state & DOWN_PIN);
      }
      break;
    }

    if (seq->state.selected_step == UINT8_MAX ||
        (seq->state.selected_step / 4) != seq->state.selected_page ||
        !seq->state.button_state[(seq->state.selected_step % 16)].pressed) {
      // Selected step has changed
      seq->state.selected_step = UINT8_MAX; // Default to no selection
      for (uint8_t i = 0; i < 16; i++) {
        if (seq->state.button_state[i].pressed) {
          // Set new selection
          seq->state.selected_step = i * (seq->state.selected_page + 1);
          break; // Always select the lowest button
        }
      }
    }
  }
}

void seq_poll_mcu_gpio() {
  mod_t *mod = mod_get();
  bool shift_pressed = HAL_GPIO_ReadPin(SHIFTSW_GPIO_Port, SHIFTSW_Pin) ==
                       GPIO_PIN_RESET; // Pulled down
  if (mod->state.shift != shift_pressed) {
    mod->state.shift = shift_pressed;
  }
}
