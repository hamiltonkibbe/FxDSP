/*
 *  FtAudioCircularBuffer.c
 *  c
 *
 *  Created by Hamilton Kibbe on 7/7/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioCircularBuffer.h"
#include "FtAudioUtilities.h"
#include <string.h>


#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

struct FtAudioCircularBuffer
{
	float* buffer;
	float* read_index;
	float* write_index;
};


FtAudioCircularBuffer* 
FtAudioCircularBufferInit(unsigned length)
{
	FtAudioCircularBuffer* cb = (FtAudioCircularBuffer*)malloc(sizeof(FtAudioCircularBuffer));
	float* buffer = (float*)malloc(length * sizeof(float));
	
#ifdef __APPLE__
	float zero = 0.0;
	vDSP_vfill(&zero, buffer, 1, length);
#else
	float* it = buffer;
	while(it != buffer+length)
	{
		*it++ = 0.0;
	}
#endif
	
	cb->buffer = buffer;
	cb->read_index = buffer;
	cb->write_index = buffer;
	return cb;
	
}

