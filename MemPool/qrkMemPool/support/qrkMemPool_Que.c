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
    qrkMemPool_OrderedLink *prevTail;

    __QRK_CRITICAL_SECTION_BEGIN__

    prevTail = q->tail;
    q->tail = item;

    item->next = NULL;
    item->prev = prevTail;

    q->tail = item;
    if( prevTail == NULL )
    {
        q->head = item;
    }
    else
    {
        prevTail->next = item;
    }

    __QRK_CRITICAL_SECTION_END__
}

qrkMemPool_OrderedLink * qrkMemPool_Deque(qrkMemPool_Que *q)
{
    qrkMemPool_OrderedLink *item = NULL;

    __QRK_CRITICAL_SECTION_BEGIN__

    if( q-> head != NULL )
    {
        item = q->head;
        q->head = item->next;

        if( q->head != NULL )
        {
            q->head->prev = NULL;
        }
        else
        {   // que is empty
            q->tail = NULL;
        }
    }

    __QRK_CRITICAL_SECTION_END__

    if( item != NULL )
    {
        item->next = item->prev = NULL;
    }

    return item;
}

