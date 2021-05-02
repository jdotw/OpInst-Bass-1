/*
 * midi_internal.h
 *
 *  Created on: 28 Aug 2020
 *      Author: jwilson
 */

#ifndef INC_MIDI_INTERNAL_H_
#define INC_MIDI_INTERNAL_H_

// midi.c
midi_state *_midi_state_ptr();

// midi_channel_voice.c
void handle_midi_note_on_msg(uint8_t status_msg, uint8_t *data);
void handle_midi_note_off_msg(uint8_t status_msg, uint8_t *data);
void handle_midi_pitch_bend_msg(uint8_t status_msg, uint8_t *data);

// midi_realtime.c
void handle_midi_clock_msg(uint8_t status_msg);
void handle_midi_start_msg(uint8_t status_msg);

// midi_cc.c
void handle_midi_cc_msg(uint8_t status_msg, uint8_t *data);

#endif /* INC_MIDI_INTERNAL_H_ */
