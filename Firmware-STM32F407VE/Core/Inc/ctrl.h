/*
 * ctrl.h
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_CTRL_H_
#define INC_CTRL_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	CTRL_OSC1_SAW,
	CTRL_OSC1_SQU,
	CTRL_OSC1_TO_OSC2,

	CTRL_OSC1_TUNE,

	CTRL_OSC1_FILT_CUTOFF,
	CTRL_OSC1_FILT_RES,
	CTRL_OSC1_DRIVE,

	CTRL_OSC2_SAW,
	CTRL_OSC2_SQU,
	CTRL_OSC2_NOISE,

	CTRL_OSC2_FILT_CUTOFF,
	CTRL_OSC2_FILT_RES,
	CTRL_OSC2_DRIVE,

	CTRL_SUB,
	CTRL_SUB_NOISE,
	CTRL_SUB_TO_OSC2,

	CTRL_SUB_FILT_CUTOFF,
	CTRL_SUB_FILT_RES,

	CTRL_OSC_FILT_ENV_A,
	CTRL_OSC_FILT_ENV_D,
	CTRL_OSC_FILT_ENV_S,
	CTRL_OSC_FILT_ENV_R,

	CTRL_SUB_FILT_ENV_A,
	CTRL_SUB_FILT_ENV_D,
	CTRL_SUB_FILT_ENV_S,
	CTRL_SUB_FILT_ENV_R,

	CTRL_OSC_AMP_ENV_A,
	CTRL_OSC_AMP_ENV_D,
	CTRL_OSC_AMP_ENV_S,
	CTRL_OSC_AMP_ENV_R,

	CTRL_SUB_AMP_ENV_A,
	CTRL_SUB_AMP_ENV_D,
	CTRL_SUB_AMP_ENV_S,
	CTRL_SUB_AMP_ENV_R,

	CTRL_FX_WETDRY,
	CTRL_FX_VAL1,
	CTRL_FX_VAL2,
	CTRL_FX_VAL3,
	CTRL_FX_VAL4,

} ctrl_enum_t;

void ctrl_apply_delta(ctrl_enum_t ctrl, int8_t delta);
void ctrl_apply_toggle(ctrl_enum_t ctrl, bool changed, bool state);

#endif /* INC_CTRL_H_ */
