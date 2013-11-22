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


/** Opaque FtAudioFIRFilter structure */
typedef struct FtAudioCircularBuffer FtAudioCircularBuffer;


/** Create a new FtAudioCircularBuffer
 *
 * @details Allocates memory and returns an initialized FtAudioCircularBuffer.
 *			Play nice and call FtAudioCircularBuffer Free on the filter when you're
 *          done with it.
 *
 * @param length		The number of elements in the circular buffer
 */
FtAudioCircularBuffer* 
FtAudioCircularBufferInit(unsigned length);


/** Free Heap Memory associated with FtAudioCircularBuffer
*
* @details Frees memory allocated by FtAudioCircularBufferInit
*/
void
FtAudioCircularBufferFree(FtAudioCircularBuffer* cb);
    
    
/** Write samples to circular buffer
*/
void
FtAudioCircularBufferWrite(FtAudioCircularBuffer* cb, const float* src, unsigned n_samples);

/** Read samples from circular buffer
 */
void
FtAudioCircularBufferRead(FtAudioCircularBuffer* cb, float* dest, unsigned n_samples);

/** Flush circular buffer
 */
void
FtAudioCircularBufferFlush(FtAudioCircularBuffer* cb);
    
#ifdef __cplusplus
}
#endif

#endif

