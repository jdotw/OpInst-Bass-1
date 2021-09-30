/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : gatt_db.h
* Author             : RF Application Team - AMG
* Version            : V1.0.0
* Date               : 09-Oct-2019
* Description        : Header file which contains variable used for application.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GATT_DB_H
#define GATT_DB_H

#include "hci.h"

/**
 * This sample application uses a char value length greater than 20 bytes
 * (tipically used).
 * For using greater values for CHAR_VALUE_LENGTH (max 512) and
 * CLIENT_MAX_MTU_SIZE (max 247):
 * - increase both the two #define below to their max values
 * - increase both the HCI_READ_PACKET_SIZE and HCI_MAX_PAYLOAD_SIZE to 256
 *   (file bluenrg_conf.h)
 * - increase the CSTACK in the IDE project options (0xC00 is enough)
*/
#define CHAR_VALUE_LENGTH 63
#define CLIENT_MAX_MTU_SIZE 158

tBleStatus Add_Sample_Service(void);
void APP_UserEvtRx(void *pData);

extern uint16_t sampleServHandle, TXCharHandle, RXCharHandle;

#endif /* GATT_DB_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

