/*
 *  kCircularBuffer.c
 *  audio pass-through
 *
 *  Created by Hamilton Kibbe on 3/18/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "kCircularBuffer.h"

int kCircularBufferInit(kCircularBufferStruct *buffer, unsigned bufferLength)
{
	// Initialize members
	buffer->length = bufferLength;
	if(!(buffer->data = (float*)calloc(buffer->length, sizeof(float)))) return 0;
	buffer->dataEnd = buffer->data + buffer->length;
	buffer->writeIndex = buffer->data;
	buffer->readIndex = buffer->data;
	
	return 1;
}



void kCircularBufferWrite(kCircularBufferStruct *buffer, int samples, float *data)
{
	// Use faster implementation if possible
	if (buffer->dataEnd - buffer->writeIndex >= samples)
	{
		memcpy(buffer->writeIndex, data, samples * sizeof(float));
	}
	
	// otherwise copy explicitly
	else
	{
		for (int i = 0; i < samples; ++i)
		{
			*(buffer->writeIndex)++ = *data++;
			if (buffer->writeIndex = buffer->dataEnd) buffer->writeIndex = buffer->data;
		}
	}
}



float kCircularBufferRead(kCircularBufferStruct *buffer, int index)
{
	// Move read index to specified offset
	buffer->readIndex = (buffer->writeIndex - 1) + index;
	
	// Wrap around buffer
	if (buffer->readIndex < buffer->data) buffer->readIndex += buffer->length;
	else if (buffer->readIndex >= buffer->dataEnd) buffer->readIndex -= buffer->length;
	
	// Return sample
	return *(buffer->readIndex);
}


void kCircularBufferDestroy(kCircularBufferStruct *buffer)
{
	free((void *)buffer->data);
}
