/*
 * adsr.h
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */

#ifndef INC_ADSR_H_
#define INC_ADSR_H_

#include "stdbool.h"
#include "stm32f4xx_hal.h"

typedef struct adsr_s {
  uint16_t attack;
  uint16_t decay;
  uint16_t sustain;
  uint16_t release;
} adsr;

uint16_t adsr_control_voltage(adsr envelope, bool note_on,
                              uint32_t note_time_ticks);

#endif /* INC_ADSR_H_ */