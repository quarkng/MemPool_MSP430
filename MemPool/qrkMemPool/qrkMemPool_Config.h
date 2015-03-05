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
    {    8,    16  },       \
    {   32,     8  },       \
    {  128,     2  },       \
}
//... as an array of  { Bytes_per_block,  Number_of_blocks }


// Make this non-zero to allow code to allocate space for the pool with a one-time malloc call.
// Otherwise, you must define QRK_MPOOL_TOTAL_BLOCKS and QRK_MPOOL_TOTAL_USER_BYTES correctly.
#define QRK_MPOOL_USE_HEAP_TO_INIT_POOL     0

#if QRK_MPOOL_USE_HEAP_TO_INIT_POOL == 0
    // Define the size needed for the mem pool
    #define QRK_MPOOL_TOTAL_BLOCKS          (16+8+2)            // !!! Must match QRK_MPOOL_BLOCKS !!!
    #define QRK_MPOOL_TOTAL_USER_BYTES      (8*16+32*8+128*2)   // !!! Must match QRK_MPOOL_BLOCKS !!!

    #define QRK_MPOOL_SIZEOF_BLOCK_HEADER   (12)
    #define QRK_MPOOL_MEM_SIZE              ( QRK_MPOOL_TOTAL_USER_BYTES + QRK_MPOOL_TOTAL_BLOCKS * QRK_MPOOL_SIZEOF_BLOCK_HEADER )
#endif



#ifndef QRK_CRITICAL_SECTION_BEGIN
    // Define macros for the Mem Pool code to enable/disable one level of interrupts.
    // Check your compiler manual for intrinsic functions.
    #define QRK_CRITICAL_SECTION_BEGIN                              \
        {                                                           \
            unsigned short istate = __get_interrupt_state(void);    \
            __disable_interrupts();

    #define QRK_CRITICAL_SECTION_END            \
            __set_interrupt_state(istate);      \
        }
#endif

#endif /* QRK_MEMPOOL_CONFIG_H_ */
