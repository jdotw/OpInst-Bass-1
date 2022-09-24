/*
 * midi.h
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#ifndef INC_MIDI_H_
#define INC_MIDI_H_

#include "stdbool.h"
#include "stm32f4xx_hal.h"

#define MIDI_NOTE_OUT_OF_RANGE 0
#define MIDI_NOTE_MINIMUM 24 // C1
#define MIDI_NOTE_MAXIMUM 72 // C5

#define MIDI_NOTE_A2 45
#define MIDI_NOTE_A3 57
#define MIDI_NOTE_A4 69
#define MIDI_NOTE_A5 81
#define MIDI_NOTE_C2 36
#define MIDI_NOTE_C3 48

#define MIDI_NOTE_NUMBER_NONE UINT8_MAX
#define MIDI_NOTE_VELOCITY_NONE UINT8_MAX

#define MIDI_PITCH_BEND_MID_POINT 0x2000 // per MIDI spec

typedef struct midi_state_s {
  // Pitch Bend and Mod Wheel
  uint16_t pitch_bend_amount;
  uint8_t mod_wheel_amount;

} midi_state;

// midi.c
void midi_init();
midi_state midi_get_state();

// midi_rx.c
void midi_rx_set_uart(UART_HandleTypeDef *huart);
void midi_rx_resume();
void midi_rx_dma_rxcomplete_callback();

#endif /* INC_MIDI_H_ */
