/*
 *  FtAudioUtilities.c
 *  c
 *
 *  Created by Hamilton Kibbe on 6/6/12.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioUtilities.h"
#include <math.h>


/* 32 bit "pointer cast" union */
typedef union {
        float f;
        int32_t i;
} f_pcast32;



/* f_max **********************************************************************/
inline float 
f_max(float x, float a)
{
	x -= a;
	x += fabs(x);
	x *= 0.5;
	x += a;
	return x;
}



/* f_min **********************************************************************/
inline float
f_min(float x, float b)
{
	x = b - x;
	x += fabs(x);
	x *= 0.5;
	x = b - x;
	return x;
}



/* f_clamp ********************************************************************/
inline float 
f_clamp(float x, float a, float b)
{
	const float x1 = fabs(x - a);
	const float x2 = fabs(x - b);
	x = x1 + a + b;
	x -= x2;
	x *= 0.5;
	return x;
}



/* f_pow2 *********************************************************************/
inline float
f_pow2(float x)
{
	f_pcast32 *px;
    f_pcast32 tx
    f_pcast32 lx;
	float dx;

	px = (ls_pcast32 *)&x;          // store address of float as long pointer
	tx.f = (x-0.5f) + (3<<22);      // temporary value for truncation
	lx.i = tx.i - 0x4b400000;       // integer power of 2
	dx = x - (float)lx.i;           // float remainder of power of 2

	x = 1.0f + dx * (0.6960656421638072f +  // cubic apporoximation of 2^x
		   dx * (0.224494337302845f +       // for x in the range [0, 1]
		   dx * (0.07944023841053369f)));
	(*px).i += (lx.i << 23);                // add int power of 2 to exponent

	return (*px).f;
}



/* int16ToFloat ***************************************************************/
inline float 
int16ToFloat(signed short sample)
{
    return (float)(sample * INT16_TO_FLOAT_SCALAR);
}

/* floatToInt16 ***************************************************************/
inline signed short
floatToInt16(float sample)
{
    return (signed short)(sample * 32767.0);
}




/* intBufferToFloat ***********************************************************/
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

/* floatBufferToInt ***********************************************************/
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

/* ratioToDb ******************************************************************/
inline float
ratioToDb(float ratio)
{
    return 20.0*log10f(ratio);
}


/* dbToRatio ******************************************************************/
inline float
dbToRatio(float dB)
{
    return (dB > -90.0f ? powf(10.0, dB * 0.05f): 0.0f);
}

