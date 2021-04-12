#include "main.h"
#include <stdbool.h>

static bool pulse_trigger = false;

void set_pulse_trigger(bool value) {
	pulse_trigger = true;
}

void pulse_trigger_if_needed() {
	if (!pulse_trigger) return;
	pulse_trigger = false;
	HAL_GPIO_WritePin(NOTE_TRIGGER_GPIO_Port, NOTE_TRIGGER_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(NOTE_TRIGGER_GPIO_Port, NOTE_TRIGGER_Pin, GPIO_PIN_RESET);
}
