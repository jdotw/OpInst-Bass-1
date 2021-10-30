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

#define PRESET_CATALOG_MAX 100

#define PRESET_NAME_MAX_LENGTH 128

typedef struct preset_struct {
  bool present;
  char name[PRESET_NAME_MAX_LENGTH];
} preset_t;

void preset_init(void);
preset_t *preset_get_active(void);
preset_t *preset_get_system(uint8_t index);
preset_t *preset_get_user(uint8_t index);
uint8_t preset_get_active_index(void);

void preset_select_apply_delta(uint8_t delta);

bool preset_save(uint8_t index, char *name);
bool preset_load(bool system, uint8_t index);

#endif /* INC_PRESET_H_ */
