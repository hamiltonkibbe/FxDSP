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
#include <stdlib.h>
#include <string.h>


/* FtAudioCircularBuffer ******************************************************/
struct FtAudioCircularBuffer
{
    float* buffer;
    float* read_index;
    float* write_index;
};

/* FtAudioCircularBufferInit **************************************************/
FtAudioCircularBuffer* 
FtAudioCircularBufferInit(unsigned length)
{
    FtAudioCircularBuffer* cb = (FtAudioCircularBuffer*)malloc(sizeof(FtAudioCircularBuffer));
    float* buffer = (float*)malloc(length * sizeof(float));

    FtAudioFillBuffer(buffer, length, 0.0);
    
    cb->buffer = buffer;
    cb->read_index = buffer;
    cb->write_index = buffer;
    return cb;
    
}

