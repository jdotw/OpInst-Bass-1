/*
 * preset.h
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#ifndef INC_PRESET_H_
#define INC_PRESET_H_

#include "ctrl.h"
#include "seq.h"

#define PRESET_MAX_NAME_LENGTH 128

typedef struct
{
  uint8_t index;
  char name[PRESET_MAX_NAME_LENGTH];
} preset_t;

void preset_init(void);
preset_t *preset_get_active(void);
void preset_select_apply_delta(uint8_t delta);
bool preset_save(preset_t preset);
void preset_load(bool system, uint8_t index);

#endif /* INC_PRESET_H_ */
