/*
 * commit.c
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "commit.h"
#include "ctrl.h"
#include "main.h"
#include "i2c.h"
#include "dac7678.h"
#include "osc.h"
#include "note.h"
#include <math.h>

void commit_dac(void);
void commit_gatetrig(void);
void commit_led_rotpic(void);
void commit_led_adsr(commit_cycle_t cycle);
void commit_led_osc(commit_cycle_t cycle);

static commit_cycle_t cycle;
ctrl_value_t commit_ctrl_value;
ctrl_changed_t commit_ctrl_changed;
ctrl_toggle_t commit_ctrl_toggle;
note_value_t commit_note_value;
note_changed_t commit_note_changed;

void commit_30hz_timer(void) {
	uint32_t total_ticks_before = HAL_GetTick();
	uint32_t ticks_before = 0;
	uint32_t ticks_after = 0;
	uint32_t ticks_cost = 0;

  commit_note_value = note_value;
  commit_note_changed = note_changed;
  note_changed_reset();

  ticks_before = HAL_GetTick();
  commit_dac();
  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;

  ticks_before = HAL_GetTick();
  commit_gatetrig();
  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;

	switch(cycle) {
	case COMMIT_INIT:
	  // Copy the control values, changed and toggle structs
	  // Our commit functions will then work off these values
	  commit_ctrl_value = ctrl_value;
	  commit_ctrl_changed = ctrl_changed;
	  commit_ctrl_toggle = ctrl_toggle;

	  // Then reset the change flag so that any further changes
	  // will be waiting for us on the next cycle
    ctrl_changed_reset();

    cycle++;
	  break;
	case COMMIT_LED_ROTPIC:
		ticks_before = HAL_GetTick();
		commit_led_rotpic();
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
	case COMMIT_LED_OSC1_FILT:
	case COMMIT_LED_OSC1_DRIVE:
	case COMMIT_LED_OSC1_TO_OSC2:
	case COMMIT_LED_OSC2_SAW:
	case COMMIT_LED_OSC2_SQU:
	case COMMIT_LED_OSC2_NOISE:
	case COMMIT_LED_OSC2_ONLY:
	case COMMIT_LED_OSC2_PREFILT:
	case COMMIT_LED_OSC2_FILT:
	case COMMIT_LED_OSC2_DRIVE:
	case COMMIT_LED_OSC_AMP_OUT:
	case COMMIT_LED_SUB_AMP_OUT:
	case COMMIT_LED_SUB_SQU:
	case COMMIT_LED_SUB_NOISE:
	case COMMIT_LED_SUB_MIX:
	case COMMIT_LED_SUB_TO_OSC2:
	case COMMIT_LED_SUB_FILT_OUT:
	case COMMIT_LED_FX_DRY:
	case COMMIT_LED_FX_WET:
	case COMMIT_LED_FX_FEEDBACK:
		ticks_before = HAL_GetTick();
		commit_led_osc(cycle);
		ticks_after = HAL_GetTick();
		ticks_cost = ticks_after - ticks_before;
		cycle++;
		break;
	default:
		cycle = 0;
	}
	uint32_t total_ticks_after = HAL_GetTick();
	uint32_t total_ticks_cost = total_ticks_after - total_ticks_before;

	UNUSED(ticks_cost);
	UNUSED(total_ticks_cost);
}
