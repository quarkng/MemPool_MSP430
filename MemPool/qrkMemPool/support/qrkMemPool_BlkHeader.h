/*
 * qrkMemPool_BlkHeader.h
 *
 *  Created on: March 5, 2015
 *      Author: Quark
 */

#ifndef QRK_MEMPOOL_BLKHEADER_H_
#define QRK_MEMPOOL_BLKHEADER_H_

#include "qrkMemPool_Config.h"
#include "qrkMemPool_OrderedLink.h"


typedef struct
{
    qrkMemPool_OrderedLink  link;

    unsigned char           freeQueIdx;   // Index to the free-Que that this block belongs to
    int                     ref;          // Reference counter (count of users actively using this block)
}
qrkMemPool_BlkHeader;


#endif  /* QRK_MEMPOOL_BLKHEADER_H_ */
