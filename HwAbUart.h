/*
 * HwAbUart.h
 *	Hardware Abstraction - UART
 *
 *  Created on: Dec 11, 2014
 *      Author: Quark
 */

#ifndef HWABUART_H_
#define HWABUART_H_

#include <stdint.h>
#include <stdbool.h>


#define HwAbUart_TX_BUFFER_SIZE       (256) // Size of RAM to use as transmit buffer
#define HwAbUart_RX_BUFFER_SIZE        (64) // Size of RAM to use as receive buffer

#define HwAbUart_MAX_ALLOWED_STR_LEN  (128) // used in case a string was not properly null terminated

void HwAbUart_Init(void);


void HwAbUart_Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void HwAbUart_SendString(const char *pcBuffer);
bool HwAbUart_IsDoneTransmitting( void );

uint8_t  HwAbUart_GetRxByte(bool *valid);
uint16_t HwAbUart_GetRxContent(uint8_t *buffer, uint16_t buffSize);

#endif /* HWABUART_H_ */
