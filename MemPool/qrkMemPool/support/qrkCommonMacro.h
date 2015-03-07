/*
 * qrkCommonMacro.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Quark
 */

#ifndef QRKCOMMONMACRO_H_
#define QRKCOMMONMACRO_H_

#define _qrk_ASSERT_ENABLE_ (1) // enable assert()




//===========================================================

#if defined(_qrk_ASSERT_ENABLE_) && ! defined(qrk_ASSERT)
    #define qrk_ASSERT(expr)        \
    {                               \
        if (!(expr)) for(;;);       \
    }
#else
    #define qrk_ASSERT(expr) ((void) 0)
#endif



#endif /* QRKCOMMONMACRO_H_ */
