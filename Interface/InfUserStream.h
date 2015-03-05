/*
 * InfUserStream.h
 *
 *  Created on: Jan 12, 2015
 *      Author: Quark
 */

#ifndef INFUSERSTREAM_H_
#define INFUSERSTREAM_H_

#include <stdint.h>
#include <stdbool.h>




typedef struct
{
	void (* sendBytes )(const uint8_t *pui8Buffer, uint32_t ui32Count);	// send a count of bytes to UI
	void (* sendString )(const char *pcBuffer);
	bool (* isTxBufferClear )( void );

	uint8_t  (* getRxByte )(bool *valid);
	uint16_t (* getRxContent )(uint8_t *buffer, uint16_t buffSize);

} InfUserStream_t;


void InfUserStream_InitDefaults(InfUserStream_t *context);

#endif /* INFUSERSTREAM_H_ */
