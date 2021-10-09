/*
 * preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include "main.h"
#include "preset.h"

typedef struct {
  preset_t *active;
} preset_state_t;

preset_t default_preset;

preset_state_t state;

void preset_init(void) {
  default_preset.name = "Default";
  state.active = &default_preset;
}

preset_t* preset_get_active(void) {
  return state.active;
}
