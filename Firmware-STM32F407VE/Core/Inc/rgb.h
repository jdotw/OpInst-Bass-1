/*
 * rgb.h
 *
 *  Created on: Sep 17, 2022
 *      Author: jwilson
 */

#ifndef INC_RGB_H_
#define INC_RGB_H_

#include "ctrl.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"

typedef struct {
  float r;
  float g;
  float b;
} rgb_t;

typedef struct {
  float L;
  float a;
  float b;
  uint16_t v; // 14-bit control value
} lab_t;

typedef struct {
  double h; // angle in degrees
  double s; // a fraction between 0 and 1
  double v; // a fraction between 0 and 1
} hsv_t;

#define MAX_HSV_V 1.0

// rgb_interpolate.c
lab_t interpolate_lab(lab_t c0, lab_t c1);

// rgb_convert.c
lab_t rgb_to_oklab(rgb_t rgb);
rgb_t oklab_to_rgb(lab_t lab);
hsv_t oklab_to_hsv(lab_t in);
lab_t hsv_to_oklab(hsv_t in);

// rgb_led.c
void rgb_led_set_pwm_lab(lab_t in, uint16_t *pwm, uint8_t len);
void rgb_led_set_scale_animated(uint16_t *pwm, uint8_t *scale, uint8_t len,
                                uint8_t offset, bool invert);
void rgb_led_increment_pattern_step();

lab_t rgb_primitive_lab(uint16_t value, double r, double g, double b);

// rgb_osc1_lab.c
lab_t rgb_osc1_saw_lab(void);
lab_t rgb_osc1_squ_lab(void);
lab_t rgb_osc1_mix_lab(void);
lab_t rgb_osc1_to_2_mix_lab(void);

// rgb_osc2_lab.c
lab_t rgb_osc2_saw_lab(void);
lab_t rgb_osc2_squ_lab(void);
lab_t rgb_osc2_noise_lab(void);
lab_t rgb_osc2_mix_lab(void);
lab_t rgb_osc2_prefilt_lab(void);

// rgb_sub_lab.c
lab_t rgb_sub_squ_lab(void);
lab_t rgb_sub_noise_lab(void);
lab_t rgb_sub_mix_lab(void);
lab_t rgb_sub_to_osc2_mix_lab(void);

// rgb_filt_lab.c
lab_t rgb_osc1_filt_freq_lab(void);
lab_t rgb_osc1_filt_reso_lab(void);
lab_t rgb_osc2_filt_freq_lab(void);
lab_t rgb_osc2_filt_reso_lab(void);
lab_t rgb_sub_filt_freq_lab(void);
lab_t rgb_sub_filt_reso_lab(void);

// rgb_drive_lab.c
lab_t rgb_osc1_drive_lab(void);
lab_t rgb_osc2_drive_lab(void);

// rgb_amp_lab.c
lab_t rgb_osc_amp_out_lab(void);
lab_t rgb_sub_amp_out_lab(void);

// rgb_fx_lab.c
lab_t rgb_fx_wet_lab(void);
lab_t rgb_fx_feedback_lab(void);
lab_t rgb_fx_dry_lab(void);

#endif /* INC_RGB_H_ */