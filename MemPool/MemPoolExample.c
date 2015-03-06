/*
 * MemPoolExample.c
 *
 *  Created on: March 5, 2015
 *      Author: Quark
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "MemPoolExample.h"

#include "qrkMemPool.h"

#define totalBlocks 26

static void Fill( qrkMemUnit_t val, void *mem );

void MemPoolExample(const InfUserStream_t *userStream)
{
    qrkMemPool_Init();

    int i, k;
    void* m[totalBlocks];


    for( k = 0; k < 64; k++ )
    {

        for( i=0; i < totalBlocks; i++ )
        {
            m[i] = NULL;

            while( m[i] == NULL )
            {
                m[i] = qrkMemPool_Alloc(8);
            }

            Fill( (i*k) ^ 0x5A, m[i] ); // Fill it up with value
        }

        __no_operation(); // A place for breakpoint;

        for( i=0; i<totalBlocks; i++ )
        {
            qrkMemPool_Free( m[i] );
        }

        __no_operation(); // A place for breakpoint;
    }

    __no_operation(); // A place for breakpoint;
}

static void Fill( qrkMemUnit_t val, void *mem )
{
    qrkMemBlkSize_t size;

    size = qrkMemPool_GetBlockSize( mem );
    memset( mem, val, size );
}

