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


/* FTA_CircularBuffer ******************************************************/
struct FTA_CircularBuffer
{
    unsigned    length;
    unsigned    wrap;
    float* buffer;
    unsigned read_index;
    unsigned write_index;
};

/* FTA_CircularBufferInit **************************************************/
FTA_CircularBuffer* 
FTA_CircularBufferInit(unsigned length)
{
    FTA_CircularBuffer* cb = (FTA_CircularBuffer*)malloc(sizeof(FTA_CircularBuffer));
    
    // use next power of two so we can do a bitwise wrap
    length = next_pow2(length);
    float* buffer = (float*)malloc(length * sizeof(float));

    FTA_FillBuffer(buffer, length, 0.0);
    
    cb->length = length;
    cb->wrap = length - 1;
    cb->buffer = buffer;
    cb->read_index = 0;
    cb->write_index = 0;
    return cb;
}


/* FTA_CircularBufferFree **************************************************/
void
FTA_CircularBufferFree(FTA_CircularBuffer* cb)
{
    if (cb)
    {
        if (cb->buffer)
        {
            free(cb->buffer);
            cb->buffer = NULL;
        }
        free(cb);
        cb = NULL;
    }
}


/* FTA_CircularBufferWrite *************************************************/
void
FTA_CircularBufferWrite(FTA_CircularBuffer* cb, const float* src, unsigned n_samples)
{
    for (unsigned i=0; i < n_samples; ++i)
    {
        cb->buffer[++cb->write_index & cb->wrap] = *src++;
    }
}

/* FTA_CircularBufferRead **************************************************/
void
FTA_CircularBufferRead(FTA_CircularBuffer* cb, float* dest, unsigned n_samples)
{
    for (unsigned i=0; i < n_samples; ++i)
    {
        *dest++ = cb->buffer[++cb->read_index & cb->wrap];
    }
}

/* FTA_CircularBufferFlush *************************************************/
void
FTA_CircularBufferFlush(FTA_CircularBuffer* cb)
{
    FTA_FillBuffer(cb->buffer, cb->length, 0.0);
}

