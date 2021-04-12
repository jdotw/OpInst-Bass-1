/*
* midi.c
*
*  Created on: Feb 16, 2020
*      Author: jwilson
*/

#include "midi.h"
#include "midi_internal.h"
#include "main.h"
#include "stdbool.h"

// Messsge Type Masks
#define MIDI_STATUS_MASK 0b10000000
#define MIDI_STATUS_MSG 0b10000000
#define MIDI_STATUS_MSG_TYPE_MASK 0b11110000

// Status Message Types
#define MIDI_STATUS_MSG_CLOCK 0b11111000
#define MIDI_STATUS_MSG_START 0b11111010

// Channel Message Types
#define MIDI_CHANNEL_VOICE_NOTE_OFF 0b10000000
#define MIDI_CHANNEL_VOICE_NOTE_ON 0b10010000
#define MIDI_CHANNEL_VOICE_POLYPHONIC_AFTERTOUCH 0b10100000
#define MIDI_CHANNEL_VOICE_CONTROL_CHANGE 0b10110000
#define MIDI_CHANNEL_VOICE_PROG_CHANGE 0b11000000
#define MIDI_CHANNEL_VOICE_CHANNEL_AFTERTOUCH 0b11010000
#define MIDI_CHANNEL_VOICE_PITCH_BEND 0b11100000

// Internal
#define STATUS_MSG_NONE 0

// Channel mask (last 4 bits)

#define MIDI_RECV_TIMEOUT 1000

// Prototypes

uint8_t handle_midi_rx_status(uint8_t status_msg);
uint8_t handle_midi_rx_data(uint8_t status_msg, uint8_t *data, uint8_t recv_count, uint8_t offset);


// MARK: - Error Handler

void handle_midi_rx_fatal_error() {
	// Fatal error.. lock up and flash
	while(1) {
	    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	    HAL_Delay(250);
	    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	    HAL_Delay(250);
	}
}

void handle_midi_rx_unexpected_error() {
	// Unexpected but not fatal
	// NO-OP
}

// MARK: - API

UART_HandleTypeDef *midi_rx_uart = NULL;

uint8_t active_status_msg = STATUS_MSG_NONE;	// The active status byte

#define RX_BUFFER_SIZE 256
uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_recv_count = 0;		// num bytes requested
uint8_t rx_recv_offset = 0;		// rx_buffer buffer offset

void midi_rx_set_uart(UART_HandleTypeDef *huart) {
	midi_rx_uart = huart;
}

void midi_rx_request_bytes(uint8_t count, uint8_t offset) {
  HAL_UART_Receive_DMA(midi_rx_uart, (uint8_t *)rx_buffer + offset, count);
  rx_recv_count = count;
}

void midi_rx_resume() {
	// NOTE: Do not reset the rx_status_buffer because of the
	// concept of 'running status' in the MIDI spec. If the status
	// is the same (e.g. NOTE ON then groups of data bytes for the
	// same status will be sent without an interleaving status msg!!
	midi_rx_request_bytes(1, 0);
}

void midi_rx_dma_rxcomplete_callback() {
	if ((rx_buffer[0] & MIDI_STATUS_MASK) == MIDI_STATUS_MSG) {
		// It's a status message byte
		active_status_msg = handle_midi_rx_status(rx_buffer[0]);
	} else if (active_status_msg != STATUS_MSG_NONE) {
		// It's the data we're waiting on for this message
		// Or, it could even be another set of data bytes for
		// the same message status message (MIDI Running Status spec)
		active_status_msg = handle_midi_rx_data(active_status_msg, rx_buffer, rx_recv_count, rx_recv_offset);
	}

	if (active_status_msg == STATUS_MSG_NONE) {
		// There's no active msg; that is we're not waiting
		// on data bytes for a msg, so poll for the next status msg
		midi_rx_resume();
	}
}

// MARK: - Status Message Handling

uint8_t midi_data_bytes_count_for_status(uint8_t status_msg) {
	switch ((status_msg & MIDI_STATUS_MSG_TYPE_MASK)) {
	case MIDI_CHANNEL_VOICE_NOTE_OFF:
	case MIDI_CHANNEL_VOICE_NOTE_ON:
	case MIDI_CHANNEL_VOICE_POLYPHONIC_AFTERTOUCH:
	case MIDI_CHANNEL_VOICE_CONTROL_CHANGE:
	case MIDI_CHANNEL_VOICE_PITCH_BEND:
		// These all have 2 data bytes
		return 2;
	default:
		return 0;
	}
}

uint8_t handle_midi_rx_status(uint8_t status_msg) {
	// Returns the status_msg if we've requested
	// data-bytes be read from the UART
	uint8_t data_byte_count = 0;
	switch ((status_msg & MIDI_STATUS_MSG_TYPE_MASK)) {
	case MIDI_STATUS_MSG_CLOCK:
		handle_midi_clock_msg(status_msg);
		return STATUS_MSG_NONE;
	case MIDI_STATUS_MSG_START:
		handle_midi_start_msg(status_msg);
		return STATUS_MSG_NONE;
	default:
		data_byte_count = midi_data_bytes_count_for_status(status_msg);
		if (data_byte_count > 0) {
			midi_rx_request_bytes(2, 0);
			return status_msg;
		} else {
			return STATUS_MSG_NONE;
		}
	}
}

// MARK: - Data Handling

uint8_t handle_midi_rx_data(uint8_t status_msg, uint8_t *data, uint8_t recv_count, uint8_t offset) {
	// Returns the status_msg if we've requested
	// more data-bytes be read from the UART
	uint8_t actual_data_byte_count = offset + recv_count;
	uint8_t expected_data_byte_count = midi_data_bytes_count_for_status(status_msg);
	if (actual_data_byte_count == expected_data_byte_count) {
		switch ((status_msg & MIDI_STATUS_MSG_TYPE_MASK)) {
		case MIDI_CHANNEL_VOICE_NOTE_OFF:
			handle_midi_note_off_msg(status_msg, data);
			return STATUS_MSG_NONE;
		case MIDI_CHANNEL_VOICE_NOTE_ON:
			handle_midi_note_on_msg(status_msg, data);
			return STATUS_MSG_NONE;
		case MIDI_CHANNEL_VOICE_PITCH_BEND:
			handle_midi_pitch_bend_msg(status_msg, data);
			return STATUS_MSG_NONE;
		case MIDI_CHANNEL_VOICE_CONTROL_CHANGE:
			handle_midi_cc_msg(status_msg, data);
			return STATUS_MSG_NONE;
		default:
			return STATUS_MSG_NONE;
		}
	} else if (actual_data_byte_count < expected_data_byte_count) {
		// We don't yet have all the expected data bytes.
		// This can happen if Running Status is used to transmit updated
		// data bytes for the same status. We would have received the first byte
		// while expecting a status byte, but instead finding a non-status byte we
		// now need to request the rest of the expected data bytes for the
		// active (last used) status msg
		midi_rx_request_bytes(expected_data_byte_count - actual_data_byte_count, actual_data_byte_count);
		return status_msg;
	} else {
		// Yeah, this shouldn't happen!
		handle_midi_rx_fatal_error();
		return STATUS_MSG_NONE;
	}
}
