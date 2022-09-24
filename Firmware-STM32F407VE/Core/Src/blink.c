/*
 * blink.c
 *
 *  Created on: Jul 11, 2021
 *      Author: jwilson
 */

#include "blink.h"

#define BLINK_SYSTICKS 250

bool blink = false;

void blink_systick() {
  if (HAL_GetTick() % BLINK_SYSTICKS == 0) {
    blink = !blink;
  }
}
