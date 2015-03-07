/*
 * qrkMemPool.c
 *
 *  Created on: Mar 5, 2015
 *      Author: Quark
 */
#include <stddef.h>
#include <stdlib.h>

#include "qrkMemPool.h"
#include "qrkMemPool_Config.h"

#include "support/qrkCommonMacro.h"
#include "support/qrkMemPool_BlkHeader.h"
#include "support/qrkMemPool_Que.h"

//==================================================
#if QRK_MPOOL_USE_HEAP_TO_INIT_POOL == 0
static qrkMemUnit_t poolMem[QRK_MPOOL_MEM_SIZE];
#else
static qrkMemUnit_t *poolMem = NULL;
#endif

static const qrkMemBlkSize_t mpBlockSizes[][2] = QRK_MPOOL_BLOCKS;  // Defines size-of-block and number-of-blocks in each pool
static const int countPools = sizeof(mpBlockSizes) / sizeof(mpBlockSizes[0]);

static int totalBlocks = 0;
static int totalUserBytes = 0;
static int mpoolMemSize = 0;

static qrkMemPool_Que *poolQues; // Each que represents a pool.
//==================================================
// Static function prototypes

static void validateConfigValues( void );
static int InitBlocksToQue( qrkMemUnit_t *mem, int queIndex );

//==================================================
void qrkMemPool_Init( void )
{
    int i;
    int totalUsed = 0;

    totalBlocks = 0;
    totalUserBytes = 0;
    for(i=0; i< countPools; i++ )
    {
        totalBlocks += mpBlockSizes[i][1];
        totalUserBytes += mpBlockSizes[i][0] * mpBlockSizes[i][1];
    }
    mpoolMemSize = totalUserBytes + totalBlocks * sizeof(qrkMemPool_BlkHeader) + countPools * sizeof(qrkMemPool_Que) ;

    validateConfigValues();

#if QRK_MPOOL_USE_HEAP_TO_INIT_POOL != 0
    if( poolMem == NULL) {  poolMem = (qrkMemUnit_t*) malloc( mpoolMemSize );  }
    qrk_ASSERT( poolMem != NULL );
#endif

    // Now build the pool ques
    qrkMemUnit_t *pm = poolMem;
    poolQues = (qrkMemPool_Que*) pm ;
    pm += sizeof(qrkMemPool_Que) * countPools;

    for( i=0; i < countPools; i++ )
    {
        pm += InitBlocksToQue( pm, i );
    }

    totalUsed = pm - poolMem;
    qrk_ASSERT(totalUsed == mpoolMemSize);  // test under/over allocated memory
}

// Returns number of qrkMemUnit_t used
static int InitBlocksToQue( qrkMemUnit_t *mem, int queIndex )
{
    int memUsed = 0;
    int blk;

    qrkMemPool_Que *q = &poolQues[queIndex];
    qrkMemBlkSize_t blkSize = mpBlockSizes[queIndex][0];
    qrkMemBlkSize_t blksPerPool = mpBlockSizes[queIndex][1];

    qrkMemPool_QueInit( q, blkSize );

    for( blk = 0; blk < blksPerPool; blk++ )
    {
        qrkMemPool_BlkHeader *hdr = (qrkMemPool_BlkHeader *) &mem[memUsed];
        memUsed += sizeof( qrkMemPool_BlkHeader );

        hdr->freeQueIdx = queIndex;
        hdr->ref = 0;

        qrkMemPool_Enque( q, &(hdr->link) );
        memUsed += blkSize;
    }

    return memUsed;
}




void* qrkMemPool_Alloc(qrkMemBlkSize_t size)
{
    void* mem = NULL;
    int i;

    for( i = 0; i < countPools; i++ )
    {
        if( size <= mpBlockSizes[i][0] )
        {
            qrkMemPool_BlkHeader *bh;
            bh = (qrkMemPool_BlkHeader *) qrkMemPool_Deque(&poolQues[i]);
            if( bh != NULL )
            {
                bh->ref++;
                mem = (void*) &bh[1];
                break;
            }
        }
    }

    return mem;
}



int  qrkMemPool_Free( void *mem )
{
    qrkMemPool_BlkHeader *bh;

    if( mem == NULL )
    {   // Ignore if null was passed in
        return 1;
    }
    if( (mem < poolMem) || (mem >= &(poolMem[mpoolMemSize]) ) )
    {   // Memory does not belong in memory pool!
        return 2;
    }

    bh = (qrkMemPool_BlkHeader*) mem;
    bh--; // now points to the block header

    bh->ref--;

    if( bh->ref == 0 )
    {
        qrkMemPool_Enque( &poolQues[bh->freeQueIdx], &bh->link );
    }

    return 0;
}


qrkMemBlkSize_t  qrkMemPool_GetBlockSize( void *mem )
{
    int i;
    qrkMemPool_BlkHeader *bh;
    qrkMemBlkSize_t size;
    qrkMemBlkSize_t sizeRaw;

    if( mem == NULL )
    {   // Ignore if null was passed in
        return 0;
    }

    bh = (qrkMemPool_BlkHeader*) mem;
    bh--; // now points to the block header


    sizeRaw = poolQues[ bh->freeQueIdx ].blkSize;

    // Make sure value is one of the valid sizes.
    // Otherwise, zero is returned.
    size = 0;
    for( i = 0; i < countPools; i++ )
    {
        if( sizeRaw == mpBlockSizes[i][0] )
        {
            size = sizeRaw;
            break;
        }
    }

    return size;
}

//==================================================
static void validateConfigValues( void )
{
#if QRK_MPOOL_USE_HEAP_TO_INIT_POOL == 0

    qrk_ASSERT( totalBlocks == QRK_MPOOL_TOTAL_BLOCKS );    // Trap here if QRK_MPOOL_TOTAL_BLOCKS is defined wrong.

    qrk_ASSERT( totalUserBytes == QRK_MPOOL_TOTAL_USER_BYTES );  // Trap here if QRK_MPOOL_TOTAL_USER_BYTES is defined wrong.

    qrk_ASSERT( sizeof(qrkMemPool_BlkHeader) == QRK_MPOOL_SIZEOF_BLOCK_HEADER ); // Trap here if QRK_MPOOL_SIZEOF_BLOCK_HEADER is defined wrong.

    qrk_ASSERT( sizeof(qrkMemPool_Que) == QRK_MPOOL_SIZEOF_QUE_STRUCT );    // Trap here if QRK_MPOOL_SIZEOF_QUE_STRUCT is defined wrong.

    qrk_ASSERT( countPools == QRK_MPOOL_COUNT_POOLS );  // Trap here if QRK_MPOOL_COUNT_POOLS is defined wrong.

    qrk_ASSERT( mpoolMemSize == QRK_MPOOL_MEM_SIZE ); // This should never happen.  Bad bug.
#endif
}
