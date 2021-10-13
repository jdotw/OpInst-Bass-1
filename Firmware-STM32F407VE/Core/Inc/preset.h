/*
 * preset.h
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#ifndef INC_PRESET_H_
#define INC_PRESET_H_

#include "ctrl.h"

typedef struct
{
  uint8_t index;
  char *name;
  ctrl_t *ctrl;
} preset_t;

void preset_init(void);
preset_t *preset_get_active(void);
bool preset_save(preset_t preset);

#endif /* INC_PRESET_H_ */
