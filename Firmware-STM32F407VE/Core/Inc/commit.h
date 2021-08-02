/*
 * commit.h
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#ifndef INC_COMMIT_H_
#define INC_COMMIT_H_

#include "ctrl.h"
#include "note.h"
#include "seq.h"
#include "mod.h"

typedef enum uint8_t {
  COMMIT_INIT = 0,

	COMMIT_LED_ROTPIC,

	COMMIT_LED_ADSR_OSC_FILT_A,
	COMMIT_LED_ADSR_OSC_FILT_D,
	COMMIT_LED_ADSR_OSC_FILT_S,
	COMMIT_LED_ADSR_OSC_FILT_R,
	COMMIT_LED_ADSR_SUB_FILT_A,
	COMMIT_LED_ADSR_SUB_FILT_D,
	COMMIT_LED_ADSR_SUB_FILT_S,
	COMMIT_LED_ADSR_SUB_FILT_R,
	COMMIT_LED_ADSR_OSC_AMP_A,
	COMMIT_LED_ADSR_OSC_AMP_D,
	COMMIT_LED_ADSR_OSC_AMP_S,
	COMMIT_LED_ADSR_OSC_AMP_R,
	COMMIT_LED_ADSR_SUB_AMP_A,
	COMMIT_LED_ADSR_SUB_AMP_D,
	COMMIT_LED_ADSR_SUB_AMP_S,
	COMMIT_LED_ADSR_SUB_AMP_R,

	COMMIT_LED_OSC1_SAW,
	COMMIT_LED_OSC1_SQU,
	COMMIT_LED_OSC1_MIX,
  COMMIT_LED_OSC1_FILT_FREQ,
  COMMIT_LED_OSC1_FILT_RESO,
	COMMIT_LED_OSC1_DRIVE,
	COMMIT_LED_OSC1_TO_OSC2,
	COMMIT_LED_OSC2_SAW,
	COMMIT_LED_OSC2_SQU,
	COMMIT_LED_OSC2_NOISE,
	COMMIT_LED_OSC2_ONLY,
	COMMIT_LED_OSC2_PREFILT,
  COMMIT_LED_OSC2_FILT_FREQ,
  COMMIT_LED_OSC2_FILT_RESO,
	COMMIT_LED_OSC2_DRIVE,
	COMMIT_LED_OSC_AMP_OUT,
	COMMIT_LED_SUB_AMP_OUT,
	COMMIT_LED_SUB_SQU,
	COMMIT_LED_SUB_NOISE,
	COMMIT_LED_SUB_MIX,
	COMMIT_LED_SUB_TO_OSC2,
  COMMIT_LED_SUB_FILT_FREQ,
  COMMIT_LED_SUB_FILT_RESO,
	COMMIT_LED_FX_DRY,
	COMMIT_LED_FX_WET,
	COMMIT_LED_FX_FEEDBACK,

	COMMIT_LED_BUTTON_STEP1TO12,
	COMMIT_LED_BUTTON_STEP13TO16,
	COMMIT_LED_BUTTON_SHIFTPAGE,
	COMMIT_LED_BUTTON_START,

	// MUST Be last
	COMMIT_RESET

} commit_cycle_t;

extern ctrl_t commit_ctrl;
extern ctrl_toggle_t commit_ctrl_toggle;
extern note_value_t commit_note_value;
extern note_changed_t commit_note_changed;
extern seq_state_t commit_seq_state;
extern seq_changed_t commit_seq_changed;
extern mod_state_t commit_mod_state;

void commit_30hz_timer(void);

// Internal methods for RGB LED commit

typedef struct {
  uint16_t r;     // 14bit 0-4095
  uint16_t g;     // 14bit 0-4095
  uint16_t b;     // 14bit 0-4095
} rgb_14;

typedef struct {
  double h;       // angle in degrees
  double s;       // a fraction between 0 and 1
  double v;       // a fraction between 0 and 1
} hsv;

uint8_t _max(uint16_t a, uint16_t b);
uint8_t _min(uint16_t a, uint16_t b);
uint8_t _12_to_8(uint16_t a);
void _rgb_copy(uint16_t *dst, uint16_t *src);
rgb_14 _hsv_to_rgb(hsv hsv);
hsv _rgb_to_hsv(rgb_14 in14bit);
hsv _interpolate_hsv(hsv in1, hsv in2);
rgb_14 _interpolate_rgb(rgb_14 in1, rgb_14 in2);
double _interpolate_h(hsv a, hsv b);
double _interpolate_s(hsv in1, hsv in2);
double _interpolate_v(hsv in1, hsv in2);
void _set_pwm_seq(uint16_t *rgb, uint8_t *pwm_seq, uint8_t len);
void _set_pwm_seq_hsv(hsv in, uint8_t *pwm_seq, uint8_t len);
void _set_scale_seq(uint8_t *pwm_seq, uint8_t *scale_seq, uint8_t len);
void _set_scale_seq_animated(uint8_t *pwm_seq, uint8_t *scale_seq, uint8_t len, uint8_t offset, bool invert);
void increment_pattern_step();

#endif /* INC_COMMIT_H_ */
