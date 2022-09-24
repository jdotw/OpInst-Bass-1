/*
 * ctrl.c
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#include "ctrl.h"
#include "midi.h"
#include "oled.h"
#include "seq.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

ctrl_t ctrl;
ctrl_toggle_t toggle;
bool ctrl_enabled = true;

ctrl_t *ctrl_get_active() { return &ctrl; }

ctrl_toggle_t *ctrl_get_active_toggle() { return &toggle; }

void ctrl_toggle_init() { memset(&toggle, 0, sizeof(toggle)); }

bool ctrl_get_enabled() { return ctrl_enabled; }

void ctrl_set_enabled(bool enabled) { ctrl_enabled = enabled; }

void ctrl_overflow_handler() {
  // TODO: Buzz the haptic
}

double ctrl_double(uint16_t value) { return (double)(value / 4095.0); }

double ctrl_double_inverse(uint16_t value) { return 1.0 - ctrl_double(value); }
