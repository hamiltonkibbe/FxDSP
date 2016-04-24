/*
 *  CircularBuffer.h
 *  c
 *
 *  Created by Hamilton Kibbe on 7/7/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include "Error.h"


#ifdef __cplusplus
extern "C" {
#endif

/** CircularBuffer type */
typedef struct CircularBuffer CircularBuffer;
typedef struct CircularBufferD CircularBufferD;

/** Create a new CircularBuffer
 *
 * @details Allocates memory and returns an initialized CircularBuffer.
 *			Play nice and call CircularBuffer Free on the filter when you're
 *          done with it.
 *
 * @param length		The minimum number of elements in the circular buffer
 */
CircularBuffer*  CircularBufferInit(unsigned length);

CircularBufferD*
CircularBufferInitD(unsigned length);


/** Free Heap Memory associated with CircularBuffer
*
* @details Frees memory allocated by CircularBufferInit
*/
Error_t
CircularBufferFree(CircularBuffer* cb);

Error_t
CircularBufferFreeD(CircularBufferD* cb);


/** Write samples to circular buffer
*/
Error_t
CircularBufferWrite(CircularBuffer* cb, const float* src, unsigned n_samples);

Error_t
CircularBufferWriteD(CircularBufferD*   cb,
                     const double*      src,
                     unsigned           n_samples);


/** Read samples from circular buffer
 */
Error_t
CircularBufferRead(CircularBuffer* cb, float* dest, unsigned n_samples);

Error_t
CircularBufferReadD(CircularBufferD* cb, double* dest, unsigned n_samples);


/** Flush circular buffer
 */
Error_t
CircularBufferFlush(CircularBuffer* cb);

Error_t
CircularBufferFlushD(CircularBufferD* cb);

/** Rewind the read head of the buffer by `n_samples` samples
 */
Error_t
CircularBufferRewind(CircularBuffer* cb, unsigned n_samples);

Error_t
CircularBufferRewindD(CircularBufferD* cb, unsigned n_samples);


/** Return the number of unread samples in the buffer
 */
unsigned
CircularBufferCount(CircularBuffer* cb);

unsigned
CircularBufferCountD(CircularBufferD* cb);


#ifdef __cplusplus
}
#endif

#endif

