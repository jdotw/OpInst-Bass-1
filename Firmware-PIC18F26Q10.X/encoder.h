/* 
 * File:   encoder.h
 * Author: jwilson
 *
 * Created on April 19, 2021, 9:42 AM
 */

#ifndef ENCODER_H
#define	ENCODER_H

#ifdef	__cplusplus
extern "C" {
#endif

void encoder_init(void);
void encoder_clc_isr(uint8_t enc, int8_t dir);
int8_t* encoder_get_values(void);

extern int8_t enc_value[4];

#ifdef	__cplusplus
}
#endif

#endif	/* ENCODER_H */

