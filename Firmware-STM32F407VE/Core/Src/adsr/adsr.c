/*
 * adsr.c
 *
 *  Created on: Jul 23, 2022
 *      Author: jwilson
 */

#include "adsr.h"
#include <math.h>

typedef struct adsr_s {
  uint8_t attack;
  uint8_t decay;
  uint8_t sustain;
  uint8_t release;
} adsr;

typedef enum {
  ADSR_STATE_ATTACK,
  ADSR_STATE_DECAY,
  ADSR_STATE_SUSTAIN,
  ADSR_STATE_RELEASE,
} adsr_state;

#define ATTACK_MAX_TICKS 2000
#define DECAY_MAX_TICKS 2000
#define RELEASE_MAX_TICKS 2000

#define PARAM_MAX 127
#define CV_MAX 4095

uint32_t _adsr_get_attack_ticks(adsr envelope) {
  return (uint16_t)((double)ATTACK_MAX_TICKS *
                    ((double)envelope.attack / (double)PARAM_MAX));
}

uint32_t _adsr_get_decay_ticks(adsr envelope) {
  return (uint16_t)((double)ATTACK_MAX_TICKS *
                    ((double)envelope.decay / (double)PARAM_MAX));
}

uint32_t _adsr_get_release_ticks(adsr envelope) {
  return (uint16_t)((double)ATTACK_MAX_TICKS *
                    ((double)envelope.release / (double)PARAM_MAX));
}

adsr_state _adsr_get_state(adsr envelope, bool note_on,
                           uint32_t note_time_ticks) {
  if (note_on) {
    // We are in Attack, Decay, Sustain
    uint32_t ticks = HAL_GetTick();
    if (ticks < (note_time_ticks + _adsr_get_attack_ticks(envelope))) {
      return ADSR_STATE_ATTACK;
    } else if (ticks < (note_time_ticks + _adsr_get_attack_ticks(envelope) +
                        _adsr_get_decay_ticks(envelope))) {
      return ADSR_STATE_DECAY;
    } else {
      return ADSR_STATE_SUSTAIN;
    }
  } else {
    // We are in Release
    return ADSR_STATE_RELEASE;
  }
}

uint16_t adsr_control_voltage(adsr envelope, bool note_on,
                              uint32_t note_time_ticks) {
  // note_time_ticks is the time at which the note went
  // either on or off.
  uint16_t ticks = HAL_GetTick();
  uint16_t step = ticks - note_time_ticks;
  switch (_adsr_get_state(envelope, note_on, note_time_ticks)) {
  case ADSR_STATE_ATTACK: {
    uint16_t max_steps = _adsr_get_attack_ticks(envelope);
    uint16_t cv =
        (uint16_t)pow((double)CV_MAX, ((double)(step + 1) / (double)max_steps));
    return cv;
  }
  case ADSR_STATE_DECAY: {
    step -= _adsr_get_attack_ticks(envelope);
    uint16_t max_steps = _adsr_get_decay_ticks(envelope);
    uint16_t cv = (uint16_t)pow(
        (double)CV_MAX, (double)(((max_steps - step) + 1) / (double)max_steps));
    return cv;
  }
  case ADSR_STATE_SUSTAIN: {
    uint16_t cv =
        (double)CV_MAX * ((double)envelope.sustain / (double)PARAM_MAX);
    return cv;
  }
  case ADSR_STATE_RELEASE: {
    uint16_t max_steps = _adsr_get_attack_ticks(envelope);
    uint16_t cv =
        (uint16_t)pow((double)CV_MAX,
                      ((double)(((max_steps - step) + 1) / (double)max_steps)));
    return cv;
  }
  default:
    return 0;
  }
}