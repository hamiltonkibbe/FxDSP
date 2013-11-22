/*
 * FtAudioRBJFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioRBJFilter.h"
#include "FtAudioDsp.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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


static FtAudioError_t
FtAudioRBJFilterUpdate(FtAudioRBJFilter* filter);


/* FtAudioRBJFilterUpdate *****************************************************/
static FtAudioError_t
FtAudioRBJFilterUpdate(FtAudioRBJFilter* filter)
{
    filter->cosOmega = cos(filter->omega);
	filter->sinOmega = sin(filter->omega);
    
	switch (filter->type)
	{
	case RBJ_LPF:
        filter->alpha = filter->sinOmega / (2.0 * filter->Q);
		filter->b[0] = (1 - filter->cosOmega) / 2;
		filter->b[1] = 1 - filter->cosOmega;
		filter->b[2] = filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case RBJ_HPF:
        filter->alpha = filter->sinOmega / (2.0 * filter->Q);
		filter->b[0] = (1 + filter->cosOmega) / 2;
		filter->b[1] = -(1 + filter->cosOmega);
		filter->b[2] = filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case RBJ_BPF:
        filter->alpha = filter->sinOmega * sinhf(logf(2.0) / 2.0 * \
            filter->Q * filter->omega/filter->sinOmega);
		filter->b[0] = filter->sinOmega / 2;
		filter->b[1] = 0;
		filter->b[2] = -filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case RBJ_APF:
        filter->alpha = filter->sinOmega / (2.0 * filter->Q);
		filter->b[0] = 1 - filter->alpha;
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1 + filter->alpha;
		filter->a[0] = filter->b[2];
		filter->a[1] = filter->b[1];
		filter->a[2] = filter->b[0];
		break;

	case RBJ_NOTCH:
        filter->alpha = filter->sinOmega * sinhf(logf(2.0) / 2.0 * \
            filter->Q * filter->omega/filter->sinOmega);
		filter->b[0] = 1;
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1;
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = filter->b[1];
		filter->a[2] = 1 - filter->alpha;
		break;

	case RBJ_PEAK:
        filter->alpha = filter->sinOmega * sinhf(logf(2.0) / 2.0 * \
            filter->Q * filter->omega/filter->sinOmega);
		filter->b[0] = 1 + (filter->alpha * filter->A);
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1 - (filter->alpha * filter->A);
		filter->a[0] = 1 + (filter->alpha / filter->A);
		filter->a[1] = filter->b[1];
		filter->a[2] = 1 - (filter->alpha / filter->A);
		break;

	case RBJ_LSHELF:
        filter->alpha = filter->sinOmega / 2.0 * sqrt( (filter->A + 1.0 / \
            filter->A) * (1.0 / filter->Q - 1.0) + 2.0);
        filter->b[0] = filter->A * ((filter->A + 1) - ((filter->A - 1) *       \
            filter->cosOmega) + (2 * sqrtf(filter->A) * filter->alpha));
        filter->b[1] = 2 * filter->A * ((filter->A - 1) - ((filter->A + 1) *   \
            filter->cosOmega));
        filter->b[2] = filter->A * ((filter->A + 1) - ((filter->A - 1) *       \
            filter->cosOmega) - (2 * sqrtf(filter->A) * filter->alpha));
        filter->a[0] = ((filter->A + 1) + ((filter->A - 1) *                   \
            filter->cosOmega) + (2 * sqrtf(filter->A) * filter->alpha));
        filter->a[1] = -2 * ((filter->A - 1) + ((filter->A + 1) *              \
            filter->cosOmega));
        filter->a[2] = ((filter->A + 1) + ((filter->A - 1) *                   \
            filter->cosOmega) - (2 * sqrtf(filter->A) * filter->alpha));
		break;

	case RBJ_HSHELF:
        filter->alpha = filter->sinOmega / 2.0 * sqrt( (filter->A + 1.0 / \
            filter->A) * (1.0 / filter->Q - 1.0) + 2.0);
        filter->b[0] = filter->A * ((filter->A + 1) + ((filter->A - 1) *       \
            filter->cosOmega) + (2 * sqrtf(filter->A) * filter->alpha));
        filter->b[1] = -2 * filter->A * ((filter->A - 1) + ((filter->A + 1) *  \
            filter->cosOmega));
        filter->b[2] = filter->A * ((filter->A + 1) + ((filter->A - 1) *       \
            filter->cosOmega) - (2 * sqrtf(filter->A) * filter->alpha));
        filter->a[0] = ((filter->A + 1) - ((filter->A - 1) *                   \
            filter->cosOmega) + (2 * sqrtf(filter->A) * filter->alpha));
        filter->a[1] = 2 * ((filter->A - 1) - ((filter->A + 1) *               \
            filter->cosOmega));
        filter->a[2] = ((filter->A + 1) - ((filter->A - 1) *                   \
            filter->cosOmega) - (2 * sqrtf(filter->A) * filter->alpha));
		break;

	default:
		return FT_ERR;
		break;
	}
    
    // Normalize filter coefficients
    float factor = 1.0 / filter->a[0];
    float norm_a[2];
    float norm_b[3];
    FtAudioVectorScalarMultiply(norm_a, &filter->a[1], factor, 2);
    FtAudioVectorScalarMultiply(norm_b, filter->b, factor, 3);
    printf("b = [ %0.10f %0.10f %0.10f];\n", norm_b[0], norm_b[1], norm_b[2]);
    printf("a = [ %0.10f %0.10f %0.10f];\n", 1.0, norm_a[0], norm_a[1]);
    FtAudioBiquadFilterUpdateKernel(filter->biquad, norm_b, norm_a);
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
	filter->omega =  HZ_TO_RAD(cutoff) / sampleRate; //hzToRadians(cutoff, sampleRate);
	filter->Q = 1;
	filter->A = 1;
	filter->dbGain = 0;
	filter->sampleRate = sampleRate;

	
	// Initialize biquad
    float b[3] = {0, 0, 0};
    float a[2] = {0, 0};
    filter->biquad = FtAudioBiquadFilterInit(b,a);
    
    // Calculate coefficients
	FtAudioRBJFilterUpdate(filter);

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
	filter->omega = HZ_TO_RAD(cutoff) / filter->sampleRate;
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

/* FtAudioRBJFilterFlush ******************************************************/
FtAudioError_t
FtAudioRBJFilterFlush(FtAudioRBJFilter* filter)
{
    FtAudioBiquadFilterFlush(filter->biquad);
    return FT_NOERR;
}