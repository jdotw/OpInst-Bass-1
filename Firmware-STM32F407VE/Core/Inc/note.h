/*
 * note.h
 *
 *  Created on: 1 May 2021
 *      Author: jwilson
 */

#ifndef INC_NOTE_H_
#define INC_NOTE_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

struct note_value_struct {
  uint8_t note_number;
  bool note_on;
};

struct note_trig_struct {
  bool ping_trigger;
  uint32_t triggered_at;
};

typedef struct {
  struct note_trig_struct trig;
  struct note_value_struct value;
  uint32_t event_ticks;
} note_t;

note_t *note_get_active(void);

void note_set_on(uint8_t note_number);
void note_set_off(void);

#endif /* INC_NOTE_H_ */
