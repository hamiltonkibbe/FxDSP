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

/* FTA_LadderFilter ********************************************************/
struct FTA_LadderFilter
{
    float y[4];
    float w[4];
	float Vt;           // transistor treshold voltage [V]
    float sample_rate;
    float cutoff;
	float resonance;
};


/* FTA_LadderFilterInit ****************************************************/
FTA_LadderFilter* 
FTA_LadderFilterInit(float _sample_rate)
{
	FTA_LadderFilter *filter = (FTA_LadderFilter*)malloc(sizeof(FTA_LadderFilter));
	FTA_FillBuffer(filter->y, 4, 0.0);
	FTA_FillBuffer(filter->w, 4, 0.0);
    filter->Vt = 0.026;
	filter->cutoff = 0;
	filter->resonance = 0;
	filter->sample_rate = _sample_rate;
	return filter;
}

/* FTA_LadderFilterFree ****************************************************/
FTA_Error_t 
FTA_LadderFilterFree(FTA_LadderFilter* filter)
{
	if (filter)
    {
        free(filter);
        filter = NULL;
    }
	return FT_NOERR;
}


/* FTA_LadderFilterFlush ***************************************************/
FTA_Error_t
FTA_LadderFilterFlush(FTA_LadderFilter* filter)
{
	FTA_FillBuffer(filter->y, 4, 0.0);
	FTA_FillBuffer(filter->w, 4, 0.0);
	filter->cutoff = 0;
	filter->resonance = 0;
	return FT_NOERR;
}


/* FTA_LadderFilterProcess *************************************************/
FTA_Error_t
FTA_LadderFilterProcess(FTA_LadderFilter *filter, float *outBuffer, float *inBuffer, unsigned n_samples)
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

FTA_Error_t
FTA_LadderFilterSetTemperature(FTA_LadderFilter *filter, float tempC)
{
    float T = tempC + 273.15;
    filter->Vt = BOLTZMANS_CONSTANT * T / Q;
    return FT_NOERR;
}