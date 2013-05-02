/*
 *  FtAudioUtilities.c
 *  c
 *
 *  Created by Hamilton Kibbe on 6/6/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioUtilities.h"
#include <math.h>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

/* int16ToFloat *******************************************************/
inline float 
int16ToFloat(signed short sample)
{
    return (float)(sample * INT16_TO_FLOAT_SCALAR);
}

/* floatToInt16 *******************************************************/
inline signed short
floatToInt16(float sample)
{
    return (signed short)(sample * 32767.0);
}


/* intBufferToFloat ***************************************************/
void
intBufferToFloat(const signed short* inBuffer, float* outBuffer, unsigned nSamples)
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

/* floatBufferToInt ***************************************************/
void
floatBufferToInt(const float* inBuffer, signed short* outBuffer, unsigned nSamples)
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

/* ratioToDb **********************************************************/
inline float
ratioToDb(float ratio)
{
    return 20.0*log10((double)ratio);
}


/* dbToRatio **********************************************************/
inline float
dbToRatio(float dB)
{
    return pow(10.0,(dB/20.0));
}

/* radiansToHz ********************************************************/
inline float
radiansToHz(float radians, long long sampleRate)
{
    return radians * (sampleRate/(2.0 * M_PI));
}


/* hzToRadians ********************************************************/
inline float
hzToRadians(float Hz, long long sampleRate)
{
    return 2.0 * M_PI * Hz/sampleRate;
}


/* FtAudioFillBuffer **************************************************/
void
FtAudioFillBuffer(float* dest, unsigned length, float value)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vfill(&value, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = value;
    }
#endif
}
	

/* FtAudioBufferAdd ****************************************************/
void
FtAudioBufferAdd(float *dest, float *buf1, float *buf2, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
	vDSP_vadd((const float*)buf1, 1, (const float*)buf2, 1, dest, 1, length);
	
#else
	// Otherwise do it manually
	unsigned i;
	for (i = 0; i < length; ++i)
	{
		*dest++ = (*buf1++) + (*buf2++);
	}
	
#endif
}


