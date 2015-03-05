/*
 * qrkMemPool_OrderedLink.h
 *
 *  Created on: March 5, 2015
 *      Author: Quark
 */

#ifndef QRK_MEMPOOL_ORDEREDLINK_H_
#define QRK_MEMPOOL_ORDEREDLINK_H_


typedef struct qrkMemPool_OrderedLink_stru
{
    struct qrkMemPool_OrderedLink_stru *next;
    struct qrkMemPool_OrderedLink_stru *prev;
}
qrkMemPool_OrderedLink;



#endif  /* QRK_MEMPOOL_ORDEREDLINK_H_ */
