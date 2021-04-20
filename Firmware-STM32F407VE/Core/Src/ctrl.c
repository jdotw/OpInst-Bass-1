/*
 * ctrl.c
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#include <stdint.h>
#include <stdbool.h>

void ctrl_apply_delta(uint16_t *ctrl, int8_t enc_delta, int16_t scale_percent, uint16_t min_value, uint16_t max_value) {
	bool did_overflow = false;
	int16_t scaled_enc_delta = (scale_percent / 100) * enc_delta;
	if (scaled_enc_delta > 0) {
		if (scaled_enc_delta > max_value - *ctrl) {
			// Overflow max
			*ctrl = max_value;
			did_overflow = true;
		} else {
			*ctrl += scaled_enc_delta;
		}
	} else if (scaled_enc_delta < 0) {
		if ((scaled_enc_delta * -1) > *ctrl) {
			// Overflow min
			*ctrl = min_value;
		} else {
			*ctrl += scaled_enc_delta;
		}
	}
	if (did_overflow) {
		ctrl_overflow_handler();
	}
}

void ctrl_overflow_handler() {
	// TODO: Buzz the haptic
}
