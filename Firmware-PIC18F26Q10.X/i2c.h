/* 
 * File:   i2c.h
 * Author: jwilson
 *
 * Created on April 18, 2021, 9:25 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

void i2c_init(void);

// Register 0
#define LED_1_STATE 1 << 0
#define LED_2_STATE 1 << 1
#define LED_3_STATE 1 << 2
#define LED_4_STATE 1 << 3
#define LED_5_STATE 1 << 4
#define LED_6_STATE 1 << 5

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

