/*
 *  CircularBuffer.c
 *  c
 *
 *  Created by Hamilton Kibbe on 7/7/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "CircularBuffer.h"
#include "Dsp.h"
#include "Utilities.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


/*******************************************************************************
 CircularBuffer */
struct CircularBuffer
{
    unsigned    length;
    unsigned    wrap;
    float*      buffer;
    unsigned    read_index;
    unsigned    write_index;
    unsigned    count;
};


/*******************************************************************************
 CircularBufferD */
struct CircularBufferD
{
    unsigned    length;
    unsigned    wrap;
    double*     buffer;
    unsigned    read_index;
    unsigned    write_index;
    unsigned    count;
};


/*******************************************************************************
 CircularBufferInit */
CircularBuffer*
CircularBufferInit(unsigned length)
{
    CircularBuffer* cb = (CircularBuffer*)malloc(sizeof(CircularBuffer));
    if (cb)
    {
        // use next power of two so we can do a bitwise wrap
        length = next_pow2(length);
        float* buffer = (float*)malloc(length * sizeof(float));

        ClearBuffer(buffer, length);

        cb->length = length;
        cb->wrap = length - 1;
        cb->buffer = buffer;
        cb->read_index = 0;
        cb->write_index = 0;
        cb->count = 0;
    }
    return cb;
}



CircularBufferD*
CircularBufferInitD(unsigned length)
{
    CircularBufferD* cb = (CircularBufferD*)malloc(sizeof(CircularBufferD));
    if (cb)
    {
        // use next power of two so we can do a bitwise wrap
        length = next_pow2(length);
        double* buffer = (double*)malloc(length * sizeof(double));

        ClearBufferD(buffer, length);

        cb->length = length;
        cb->wrap = length - 1;
        cb->buffer = buffer;
        cb->read_index = 0;
        cb->write_index = 0;
        cb->count = 0;
    }
    return cb;
}


/*******************************************************************************
 CircularBufferFree */
Error_t
CircularBufferFree(CircularBuffer* cb)
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
    return NOERR;
}


Error_t
CircularBufferFreeD(CircularBufferD* cb)
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
    return NOERR;
}

/*******************************************************************************
 CircularBufferWrite */
Error_t
CircularBufferWrite(CircularBuffer* cb, const float* src, unsigned n_samples)
{
    for (unsigned i=0; i < n_samples; ++i)
    {
        cb->buffer[++cb->write_index & cb->wrap] = *src++;
    }
    cb->count += n_samples;

    if (cb->count <= cb->length)
    {
        return NOERR;
    }
    else
    {
        return VALUE_ERROR;
    }
}

Error_t
CircularBufferWriteD(CircularBufferD* cb, const double* src, unsigned n_samples)
{
    for (unsigned i=0; i < n_samples; ++i)
    {
        cb->buffer[++cb->write_index & cb->wrap] = *src++;
    }
    cb->count += n_samples;

    if (cb->count <= cb->length)
    {
        return NOERR;
    }
    else
    {
        return VALUE_ERROR;
    }
}


/*******************************************************************************
 CircularBufferRead */
Error_t
CircularBufferRead(CircularBuffer* cb, float* dest, unsigned n_samples)
{
    for (unsigned i=0; i < n_samples; ++i)
    {
        *dest++ = cb->buffer[++cb->read_index & cb->wrap];
    }
    cb->count -= n_samples;

    if (cb->count <= cb->length)
    {
        return NOERR;
    }
    else
    {
        return VALUE_ERROR;
    }
}


Error_t
CircularBufferReadD(CircularBufferD* cb, double* dest, unsigned n_samples)
{
    for (unsigned i=0; i < n_samples; ++i)
    {
        *dest++ = cb->buffer[++cb->read_index & cb->wrap];
    }
    cb->count -= n_samples;

    if (cb->count <= cb->length)
    {
        return NOERR;
    }
    else
    {
        return VALUE_ERROR;
    }
}


/*******************************************************************************
 CircularBufferFlush */
Error_t
CircularBufferFlush(CircularBuffer* cb)
{
    ClearBuffer(cb->buffer, cb->length);
    cb->count = 0;
    return NOERR;
}


Error_t
CircularBufferFlushD(CircularBufferD* cb)
{
    ClearBufferD(cb->buffer, cb->length);
    cb->count = 0;
    return NOERR;
}


/*******************************************************************************
 CircularBufferRewind */

Error_t
CircularBufferRewind(CircularBuffer* cb, unsigned samples)
{
    cb->read_index = ((cb->read_index + cb->length) - samples) % cb->length;
    cb->count += samples;

    if (cb->count <= cb->length)
    {
        return NOERR;
    }
    else
    {
        return VALUE_ERROR;
    }
}

Error_t
CircularBufferRewindD(CircularBufferD* cb, unsigned samples)
{
    cb->read_index = ((cb->read_index + cb->length) - samples) % cb->length;
    cb->count += samples;

    if (cb->count <= cb->length)
    {
        return NOERR;
    }
    else
    {
        return VALUE_ERROR;
    }
}

/*******************************************************************************
 CircularBufferCount */
unsigned
CircularBufferCount(CircularBuffer* cb)
{
    return cb->count;
}

unsigned
CircularBufferCountD(CircularBufferD* cb)
{
    return cb->count;
}
