/*
 * seq.c
 *
 *  Created on: 10 Jul 2021
 *      Author: jwilson
 */

#include "seq.h"
#include "string.h"

seq_state_t seq_state;

void seq_changed_reset() {
  memset(&seq_state.button_changed, 0, 16);
}
