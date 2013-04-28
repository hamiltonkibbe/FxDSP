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

#include <string.h>
#include "FtAudioError.h"
#include "FtAudioUtilities.h"

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

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





#ifdef __cplusplus
}
#endif

#endif

