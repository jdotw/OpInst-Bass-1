/*
 * dpot.c
 *
 *  Created on: 20 Aug 2020
 *      Author: jwilson
 */

#include "main.h"
#include "dpot.h"

void dpot_calibrate_tracking(I2C_HandleTypeDef *i2c) {
	uint8_t dpot_addr = 0b01010000;

	// Set the control register
	uint8_t cr_buf[2];
	cr_buf[0] = 0x02;
	cr_buf[1] = 0x80;
	HAL_StatusTypeDef cr_ret = HAL_I2C_Master_Transmit(i2c, dpot_addr, cr_buf, 2, 1000);
	if (cr_ret == HAL_OK)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	}
	else
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	}

	// Set the value
	uint8_t buf[2];
	buf[0] = 0x00;
	buf[1] = 0;
	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(i2c, dpot_addr, buf, 2, 1000);
	if (ret == HAL_OK)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	}
	else
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	}

}
