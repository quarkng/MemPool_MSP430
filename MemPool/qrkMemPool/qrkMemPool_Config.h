/*
 * qrkMemPool_Config.h
 *
 *  Created on: March 5, 2015
 *      Author: Quark
 */

#ifndef QRK_MEMPOOL_CONFIG_H_
#define QRK_MEMPOOL_CONFIG_H_

#ifndef DEFINED_QRKMEMUNIT
#define  DEFINED_QRKMEMUNIT
    // Define the standard unit of memory.  Note that sizeof(qrkMemUnit_t) should return 1.
    typedef  unsigned char  qrkMemUnit_t;

    // Unit to use to define the size of a memory block.
    typedef unsigned int qrkMemBlkSize_t;
#endif

// Define the block sizes and number of blocks in the pool...
#define QRK_MPOOL_BLOCKS    \
{                           \
    {   16,    16  },       \
    {   64,     8  },       \
    {   256,    2  },       \
}
//... as an array of  { Bytes_per_block,  Number_of_blocks }


// Make this non-zero to allow code to allocate space for the pool with a one-time malloc call.
// Otherwise, you must define QRK_MPOOL_TOTAL_BLOCKS and QRK_MPOOL_TOTAL_USER_BYTES correctly for static allocation.
#define QRK_MPOOL_USE_HEAP_TO_INIT_POOL     0

#if QRK_MPOOL_USE_HEAP_TO_INIT_POOL == 0
    // Define the size needed for the mem pool
    #define QRK_MPOOL_COUNT_POOLS           (3)                     // !!! Must match QRK_MPOOL_BLOCKS !!!
    #define QRK_MPOOL_TOTAL_BLOCKS          (16+8+2)                // !!! Must match QRK_MPOOL_BLOCKS !!!
    #define QRK_MPOOL_TOTAL_USER_BYTES      (16*16 + 64*8 + 256*2)  // !!! Must match QRK_MPOOL_BLOCKS !!!

    #define QRK_MPOOL_SIZEOF_BLOCK_HEADER   (12)
    #define QRK_MPOOL_SIZEOF_QUE_STRUCT     (10)
    #define QRK_MPOOL_MEM_SIZE              ( QRK_MPOOL_TOTAL_USER_BYTES + QRK_MPOOL_TOTAL_BLOCKS * QRK_MPOOL_SIZEOF_BLOCK_HEADER + QRK_MPOOL_COUNT_POOLS * QRK_MPOOL_SIZEOF_QUE_STRUCT )
#endif



#ifndef __QRK_CRITICAL_SECTION_BEGIN__
    // Define macros for the Mem Pool code to enable/disable one level of interrupts.
    // Check your compiler manual for intrinsic functions.
    #define __QRK_CRITICAL_SECTION_BEGIN__                          \
        {                                                           \
            unsigned short istate = __get_interrupt_state();        \
            __disable_interrupt();

    #define __QRK_CRITICAL_SECTION_END__        \
            __set_interrupt_state(istate);      \
        }

    // Place any #include neccessary to use the intrisic functions here.
    #include "intrinsics.h"
#endif




#endif /* QRK_MEMPOOL_CONFIG_H_ */
