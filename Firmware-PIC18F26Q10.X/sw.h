/* 
 * File:   sw.h
 * Author: jwilson
 *
 * Created on April 19, 2021, 3:49 PM
 */

#ifndef SW_H
#define	SW_H

#ifdef	__cplusplus
extern "C" {
#endif


extern bool sw_state[2];
extern bool sw_changed[2];

void sw_1_isr(void);
void sw_2_isr(void);
void sw_init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SW_H */

