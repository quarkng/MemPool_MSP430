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
#include "support/qrkCommonMacro.h"

#define TOTAL_BLOCKS 26

static void Fill( qrkMemUnit_t val, void *mem );



// Allocate and free memory blocks from the pool.
// Write to the entire block each time memory is allocated.
void MemPoolExample(const InfUserStream_t *userStream)
{
    int i, k;
    void* m[TOTAL_BLOCKS];

    qrkMemPool_Init();

    qrk_ASSERT( qrkMemPool_Free( &i ) != 0 ); // Test for pool's self-check.

    for( k = 0; k < 64; k++ ) // Do allocate/free sequence 64 times
    {

        for( i=0; i < TOTAL_BLOCKS; i++ ) // Allocate all available blocks
        {
            m[i] = qrkMemPool_Alloc(8);
            qrk_ASSERT(m[i] != NULL);

            Fill( (i*(k+1)) ^ 0x5A, m[i] ); // Fill it up with value
        }

        qrk_ASSERT( qrkMemPool_Alloc(8) == NULL ); // Pool should be empty since we just allocated all blocks.

        __no_operation(); // A place for breakpoint;

        for( i=0; i<TOTAL_BLOCKS; i++ ) // Free all the blocks
        {
            int ret = qrkMemPool_Free( m[i] );
            qrk_ASSERT( ret == 0 );
        }

        __no_operation(); // A place for breakpoint;
    }

    __no_operation(); // A place for breakpoint;
}


// Helper function to fill the entire block with a value.
static void Fill( qrkMemUnit_t val, void *mem )
{
    qrkMemBlkSize_t size;

    size = qrkMemPool_GetBlockSize( mem );
    memset( mem, val, size );
}

