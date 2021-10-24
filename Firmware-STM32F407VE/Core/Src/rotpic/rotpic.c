/*
 * rotpic.c
 *
 *  Created on: 17 Apr 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "ctrl.h"
#include "i2c.h"
#include "main.h"
#include "oled.h"
#include "osc.h"
#include "rotpic.h"
#include "tca9544a.h"

#define ROTPIC_SW1_STATE_MASK 0b00000001
#define ROTPIC_SW1_CHANGED_MASK 0b00000010
#define ROTPIC_SW2_STATE_MASK 0b00000100
#define ROTPIC_SW2_CHANGED_MASK 0b00001000

rotpic_state _rotpic_poll_selected(uint8_t bus, uint8_t channel, uint8_t pic) {
  rotpic_state state;

  uint8_t rx[5];
  bool res = i2c_rx(bus, channel, DEFAULT_ROTPIC_ADDR + pic, rx, 5);
  if (!res) {
    // Rotpic doesnt exist?
    state.success = false;
    return state;
  }

  state.success = true;
  state.enc1_delta = rx[0];
  state.enc2_delta = rx[1];
  state.enc3_delta = rx[2];
  state.enc4_delta = rx[3];
  state.sw1_state = rx[4] & ROTPIC_SW1_STATE_MASK;
  state.sw1_changed = rx[4] & ROTPIC_SW1_CHANGED_MASK;
  state.sw2_state = rx[4] & ROTPIC_SW2_STATE_MASK;
  state.sw2_changed = rx[4] & ROTPIC_SW2_CHANGED_MASK;

  return state;
}

void _rotpic_handle_state(uint8_t bus, uint8_t channel, uint8_t pic,
                          rotpic_state state) {
  // Depending on which PIC this is, update actual state values
  if (bus == I2C_LEFT) {
    // Left I2C
    switch (channel) {
    case 0:
      // Left I2C 0
      switch (pic) {
      case 0:
        // Left 0:000
        ctrl_apply_delta(ENC_OSC1_SAW, state.enc1_delta);
        ctrl_apply_delta(ENC_OSC1_SQU, state.enc2_delta);
        ctrl_apply_delta(ENC_OSC1_TO_OSC2, state.enc4_delta);
        ctrl_apply_toggle(ENC_OSC1_SQU, state.sw2_changed, state.sw2_state);
        break;
      case 1:
        // Left 0:001
        ctrl_apply_delta(ENC_OSC1_DRIVE, state.enc1_delta);
        ctrl_apply_delta(ENC_OSC1_FILT_RES, state.enc2_delta);
        ctrl_apply_delta(ENC_OSC1_TUNE, (state.enc3_delta * -1));
        ctrl_apply_delta(ENC_OSC1_FILT_CUTOFF, (state.enc4_delta * -1));
        ctrl_apply_toggle(ENC_OSC1_TUNE, state.sw1_changed, state.sw1_state);
        break;
      case 2:
        // Left 0:010
        ctrl_apply_delta(ENC_OSC_FILT_ENV_A, (state.enc1_delta * -1));
        ctrl_apply_delta(ENC_OSC_FILT_ENV_S, (state.enc2_delta * -1));
        ctrl_apply_delta(ENC_OSC_FILT_ENV_D, (state.enc3_delta * -1));
        ctrl_apply_delta(ENC_OSC_FILT_ENV_R, (state.enc4_delta * -1));
        ctrl_apply_toggle(ENC_OSC_FILT_ENV_A, state.sw1_changed,
                          state.sw1_state);
        ctrl_apply_toggle(ENC_OSC_FILT_ENV_S, state.sw2_changed,
                          state.sw2_state);
        break;
      case 3:
        // Left 0:011
        ctrl_apply_delta(ENC_OSC2_SAW, state.enc1_delta);
        ctrl_apply_delta(ENC_OSC2_NOISE, state.enc2_delta);
        ctrl_apply_delta(ENC_OSC2_SQU, state.enc3_delta);
        ctrl_apply_toggle(ENC_OSC2_SQU, state.sw1_changed, state.sw1_state);
        break;
      case 4:
        // Left 0:100
        ctrl_apply_delta(ENC_SUB, (state.enc3_delta * -1));
        ctrl_apply_delta(ENC_SUB_NOISE, (state.enc1_delta * -1));
        ctrl_apply_delta(ENC_SUB_TO_OSC2, (state.enc2_delta * -1));
        break;
      }
      break;
    case 2:
      // Left I2C 2
      switch (pic) {
      case 0:
        // Left 2:000
        ctrl_apply_delta(ENC_SUB_FILT_ENV_A, state.enc3_delta);
        ctrl_apply_delta(ENC_SUB_FILT_ENV_D, state.enc1_delta);
        ctrl_apply_delta(ENC_SUB_FILT_ENV_S, state.enc4_delta);
        ctrl_apply_delta(ENC_SUB_FILT_ENV_R, state.enc2_delta);
        ctrl_apply_toggle(ENC_SUB_FILT_ENV_A, state.sw1_changed,
                          state.sw1_state);
        ctrl_apply_toggle(ENC_SUB_FILT_ENV_S, state.sw2_changed,
                          state.sw2_state);
        break;
      case 1:
        // Left 2:001
        ctrl_apply_delta(ENC_SUB_FILT_CUTOFF, (state.enc1_delta * -1));
        ctrl_apply_delta(ENC_SUB_FILT_RES, (state.enc2_delta * -1));
      }
      break;
    case 3:
      // Left I2C 3
      switch (pic) {
      case 0:
        // Left 3:000
        ctrl_apply_delta(ENC_OSC2_FILT_CUTOFF, (state.enc1_delta * -1));
        ctrl_apply_delta(ENC_OSC2_FILT_RES, state.enc3_delta);
        ctrl_apply_delta(ENC_OSC2_DRIVE, state.enc2_delta);
        if (oled_state == OLED_NAME_PRESET) {
          // ENC_FX_WETDRY = Change character
          oled_preset_apply_char_delta((state.enc4_delta * -1));
        } else {
          ctrl_apply_toggle(ENC_FX_WETDRY, state.sw1_changed, state.sw1_state);
          if (ctrl_toggle.fx_wetdry_func == ENC_FX_WETDRY_PRESET) {
            // ENC_FX_WETDRY = Select Preset (Pressed)
            preset_select_apply_delta((state.enc4_delta * -1));
          } else {
            // ENC_FX_WETDRY = FX Wet / Dry (Normal)
            ctrl_apply_delta(ENC_FX_WETDRY, (state.enc4_delta * -1));
          }
        }
        break;
      }
      break;
    }
  } else if (bus == I2C_RIGHT) {
    // Right I2C
    switch (channel) {
    case 0:
      // Right I2C 0
      switch (pic) {
      case 0:
        // Right 0:000
        ctrl_apply_delta(ENC_OSC_AMP_ENV_D, (state.enc1_delta * -1));
        ctrl_apply_delta(ENC_OSC_AMP_ENV_R, (state.enc2_delta * -1));
        ctrl_apply_delta(ENC_OSC_AMP_ENV_A, (state.enc3_delta * -1));
        ctrl_apply_delta(ENC_OSC_AMP_ENV_S, (state.enc4_delta * -1));
        ctrl_apply_toggle(ENC_OSC_AMP_ENV_S, state.sw2_changed,
                          state.sw2_state);
        break;
      }
      break;
    case 1:
      // Right I2C 1
      switch (pic) {
      case 0:
        // Right 1:000
        if (oled_state == OLED_NAME_PRESET) {
          // ENC_FX_VAL1 = Character Index
          oled_preset_apply_index_delta((state.enc4_delta * -1));
        } else {
          // ENC_FX_VAL1 = FX Value 1 (Normal)
          ctrl_apply_delta(ENC_FX_VAL1, (state.enc4_delta * -1));
        }
        ctrl_apply_delta(ENC_FX_VAL2, (state.enc1_delta * -1));
        ctrl_apply_delta(ENC_FX_VAL3, state.enc3_delta);
        ctrl_apply_delta(ENC_FX_VAL4, (state.enc2_delta * -1));
        break;
      case 1:
        // Right 1:001
        ctrl_apply_delta(ENC_SUB_AMP_ENV_A, (state.enc1_delta * -1));
        ctrl_apply_delta(ENC_SUB_AMP_ENV_D, (state.enc3_delta * -1));
        ctrl_apply_delta(ENC_SUB_AMP_ENV_S, (state.enc2_delta * -1));
        ctrl_apply_delta(ENC_SUB_AMP_ENV_R, (state.enc4_delta * -1));
        ctrl_apply_toggle(ENC_SUB_AMP_ENV_S, state.sw2_changed,
                          state.sw2_state);
      }
      break;
    }
  }
}

bool _rotpic_exists(uint8_t bus, uint8_t channel, uint8_t pic) {
  switch (bus) {
  case I2C_LEFT:
    switch (channel) {
    case 0: // LEFT0
      switch (pic) {
      case 0b000: // LEFT0:000
        return true;
      case 0b001: // LEFT0:001
        return true;
      case 0b010: // LEFT0:010
        return true;
      case 0b011: // LEFT0:011
        return true;
      case 0b100: // LEFT0:100
        return true;
      }
      break;
    case 2: // LEFT2
      switch (pic) {
      case 0b000: // LEFT2:000
        return true;
      case 0b001: // LEFT2:001
        return true;
      }
      break;
    case 3: // LEFT3
      switch (pic) {
      case 0b000: // LEFT3:000
        return true;
      }
      break;
    }
  case I2C_RIGHT:
    switch (channel) {
    case 0: // RIGHT0
      switch (pic) {
      case 0b000: // RIGHT0:000
        return true;
      }
      break;
    case 1: // RIGHT1
      switch (pic) {
      case 0b000: // RIGHT1:000
        return true;
      case 0b001: // RIGHT1:001
        return true;
      }
      break;
    }
  }
  return false;
}

void rotpic_poll_all(uint8_t bus, uint8_t channel) {
  for (uint8_t pic = 0; pic < 8; pic++) {
    bool poll = _rotpic_exists(bus, channel, pic);
    if (poll) {
      rotpic_state state = _rotpic_poll_selected(bus, channel, pic);
      if (state.success) {
        _rotpic_handle_state(bus, channel, pic, state);
      }
    }
  }
}
