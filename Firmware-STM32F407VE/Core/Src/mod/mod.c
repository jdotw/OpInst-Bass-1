/*
 * mod.c
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */


#include "mod.h"
#include "string.h"

mod_state_t mod_state;

void mod_state_changed_reset() {
  memset(&mod_state.button_changed, 0, sizeof(mod_button_changed_t));
}
