#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "stdbool.h"
#include "stm32f4xx_hal.h"

void button_led_set_pwm_seq(uint16_t *seqptr, uint16_t *val);
void button_led_set_scale_seq(uint16_t *pwm_seq, uint8_t *scale_seq,
                              uint8_t len);

#endif /* INC_BUTTON_H_ */