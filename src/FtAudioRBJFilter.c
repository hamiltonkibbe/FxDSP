/*
 * FtAudioRBJFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioRBJFilter.h"
#include <math.h>
#include <stdlib.h>

/* FtAudioRBJFilter ***********************************************************/
struct FtAudioRBJFilter
{
	FtAudioBiquadFilter* biquad;
	RBJFilter_t type;
	float omega;
	float Q;
	float cosOmega;
	float sinOmega;
	float alpha;
	float A;
	float dbGain;
	float b[3];
	float a[3];
	long long sampleRate;
};






/* FtAudioRBJFilterUpdate *****************************************************/
static FtAudioError_t
FtAudioRBJFilterUpdate(FtAudioRBJFilter* filter)
{
	switch (filter->type)
	{
	case RBJ_LPF:
		filter->b[0] = (1 - filter->cosOmega) / 2;
		filter->b[1] = 1 - filter->cosOmega;
		filter->b[2] = filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case RBJ_HPF:
		filter->b[0] = (1 + filter->cosOmega) / 2;
		filter->b[1] = -(1 + filter->cosOmega);
		filter->b[2] = filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case RBJ_BPF:
		filter->b[0] = filter->sinOmega / 2;
		filter->b[1] = 0;
		filter->b[2] = -filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case RBJ_APF:
		filter->b[0] = 1 - filter->alpha;
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1 + filter->alpha;
		filter->a[0] = filter->b[2];
		filter->a[1] = filter->b[1];
		filter->a[2] = filter->b[0];
		break;

	case RBJ_NOTCH:
		filter->b[0] = 1;
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1;
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = filter->b[1];
		filter->a[2] = 1 - filter->alpha;
		break;

	case RBJ_PEAK:
		filter->b[0] = 1 + (filter->alpha * filter->A);
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1 - (filter->alpha * filter->A);
		filter->a[0] = 1 + (filter->alpha / filter->A);
		filter->a[1] = filter->b[1];
		filter->a[2] = 1 - (filter->alpha / filter->A);
		break;

	case RBJ_LSHELF:
		break;

	case RBJ_HSHELF:
		break;

	default:
		return FT_ERR;
		break;
	}
	return FT_NOERR;
}


/* FtAudioRBJFilterInit *******************************************************/
FtAudioRBJFilter* 
FtAudioRBJFilterInit(RBJFilter_t type, float cutoff,long long sampleRate)
{	
	// Create the filter
	FtAudioRBJFilter* filter = (FtAudioRBJFilter*)malloc(sizeof(FtAudioRBJFilter));

	// Initialization
	filter->type = type;
	filter->omega = hzToRadians(cutoff, sampleRate);
	filter->cosOmega = cos(filter->omega);
	filter->sinOmega = sin(filter->omega);
	filter->Q = 1;
	filter->A = 1;
	filter->dbGain = 0;
	filter->sampleRate = sampleRate;

	// Type-specific initialization
	switch (type)
	{
	case RBJ_LPF:
		filter->alpha = filter->sinOmega/(2*filter->Q);
		break;

	case RBJ_HPF:
		filter->alpha = filter->sinOmega/(2*filter->Q);
		break;

	case RBJ_BPF:
		break;

	case RBJ_APF:
		filter->alpha = filter->sinOmega/(2*filter->Q);
		break;

	case RBJ_NOTCH:
		break;

	case RBJ_PEAK:
		break;

	case RBJ_LSHELF:
		break;

	case RBJ_HSHELF:
		break;
	default:
		break;
	}
	
	//Update Coefficients and create biquad
	FtAudioRBJFilterUpdate(filter);
	filter->biquad = FtAudioBiquadFilterInit(filter->b,filter->a);

	return filter;
}


/* FtAudioRBJFilterFree *******************************************************/
FtAudioError_t 
FtAudioRBJFilterFree(FtAudioRBJFilter* 	filter)
{
	FtAudioBiquadFilterFree(filter->biquad);
	free(filter);
	return FT_NOERR;
}

/* FtAudioRBJFilterSetType ****************************************************/
FtAudioError_t 
FtAudioRBJFilterSetType(FtAudioRBJFilter*	filter,
						  RBJFilter_t 		type)
{
	filter->type = type;
	FtAudioRBJFilterUpdate(filter);
	return FT_NOERR;
}


/* FtAudioRBJFilterSetCutoff **************************************************/
FtAudioError_t 
FtAudioRBJFilterSetCutoff(FtAudioRBJFilter* filter,
						  float 			cutoff)
{
	filter->omega = hzToRadians(cutoff, filter->sampleRate);
	FtAudioRBJFilterUpdate(filter);
	return FT_NOERR;
}


/* FtAudioRBJFilterSetQ *******************************************************/
FtAudioError_t 
FtAudioRBJFilterSetQ(FtAudioRBJFilter* 	filter, 
					 float 				Q)
{
	filter->Q = Q;
	FtAudioRBJFilterUpdate(filter);
	return FT_NOERR;
}


/* FtAudioRBJFilterProcess ****************************************************/
FtAudioError_t
FtAudioRBJFilterProcess(FtAudioRBJFilter* 	filter,
						float* 				outBuffer,
						const float* 		inBuffer, 
						unsigned 			n_samples)
{
	FtAudioBiquadFilterProcess(filter->biquad,outBuffer,inBuffer,n_samples);
	return FT_NOERR;
}
