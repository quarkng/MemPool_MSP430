/*
 * HwAbUart.c
 *
 *  Created on: Dec 11, 2014
 *      Author: Quark
 */

#include "HwAbUart.h"

#include "msp430fr5969.h"


typedef struct
{
	uint8_t *buf;
	uint16_t size;
	uint16_t head;
	uint16_t tail;
} SingleBuffer_t;

static struct
{
	SingleBuffer_t tx;
	SingleBuffer_t rx;
} uartBuffer;



static void     EnsureTransmitting( void );
static bool    PushToBuffer( SingleBuffer_t *b, char c );
static uint8_t GetFromBuffer( SingleBuffer_t *b, bool *isValid );


//*****************************************************************************
//
// Initialize abstraction code.  Must be called before any use of other code
// in this module.
//
//*****************************************************************************
void HwAbUart_Init(void)
{
	static uint8_t txBuf[HwAbUart_TX_BUFFER_SIZE];
	static uint8_t rxBuf[HwAbUart_RX_BUFFER_SIZE];

	uartBuffer.tx.buf = txBuf;
	uartBuffer.tx.size = HwAbUart_TX_BUFFER_SIZE;
	uartBuffer.tx.head = 0;
	uartBuffer.tx.tail = 0;

	uartBuffer.rx.buf = rxBuf;
	uartBuffer.rx.size = HwAbUart_RX_BUFFER_SIZE;
	uartBuffer.rx.head = 0;
	uartBuffer.rx.tail = 0;
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void HwAbUart_Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
    	PushToBuffer( &(uartBuffer.tx), *pui8Buffer++ );
    }

    EnsureTransmitting();
}

//*****************************************************************************
void HwAbUart_SendString(const char *pcBuffer)
{
	int i = 0;

	while( ( *pcBuffer != 0 ) && (i < HwAbUart_MAX_ALLOWED_STR_LEN) )
	{
		PushToBuffer( &(uartBuffer.tx), *pcBuffer++ );
		i++;
	}

	EnsureTransmitting();
}
//*****************************************************************************
bool HwAbUart_IsDoneTransmitting( void )
{
	return ( !(UCA0IE & UCTXIE) );
}

//*****************************************************************************
uint8_t  HwAbUart_GetRxByte(bool *valid)
{
	bool v;
	uint8_t c = GetFromBuffer( &(uartBuffer.rx), &v );
	if( valid != 0 )
	{
		*valid = v;
	}
	return c;
}

//*****************************************************************************
uint16_t HwAbUart_GetRxContent(uint8_t *buffer, uint16_t buffSize)
{
	uint16_t size;
	bool valid;
	uint8_t c;

	for( size = 0; size < buffSize; size++ )
	{
		c = GetFromBuffer( &(uartBuffer.rx), &valid );
		if( valid )
		{
			buffer[size] = c;
		}
		else
		{
			break;
		}
	}
	return size;
}


//*****************************************************************************
static bool PushToBuffer( SingleBuffer_t *b, char c )
{
	bool overflow = false;
	uint16_t nextTail = b->tail + 1;
	if( nextTail >= b->size )
	{
		nextTail = 0;
	}


	if( nextTail == b->head )
	{	// Buffer is full.  Overwrite last character.
		nextTail = b->tail;
		overflow = true;
	}

	b->buf[ b->tail ] = c;
	b->tail = nextTail;

	return overflow;
}
//*****************************************************************************
static uint8_t GetFromBuffer( SingleBuffer_t *b, bool *isValid )
{
	char retVal = 0;
	bool valid = false;

	if( b->head != b->tail )
	{
		uint16_t nextHead;

		valid = true;
		retVal = b->buf[b->head];
		nextHead = (b->head + 1);
		if( nextHead >= b->size )
		{
			nextHead = 0;
		}
		b->head = nextHead;
	}

	if( isValid != 0 )
	{
		*isValid = valid;
	}
	return retVal;
}


//****************************************************************************************
static void EnsureTransmitting( void )
{
	if( !(UCA0IE & UCTXIE) )
	{
		UCA0IFG |= UCTXIFG;
		UCA0IE  |= UCTXIE; // Transmit interrupt enable
	}

}

//****************************************************************************************
/*
 * USCI_A0 Interrupt Service Routine that receives PC GUI's commands
 */
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
	{
		case USCI_NONE:
		{
		} break;
		case USCI_UART_UCRXIFG:
		{
			PushToBuffer( &(uartBuffer.rx), UCA0RXBUF_L );
		} break;
		case USCI_UART_UCTXIFG:
		{
			bool valid;
			char c = GetFromBuffer( &(uartBuffer.tx), &valid );
			if( valid )
			{
				UCA0TXBUF = c;
			}
			else
			{	// Buffer was empty
				UCA0IE  &= ~UCTXIE; // Transmit interrupt disabled
			}

		} break;
		case USCI_UART_UCSTTIFG:
		{

		} break;
		case USCI_UART_UCTXCPTIFG:
		{

		} break;
	}
}


