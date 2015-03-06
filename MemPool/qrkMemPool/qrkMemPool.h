/*
 * qrkMemPool.h
 *
 *  Created on: March 5, 2015
 *      Author: Quark
 */

#ifndef QRK_MEMPOOL_H_
#define QRK_MEMPOOL_H_

#include "qrkMemPool_Config.h"

// This must be called once before any use of the memory pool.
void qrkMemPool_Init( void );


void* qrkMemPool_Alloc(qrkMemBlkSize_t size);

int   qrkMemPool_Free( void *mem );


qrkMemBlkSize_t  qrkMemPool_GetBlockSize( void *mem );

#endif  /* QRK_MEMPOOL_H_ */
