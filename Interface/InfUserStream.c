/*
 * InfUserStream.c
 *
 *  Created on: Jan 12, 2015
 *      Author: Quark
 */

#include "InfUserStream.h"

#ifndef NULLPTR
	#define NULLPTR (0)
#endif

//****************************************************************************************
static void dummySendBytes(const uint8_t *pui8Buffer, uint32_t ui32Count);
static void dummySendString(const char *pcBuffer);
static bool dummyIsTxBufferClear( void );

static uint8_t  dummyGetRxByte(bool *valid);
static uint16_t dummyGetRxContent(uint8_t *buffer, uint16_t buffSize);

//****************************************************************************************
void InfUserStream_InitDefaults(InfUserStream_t *context)
{
	context->sendBytes = &dummySendBytes;
	context->sendString = &dummySendString;
	context->isTxBufferClear = &dummyIsTxBufferClear;
	context->getRxByte = &dummyGetRxByte;
	context->getRxContent = &dummyGetRxContent;
}

//****************************************************************************************

void dummySendBytes(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
}

void dummySendString(const char *pcBuffer)
{
}

bool dummyIsTxBufferClear( void )
{
	return true;
}

uint8_t  dummyGetRxByte(bool *valid)
{
	if( valid != NULLPTR )
	{
		*valid = false;
	}
	return 0;
}

uint16_t dummyGetRxContent(uint8_t *buffer, uint16_t buffSize)
{
	return 0;
}
