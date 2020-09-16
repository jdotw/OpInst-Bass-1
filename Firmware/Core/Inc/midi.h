/*
 * midi.h
 *
 *  Created on: Feb 16, 2020
 *      Author: jwilson
 */

#ifndef INC_MIDI_H_
#define INC_MIDI_H_

#include "stm32g4xx_hal.h"
#include "stdbool.h"

#define MIDI_NOTE_NUMBER_NONE UINT8_MAX
#define MIDI_NOTE_VELOCITY_NONE UINT8_MAX

#define MIDI_PITCH_BEND_MID_POINT 0x2000 // per MIDI spec

typedef struct midi_state_s
{
    // Note State
    bool note_on;
    uint8_t prev_note_number;
    uint8_t prev_note_velocity;
    uint8_t cur_note_number;
    uint8_t cur_note_velocity;
    uint32_t cur_note_time_ticks;	// Time that the current note key was pressed (ticks)

    // Pitch Bend and Mod Wheel
    uint16_t pitch_bend_amount;

} midi_state;

// midi.c
void midi_init();
midi_state midi_get_state();

// midi_rx.c
void midi_rx_set_uart(UART_HandleTypeDef *huart);
void midi_rx_resume();
void midi_rx_dma_rxcomplete_callback();


#endif /* INC_MIDI_H_ */