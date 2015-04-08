/*
 * Upsampler.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "Upsampler.h"
#include "PolyphaseCoeffs.h"
#include "Dsp.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/* Upsampler **********************************************************/
struct Upsampler
{
	unsigned factor;
	FIRFilter** polyphase;
};


/* UpsamplerInit *******************************************************/
Upsampler*
UpsamplerInit(UpsampleFactor_t factor)
{
    unsigned n_filters = 1;
    switch(factor)
    {
        case UP_4X:
            n_filters = 4;
            break;
        case UP_8X:
            n_filters = 8;
            break;
        case UP_16X:
            n_filters = 16;
            break;
        default:
            break;
    }

	// Allocate memory for the upsampler
	Upsampler* upsampler = (Upsampler*)malloc(sizeof(Upsampler));

	// Allocate memory for the polyphase array
	FIRFilter** polyphase = (FIRFilter**)malloc(n_filters * sizeof(FIRFilter*));

    if (upsampler && polyphase)
    {
        upsampler->polyphase = polyphase;
        
        // Create polyphase filters
        unsigned idx;
        for(idx = 0; idx < n_filters; ++idx)
        {
            upsampler->polyphase[idx] = FIRFilterInit(PolyphaseCoeffs[factor][idx], 64, BEST);
        }

        // Add factor
        upsampler->factor = n_filters;
        
        return upsampler;
    }
    else
    {
        if (polyphase)
        {
            free(polyphase);
        }
        if (upsampler)
        {
            free(upsampler);
        }
        return NULL;
    }
}


/* UpsamplerFree *******************************************************/
Error_t
UpsamplerFree(Upsampler* upsampler)
{
    if (upsampler)
    {
        if (upsampler->polyphase)
        {
            for (unsigned i = 0; i < upsampler->factor; ++i)
            {
                FIRFilterFree(upsampler->polyphase[i]);
            }
            free(upsampler->polyphase);
        }
        free(upsampler);
    }
	return NOERR;
}

Error_t
UpsamplerFlush(Upsampler* upsampler)
{
	unsigned idx;
	for (idx = 0; idx < upsampler->factor; ++idx)
	{
		FIRFilterFlush(upsampler->polyphase[idx]);
	}
	return NOERR;
}


/* UpsamplerProcess ****************************************************/
Error_t
UpsamplerProcess(Upsampler      *upsampler,
                 float          *outBuffer,
				 const float    *inBuffer,
				 unsigned       n_samples)
{
    if (upsampler && outBuffer)
    {
	unsigned sampleIdx;
	unsigned filterIdx;
	unsigned offset;
	
	for (sampleIdx = 0; sampleIdx < n_samples; ++sampleIdx)
	{
		offset = upsampler->factor * sampleIdx;
		for(filterIdx = 0; filterIdx < upsampler->factor; ++filterIdx)
		{
			FIRFilterProcess(upsampler->polyphase[filterIdx], 
									&outBuffer[filterIdx + offset], 
									&inBuffer[sampleIdx], 1);
		}
	}
        VectorScalarMultiply(outBuffer, (const float*)outBuffer,
                             upsampler->factor, n_samples * upsampler->factor);
	return NOERR;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}

