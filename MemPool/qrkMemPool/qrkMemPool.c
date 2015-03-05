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
#include "support/qrkMemPool_BlkHeader.h"


#if QRK_MPOOL_USE_HEAP_TO_INIT_POOL == 0
static qrkMemUnit_t poolMem[QRK_MPOOL_MEM_SIZE];
#else
static qrkMemUnit_t *poolMem = 0;
#endif



static const qrkMemBlkSize_t mpBlockSizes[][2] = QRK_MPOOL_BLOCKS;
static const int countPools = sizeof(mpBlockSizes) / sizeof(mpBlockSizes[0]);

static int totalBlocks = 0;
static int totalUserBytes = 0;
static int mpoolMemSize = 0;
//==================================================


static void validateConfigValues( void );

//==================================================
void qrkMemPool_Init( void )
{
    int i;

    totalBlocks = 0;
    totalUserBytes = 0;
    for(i=0; i< countPools; i++ )
    {
        totalBlocks += mpBlockSizes[i][1];
        totalUserBytes += mpBlockSizes[i][0] * mpBlockSizes[i][1];
    }
    mpoolMemSize = totalUserBytes + totalBlocks * sizeof(qrkMemPool_BlkHeader);

    validateConfigValues();

#if QRK_MPOOL_USE_HEAP_TO_INIT_POOL != 0
    poolMem = (qrkMemUnit_t*) malloc( mpoolMemSize );
#endif



}


void* qrkMemPool_Alloc(qrkMemBlkSize_t size)
{

    return NULL;
}



int  qrkMemPool_Free( void *mem )
{
    return 0;
}





//==================================================
static void validateConfigValues( void )
{
#if QRK_MPOOL_USE_HEAP_TO_INIT_POOL == 0
    if( totalBlocks != QRK_MPOOL_TOTAL_BLOCKS )
    {
        for(;;);    // Trap here if QRK_MPOOL_TOTAL_BLOCKS is defined wrong.
    }

    if( totalUserBytes != QRK_MPOOL_TOTAL_USER_BYTES )
    {
        for(;;);    // Trap here if QRK_MPOOL_TOTAL_USER_BYTES is defined wrong.
    }

    if( sizeof(qrkMemPool_BlkHeader) != QRK_MPOOL_SIZEOF_BLOCK_HEADER )
    {
        for(;;);    // Trap here if QRK_MPOOL_SIZEOF_BLOCK_HEADER is defined wrong.
    }
    if( mpoolMemSize != QRK_MPOOL_MEM_SIZE )
    {
        for(;;);    // This should never happen.  Bad bug.
    }
#endif
}
