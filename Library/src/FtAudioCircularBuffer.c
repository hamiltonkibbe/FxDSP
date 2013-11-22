/*
 *  FtAudioCircularBuffer.c
 *  c
 *
 *  Created by Hamilton Kibbe on 7/7/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioCircularBuffer.h"
#include "FtAudioDsp.h"
#include "FtAudioUtilities.h"
#include <stdlib.h>
#include <string.h>


/* FtAudioCircularBuffer ******************************************************/
struct FtAudioCircularBuffer
{
    unsigned    length;
    unsigned    wrap;
    float* buffer;
    unsigned read_index;
    unsigned write_index;
};

/* FtAudioCircularBufferInit **************************************************/
FtAudioCircularBuffer* 
FtAudioCircularBufferInit(unsigned length)
{
    FtAudioCircularBuffer* cb = (FtAudioCircularBuffer*)malloc(sizeof(FtAudioCircularBuffer));
    
    // use next power of two so we can do a bitwise wrap
    length = next_pow2(length);
    float* buffer = (float*)malloc(length * sizeof(float));

    FtAudioFillBuffer(buffer, length, 0.0);
    
    cb->length = length;
    cb->wrap = length - 1;
    cb->buffer = buffer;
    cb->read_index = 0;
    cb->write_index = 0;
    return cb;
}


/* FtAudioCircularBufferFree **************************************************/
void
FtAudioCircularBufferFree(FtAudioCircularBuffer* cb)
{
    free(cb->buffer);
}


/* FtAudioCircularBufferWrite *************************************************/
void
FtAudioCircularBufferWrite(FtAudioCircularBuffer* cb, const float* src, unsigned n_samples)
{
    for (unsigned i=0; i < n_samples; ++i)
    {
        cb->buffer[++cb->write_index & cb->wrap] = *src++;
    }
}

/* FtAudioCircularBufferRead **************************************************/
void
FtAudioCircularBufferRead(FtAudioCircularBuffer* cb, float* dest, unsigned n_samples)
{
    for (unsigned i=0; i < n_samples; ++i)
    {
        *dest++ = cb->buffer[++cb->read_index & cb->wrap];
    }
}

/* FtAudioCircularBufferFlush *************************************************/
void
FtAudioCircularBufferFlush(FtAudioCircularBuffer* cb)
{
    FtAudioFillBuffer(cb->buffer, cb->length, 0.0);
}

