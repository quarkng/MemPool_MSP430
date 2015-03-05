/*
 * qrkMemPool_Que.c
 *
 *  Created on: March 5, 2015
 *      Author: Quark
 */

#include <stddef.h>

#include "qrkMemPool_Que.h"



void qrkMemPool_QueInit( qrkMemPool_Que *q, qrkMemBlkSize_t blkSizeInit )
{
    q->head = q->tail = NULL;
    q->blkSize = blkSizeInit;
}

void qrkMemPool_Enque( qrkMemPool_Que *q, qrkMemPool_OrderedLink *item )
{

}

qrkMemPool_OrderedLink * qrkMemPool_Deque(qrkMemPool_Que *q)
{

    return NULL;
}

