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

typedef struct
{
  uint8_t index;
  char *name;
  ctrl_t *ctrl;
  ctrl_toggle_t *toggle;
  seq_state_t *seq;
} preset_t;

void preset_init(void);
preset_t *preset_get_active(void);
bool preset_save(preset_t preset);
void preset_load(bool system, uint8_t index);

#endif /* INC_PRESET_H_ */
