/*
 * commit.c
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#include "commit.h"
#include "../../lvgl/lvgl.h"
#include "app_bluenrg_2.h"
#include "blink.h"
#include "ctrl.h"
#include "dac7678.h"
#include "i2c.h"
#include "main.h"
#include "mod.h"
#include "note.h"
#include "oled.h"
#include "osc.h"
#include "seq.h"
#include <math.h>
#include <stdbool.h>

void commit_dac(ctrl_t *ctrl, note_t *note);
void commit_gatetrig(note_t *note);
void commit_led_rotpic(ctrl_toggle_t *toggle);
void commit_led_adsr(commit_cycle_t cycle);
void commit_led_osc(commit_cycle_t cycle);
void commit_led_button(commit_cycle_t cycle, seq_state_t *seq,
                       seq_changed_t *changed, mod_state_t *mod);

static commit_cycle_t cycle;

uint16_t pattern_cycle_count = 0;

void commit_30hz_timer(void) {
  uint32_t total_ticks_before = HAL_GetTick();
  uint32_t ticks_before = 0;
  uint32_t ticks_after = 0;
  uint32_t ticks_cost = 0;

  note_changed_reset();

  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

  ctrl_t *ctrl = ctrl_get_active();
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  note_t *note = note_get_active();

  ticks_before = HAL_GetTick();
  commit_dac(ctrl, note);
  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;

  ticks_before = HAL_GetTick();
  commit_gatetrig(note);
  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;

  switch (cycle) {
  case COMMIT_INIT:
    // Copy the control values, changed and toggle structs
    // Our commit functions will then work off these values

    // Apply p-lock
    seq_apply_active_step_ctrl(&seq_state, &seq_changed, ctrl);

    // Then reset the change flag so that any further changes
    // will be waiting for us on the next cycle
    ctrl_changed_reset();
    seq_changed_reset();
    mod_state_changed_reset();
    blink_reset();

    pattern_cycle_count++;
    if (pattern_cycle_count > 1) {
      increment_pattern_step();
      pattern_cycle_count = 0;
    }

    cycle++;
    break;
  case COMMIT_LED_ROTPIC:
    ticks_before = HAL_GetTick();
    commit_led_rotpic(toggle);
    ticks_after = HAL_GetTick();
    ticks_cost = ticks_after - ticks_before;
    cycle++;
    break;
  case COMMIT_LED_ADSR_OSC_FILT_A:
  case COMMIT_LED_ADSR_OSC_FILT_D:
  case COMMIT_LED_ADSR_OSC_FILT_S:
  case COMMIT_LED_ADSR_OSC_FILT_R:
  case COMMIT_LED_ADSR_SUB_FILT_A:
  case COMMIT_LED_ADSR_SUB_FILT_D:
  case COMMIT_LED_ADSR_SUB_FILT_S:
  case COMMIT_LED_ADSR_SUB_FILT_R:
  case COMMIT_LED_ADSR_OSC_AMP_A:
  case COMMIT_LED_ADSR_OSC_AMP_D:
  case COMMIT_LED_ADSR_OSC_AMP_S:
  case COMMIT_LED_ADSR_OSC_AMP_R:
  case COMMIT_LED_ADSR_SUB_AMP_A:
  case COMMIT_LED_ADSR_SUB_AMP_D:
  case COMMIT_LED_ADSR_SUB_AMP_S:
  case COMMIT_LED_ADSR_SUB_AMP_R:
    ticks_before = HAL_GetTick();
    commit_led_adsr(cycle);
    ticks_after = HAL_GetTick();
    ticks_cost = ticks_after - ticks_before;
    cycle++;
    break;
  case COMMIT_LED_OSC1_SAW:
  case COMMIT_LED_OSC1_SQU:
  case COMMIT_LED_OSC1_MIX:
  case COMMIT_LED_OSC1_FILT_FREQ:
  case COMMIT_LED_OSC1_FILT_RESO:
  case COMMIT_LED_OSC1_DRIVE:
  case COMMIT_LED_OSC1_TO_OSC2:
  case COMMIT_LED_OSC2_SAW:
  case COMMIT_LED_OSC2_SQU:
  case COMMIT_LED_OSC2_NOISE:
  case COMMIT_LED_OSC2_ONLY:
  case COMMIT_LED_OSC2_PREFILT:
  case COMMIT_LED_OSC2_FILT_FREQ:
  case COMMIT_LED_OSC2_FILT_RESO:
  case COMMIT_LED_OSC2_DRIVE:
  case COMMIT_LED_OSC_AMP_OUT:
  case COMMIT_LED_SUB_AMP_OUT:
  case COMMIT_LED_SUB_SQU:
  case COMMIT_LED_SUB_NOISE:
  case COMMIT_LED_SUB_MIX:
  case COMMIT_LED_SUB_TO_OSC2:
  case COMMIT_LED_SUB_FILT_FREQ:
  case COMMIT_LED_SUB_FILT_RESO:
  case COMMIT_LED_FX_DRY:
  case COMMIT_LED_FX_WET:
  case COMMIT_LED_FX_FEEDBACK:
    ticks_before = HAL_GetTick();
    commit_led_osc(cycle);
    ticks_after = HAL_GetTick();
    ticks_cost = ticks_after - ticks_before;
    cycle++;
    break;
  case COMMIT_LED_BUTTON_STEP1TO12:
  case COMMIT_LED_BUTTON_STEP13TO16:
  case COMMIT_LED_BUTTON_SHIFTPAGE:
  case COMMIT_LED_BUTTON_START:
    commit_led_button(cycle);
    cycle++;
    break;
  case COMMIT_OLED_UPDATE:
    oled_commit();
    cycle++;
    break;
  case COMMIT_BLUETOOTH_UPDATE:
    cycle++;
    break;
  default:
    cycle = 0;
  }

  uint32_t total_ticks_after = HAL_GetTick();
  uint32_t total_ticks_cost = total_ticks_after - total_ticks_before;

  UNUSED(ticks_cost);
  UNUSED(total_ticks_cost);

  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
