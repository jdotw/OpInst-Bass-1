/*
 * mod.c
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */

#include "mod.h"
#include "string.h"

mod_t mod;

mod_t *mod_get() { return &mod; }

void mod_changed_reset() { memset(&mod.changed, 0, sizeof(mod.changed)); }
