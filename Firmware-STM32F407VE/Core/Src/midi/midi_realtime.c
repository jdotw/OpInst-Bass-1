/*
 * midi.c
 *
 *  Created on: Feb 16, 2020
 *      Author: jwilson
 */

#include "main.h"
#include "midi.h"
#include "seq.h"
#include "stdbool.h"

uint8_t clock_count = 0;
#define CLOCK_PER_16TH_NOTE 6

// MARK: - Message Handlers

void handle_midi_clock_msg(uint8_t status_msg) {
  clock_count++;
  if (clock_count == CLOCK_PER_16TH_NOTE) {
    seq_advance_step();
    clock_count = 0;
  }
}

void handle_midi_start_msg(uint8_t status_msg) {
  clock_count = 0;
  seq_start();
}

void handle_midi_stop_msg(uint8_t status_msg) {
  clock_count = 0;
  seq_stop();
}

void handle_midi_continue_msg(uint8_t status_msg) { seq_continue(); }

void handle_midi_song_position_msg(uint8_t status_msg, uint8_t *data) {
  uint16_t beat_count = data[0] | (data[1] << 7);
  seq_set_step(beat_count % 64);
  clock_count = 0;
}
