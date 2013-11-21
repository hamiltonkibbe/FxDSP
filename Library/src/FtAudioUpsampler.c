/*
 * FtAudioUpsampler.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioUpsampler.h"
#include "FtAudioPolyphaseCoeffs.h"
#include <math.h>
#include <stdlib.h>

/* FtAudioUpsampler **********************************************************/
struct FtAudioUpsampler
{
	unsigned factor;
	FtAudioFIRFilter** polyphase;
};


/* FtAudioUpsamplerInit *******************************************************/
FtAudioUpsampler*
FtAudioUpsamplerInit(FtAudioUpsampleFactor_t factor)
{
	// Polyphase filter coefficients
	//extern float** polyphaseCoeffs[];

	// Allocate memory for the upsampler
	FtAudioUpsampler* upsampler = (FtAudioUpsampler*)malloc(sizeof(FtAudioUpsampler));

	// Allocate memory for the polyphase array
	upsampler->polyphase = (FtAudioFIRFilter**)malloc(factor * sizeof(FtAudioUpsampler*));

	// Create polyphase filters
	unsigned idx;
	for(idx = 0; idx < 4; ++idx)
	{
		upsampler->polyphase[idx] = FtAudioFIRFilterInit(PolyphaseCoeffs[UP_4X][idx],64, BEST);
	}

	// Add factor
	upsampler->factor = (unsigned)powf(2,(factor + 2));
	
	return upsampler;
}


/* FtAudioUpsamplerFree *******************************************************/
FtAudioError_t
FtAudioUpsamplerFree(FtAudioUpsampler* upsampler)
{
	free(upsampler->polyphase);
	free(upsampler);
	return FT_NOERR;
}

FtAudioError_t
FtAudioUpsamplerFlush(FtAudioUpsampler* upsampler)
{
	unsigned idx;
	for (idx = 0; idx < upsampler->factor; ++idx)
	{
		FtAudioFIRFilterFlush(upsampler->polyphase[idx]);
	}
	return FT_NOERR;
}


/* FtAudioUpsamplerProcess ****************************************************/
FtAudioError_t
FtAudioUpsamplerProcess(FtAudioUpsampler	*upsampler, 
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
			FtAudioFIRFilterProcess(upsampler->polyphase[filterIdx], 
									&outBuffer[filterIdx + offset], 
									&inBuffer[sampleIdx], 1);
		}
	}
	return FT_NOERR;
}

