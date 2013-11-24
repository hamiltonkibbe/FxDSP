/*
 *  FtAudioCircularBuffer.h
 *  c
 *
 *  Created by Hamilton Kibbe on 7/7/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#ifndef FTAUDIOCIRCULARBUFFER_H_
#define FTAUDIOCIRCULARBUFFER_H_


#ifdef __cplusplus
extern "C" {
#endif


/** Opaque FTA_FIRFilter structure */
typedef struct FTA_CircularBuffer FTA_CircularBuffer;


/** Create a new FTA_CircularBuffer
 *
 * @details Allocates memory and returns an initialized FTA_CircularBuffer.
 *			Play nice and call FTA_CircularBuffer Free on the filter when you're
 *          done with it.
 *
 * @param length		The number of elements in the circular buffer
 */
FTA_CircularBuffer* 
FTA_CircularBufferInit(unsigned length);


/** Free Heap Memory associated with FTA_CircularBuffer
*
* @details Frees memory allocated by FTA_CircularBufferInit
*/
void
FTA_CircularBufferFree(FTA_CircularBuffer* cb);
    
    
/** Write samples to circular buffer
*/
void
FTA_CircularBufferWrite(FTA_CircularBuffer* cb, const float* src, unsigned n_samples);

/** Read samples from circular buffer
 */
void
FTA_CircularBufferRead(FTA_CircularBuffer* cb, float* dest, unsigned n_samples);

/** Flush circular buffer
 */
void
FTA_CircularBufferFlush(FTA_CircularBuffer* cb);
    
#ifdef __cplusplus
}
#endif

#endif

