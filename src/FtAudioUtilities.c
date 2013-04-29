/*
 *  FtAudioUtilities.c
 *  c
 *
 *  Created by Hamilton Kibbe on 6/6/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioUtilities.h"


inline float 
int16ToFloat(signed short sample)
{
    return (float)(sample * INT16_TO_FLOAT_SCALAR);
}

inline signed short
floatToInt16(float sample)
{
	return (signed short)(sample * 32767.0);
}

void
int_buffer_to_float(const signed short* inBuffer, float* outBuffer, unsigned nSamples)
{
	const signed short *src = inBuffer;
	const signed short *end = inBuffer + nSamples;
	float *dst = outBuffer;
	
	// Convert and write to output buffer
	while(src < end)
	{
		*dst++ = int16ToFloat(*src++);
	}
	
}

void
float_buffer_to_int(const float* inBuffer, signed short* outBuffer, unsigned nSamples)
{
	const float *src = inBuffer;
	const float *end = inBuffer + nSamples;
	signed short *dst = outBuffer;
	
	// Convert and write to output buffer
	while(src < end)
	{
		*dst++ = floatToInt16(*src++);
	}
	
}


inline float to_dB(float ratio)
{
	return 20.0*log10((double)ratio);
}

inline float to_ratio(float dB)
{
	return pow(10.0,(dB/20.0));
}


inline float radians_to_Hz(float radians, long long sampleRate)
{
	return radians * (sampleRate/(2.0 * M_PI));
}


inline float Hz_to_radians(float Hz, long long sampleRate)
{
	return 2.0 * M_PI * Hz/sampleRate;
}

