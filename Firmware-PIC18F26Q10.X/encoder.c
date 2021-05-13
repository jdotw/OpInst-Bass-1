#include "mcc_generated_files/mcc.h"
#include "encoder.h"

uint8_t debounce[4];

int8_t enc_direction[4];

int8_t enc_value[4];

void _encoder_clock_isr() {
    debounce[0]++;
    debounce[1]++;
    debounce[2]++;
    debounce[3]++;
}

void encoder_init() {
    debounce[0] = 0;
    debounce[1] = 0;
    debounce[2] = 0;
    debounce[3] = 0;
    enc_direction[0] = 0;
    enc_direction[1] = 0;
    enc_direction[2] = 0;
    enc_direction[3] = 0;
    enc_value[0] = 0;
    enc_value[1] = 0;
    enc_value[2] = 0;
    enc_value[3] = 0;
    TMR0_SetInterruptHandler(_encoder_clock_isr);
}

#define DEBOUNCE_MIN 5

void encoder_clc_isr(uint8_t enc, int8_t dir) {
//    if (enc_direction[enc] != dir && debounce[enc] < DEBOUNCE_MIN) {
//        return;
//    }
    
    enc_value[enc] += dir;
    debounce[enc] = 0;
    
    // Pull interrupt low (active)
    I2CINT_SetLow();
 }

