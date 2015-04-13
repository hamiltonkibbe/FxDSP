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
 * @param length		The number of elements in the circular buffer
 */
CircularBuffer* 
CircularBufferInit(unsigned length);

CircularBufferD*
CircularBufferInitD(unsigned length);

    
/** Free Heap Memory associated with CircularBuffer
*
* @details Frees memory allocated by CircularBufferInit
*/
void
CircularBufferFree(CircularBuffer* cb);
    
void
CircularBufferFreeD(CircularBufferD* cb);

    
/** Write samples to circular buffer
*/
void
CircularBufferWrite(CircularBuffer* cb, const float* src, unsigned n_samples);

void
CircularBufferWriteD(CircularBufferD*   cb,
                     const double*      src,
                     unsigned           n_samples);
    
/** Read samples from circular buffer
 */
void
CircularBufferRead(CircularBuffer* cb, float* dest, unsigned n_samples);
    
void
CircularBufferReadD(CircularBufferD* cb, double* dest, unsigned n_samples);

/** Flush circular buffer
 */
void
CircularBufferFlush(CircularBuffer* cb);

void
CircularBufferFlushD(CircularBufferD* cb);
    
#ifdef __cplusplus
}
#endif

#endif

