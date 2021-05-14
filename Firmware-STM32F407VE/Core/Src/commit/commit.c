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

static uint8_t cycle;

void commit_30hz_timer(void) {
	commit_dac();
	commit_gatetrig();
//	switch(cycle) {
//	case 0:
//		commit_led_rotpic();
//		cycle++;
//		break;
//	case 1:
//		commit_led_adsr();
//		cycle++;
//		break;
//	case 3:
//		commit_led_osc();
//		cycle++;
//		break;
//	default:
//		cycle = 0;
//	}
}
