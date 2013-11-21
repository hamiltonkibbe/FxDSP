/*
 *  FtAudioLadderFilter.c
 *  
 *
 *  Created by Hamilton Kibbe on 5/10/13.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioLadderFilter.h"
#include "FtAudioDsp.h"
#include "FtAudioUtilities.h"

#include <stdlib.h>
#include <math.h>

/* FtAudioLadderFilter ********************************************************/
struct FtAudioLadderFilter
{
    float y[4];
    float w[4];
	float Vt;           // transistor treshold voltage [V]
    float sample_rate;
    float cutoff;
	float resonance;
};


/* FtAudioLadderFilterInit ****************************************************/
FtAudioLadderFilter* 
FtAudioLadderFilterInit(float _sample_rate)
{
	FtAudioLadderFilter *filter = (FtAudioLadderFilter*)malloc(sizeof(FtAudioLadderFilter));
	FtAudioFillBuffer(filter->y, 4, 0.0);
	FtAudioFillBuffer(filter->w, 4, 0.0);
    filter->Vt = 0.026;
	filter->cutoff = 0;
	filter->resonance = 0;
	filter->sample_rate = _sample_rate;
	return filter;
}

/* FtAudioLadderFilterFree ****************************************************/
FtAudioError_t 
FtAudioLadderFilterFree(FtAudioLadderFilter* filter)
{
	free(filter);
	return FT_NOERR;
}


/* FtAudioLadderFilterFlush ***************************************************/
FtAudioError_t
FtAudioLadderFilterFlush(FtAudioLadderFilter* filter)
{
	FtAudioFillBuffer(filter->y, 4, 0.0);
	FtAudioFillBuffer(filter->w, 4, 0.0);
	filter->cutoff = 0;
	filter->resonance = 0;
	return FT_NOERR;
}


/* FtAudioLadderFilterProcess *************************************************/
FtAudioError_t
FtAudioLadderFilterProcess(FtAudioLadderFilter *filter, float *outBuffer, float *inBuffer, unsigned n_samples)
{
	// Pre-calculate Scalars
	float TWO_VT_INV = 1.0 / (2 * filter->Vt);
	float TWO_VT_G = 2 * filter->Vt * (1 - exp(-TWO_PI * filter->cutoff / filter->sample_rate));

	// Filter audio
	for (unsigned i = 0; i < n_samples; ++i)
	{
		
		// Stage 1 output
		filter->y[0] = filter->y[0] + TWO_VT_G * (f_tanh(inBuffer[i] - 4 * f_tanh(2 * filter->resonance * filter->y[3]) * TWO_VT_INV) - filter->w[0]);
		filter->w[0] = f_tanh(filter->y[0] * TWO_VT_INV);
			
		// Stage 2 output
		filter->y[1] = filter->y[1] + TWO_VT_G * (filter->w[0]- filter->w[1]);
		filter->w[1] = f_tanh(filter->y[1] * TWO_VT_INV);
			
		// Stage 3 output
		filter->y[2] = filter->y[2] + TWO_VT_G * (filter->w[1]- filter->w[2]);
		filter->w[2] = f_tanh(filter->y[2] * TWO_VT_INV);
			
		// Stage 4 output
		filter->y[3] = filter->y[3] + TWO_VT_G * (filter->w[2]- filter->w[3]);
		filter->w[3] = f_tanh(filter->y[3] * TWO_VT_INV);
		
		// Write output
		outBuffer[i] = filter->y[3];
			
	}

	return FT_NOERR;
}

FtAudioError_t
FtAudioLadderFilterSetTemperature(FtAudioLadderFilter *filter, float tempC)
{
    float T = tempC + 273.15;
    filter->Vt = BOLTZMANS_CONSTANT * T / Q;
    return FT_NOERR;
}