/*
 * commit.c
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "ctrl.h"
#include "main.h"
#include "i2c.h"
#include "dac7678.h"
#include "osc.h"
#include "note.h"
#include <math.h>

void commit_dac(void);
void commit_gatetrig(void);
void commit_led_rotpic(void);
void commit_led_adsr(void);
void commit_led_osc(void);

void commit_30hz_timer(void) {
	commit_dac();
	commit_gatetrig();
	commit_led_rotpic();
	commit_led_adsr();
	commit_led_osc();
}
