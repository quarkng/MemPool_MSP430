/*
 * qrkMemPool_Que.h
 *
 *  Created on: March 5, 2015
 *      Author: Quark
 */

#ifndef QRK_MEMPOOL_QUE_H_
#define QRK_MEMPOOL_QUE_H_

#include "qrkMemPool_Config.h"
#include "qrkMemPool_OrderedLink.h"

typedef struct
{
    qrkMemPool_OrderedLink *head;
    qrkMemPool_OrderedLink *tail;

    qrkMemBlkSize_t             blkSize;
}
qrkMemPool_Que;


void qrkMemPool_QueInit( qrkMemPool_Que *q, qrkMemBlkSize_t blkSizeInit );

void qrkMemPool_Enque( qrkMemPool_Que *q, qrkMemPool_OrderedLink *item );

qrkMemPool_OrderedLink * qrkMemPool_Deque(qrkMemPool_Que *q);

#endif  /* QRK_MEMPOOL_ORDEREDLINK_H_ */
