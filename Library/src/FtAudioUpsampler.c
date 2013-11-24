/*
 * FtAudioUpsampler.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioUpsampler.h"
#include "FtAudioPolyphaseCoeffs.h"
#include <math.h>
#include <stdlib.h>

/* FTA_Upsampler **********************************************************/
struct FTA_Upsampler
{
	unsigned factor;
	FTA_FIRFilter** polyphase;
};


/* FTA_UpsamplerInit *******************************************************/
FTA_Upsampler*
FTA_UpsamplerInit(FTA_UpsampleFactor_t factor)
{
	// Polyphase filter coefficients
	//extern float** polyphaseCoeffs[];

	// Allocate memory for the upsampler
	FTA_Upsampler* upsampler = (FTA_Upsampler*)malloc(sizeof(FTA_Upsampler));

	// Allocate memory for the polyphase array
	upsampler->polyphase = (FTA_FIRFilter**)malloc(factor * sizeof(FTA_Upsampler*));

	// Create polyphase filters
	unsigned idx;
	for(idx = 0; idx < 4; ++idx)
	{
		upsampler->polyphase[idx] = FTA_FIRFilterInit(PolyphaseCoeffs[UP_4X][idx],64, BEST);
	}

	// Add factor
	upsampler->factor = (unsigned)powf(2,(factor + 2));
	
	return upsampler;
}


/* FTA_UpsamplerFree *******************************************************/
FTA_Error_t
FTA_UpsamplerFree(FTA_Upsampler* upsampler)
{
	free(upsampler->polyphase);
	free(upsampler);
	return FT_NOERR;
}

FTA_Error_t
FTA_UpsamplerFlush(FTA_Upsampler* upsampler)
{
	unsigned idx;
	for (idx = 0; idx < upsampler->factor; ++idx)
	{
		FTA_FIRFilterFlush(upsampler->polyphase[idx]);
	}
	return FT_NOERR;
}


/* FTA_UpsamplerProcess ****************************************************/
FTA_Error_t
FTA_UpsamplerProcess(FTA_Upsampler	*upsampler, 
						float				*outBuffer, 
						const float			*inBuffer, 
						unsigned			n_samples)
{
	unsigned sampleIdx;
	unsigned filterIdx;
	unsigned offset;
	
	for (sampleIdx = 0; sampleIdx < n_samples; ++sampleIdx)
	{
		offset = upsampler->factor * sampleIdx;
		for(filterIdx = 0; filterIdx < upsampler->factor; ++ filterIdx)
		{
			FTA_FIRFilterProcess(upsampler->polyphase[filterIdx], 
									&outBuffer[filterIdx + offset], 
									&inBuffer[sampleIdx], 1);
		}
	}
	return FT_NOERR;
}

