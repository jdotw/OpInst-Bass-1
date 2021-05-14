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
void commit_led_osc(void);

static commit_cycle_t cycle;

void commit_30hz_timer(void) {
	uint32_t ticks_before = 0;
	uint32_t ticks_after = 0;
	uint32_t ticks_cost = 0;
	commit_dac();
	commit_gatetrig();
	switch(cycle) {
	case COMMIT_LED_ROTPIC:
		ticks_before = HAL_GetTick();
		commit_led_rotpic();
		ticks_after = HAL_GetTick();
		ticks_cost = ticks_after - ticks_before;
		cycle++;
		break;
	case COMMIT_LED_ADSR_OSC_FILT:
	case COMMIT_LED_ADSR_SUB_FILT:
	case COMMIT_LED_ADSR_OSC_AMP:
	case COMMIT_LED_ADSR_SUB_AMP:
		ticks_before = HAL_GetTick();
		commit_led_adsr(cycle);
		ticks_after = HAL_GetTick();
		ticks_cost = ticks_after - ticks_before;
		cycle++;
		break;
	case COMMIT_LED_OSC:
		ticks_before = HAL_GetTick();
		commit_led_osc();
		ticks_after = HAL_GetTick();
		ticks_cost = ticks_after - ticks_before;
		cycle++;
		break;
	default:
		cycle = 0;
	}
}
