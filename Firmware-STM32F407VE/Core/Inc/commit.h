/*
 * commit.h
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#ifndef INC_COMMIT_H_
#define INC_COMMIT_H_

typedef enum uint8_t {
	COMMIT_LED_ROTPIC = 0,

	COMMIT_LED_ADSR_OSC_FILT,
	COMMIT_LED_ADSR_SUB_FILT,
	COMMIT_LED_ADSR_OSC_AMP,
	COMMIT_LED_ADSR_SUB_AMP,

	COMMIT_LED_OSC,
} commit_cycle_t;

void commit_30hz_timer(void);

#endif /* INC_COMMIT_H_ */
