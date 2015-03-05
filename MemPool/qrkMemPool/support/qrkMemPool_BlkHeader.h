/*
 * qrkMemPool_BlkHeader.h
 *
 *  Created on: March 5, 2015
 *      Author: Quark
 */

#ifndef QRK_MEMPOOL_BLKHEADER_H_
#define QRK_MEMPOOL_BLKHEADER_H_

#include "qrkMemPool_Config.h"


typedef struct
{
    void            *freeQueHead;  // Also used as magic number to ensure integrity
    qrkMemBlkSize_t  size;
    int              ref;          // Reference counter (count of users actively using this block)
    void            *mem;          // Point to the user memory
}
qrkMemPool_BlkHeader;



#endif  /* QRK_MEMPOOL_BLKHEADER_H_ */
