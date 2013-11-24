/*
 * FtAudioRBJFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioRBJFilter.h"
#include "FtAudioBiquadFilter.h"
#include "FtAudioDsp.h"
#include "FtAudioUtilities.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* FTA_RBJFilter ***********************************************************/
struct FTA_RBJFilter
{
	FTA_BiquadFilter* biquad;
	Filter_t type;
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


static FTA_Error_t
FTA_RBJFilterUpdate(FTA_RBJFilter* filter);


/* FTA_RBJFilterUpdate *****************************************************/
static FTA_Error_t
FTA_RBJFilterUpdate(FTA_RBJFilter* filter)
{
    filter->cosOmega = cos(filter->omega);
	filter->sinOmega = sin(filter->omega);
    
	switch (filter->type)
	{
	case LOWPASS:
        filter->alpha = filter->sinOmega / (2.0 * filter->Q);
		filter->b[0] = (1 - filter->cosOmega) / 2;
		filter->b[1] = 1 - filter->cosOmega;
		filter->b[2] = filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case HIGHPASS:
        filter->alpha = filter->sinOmega / (2.0 * filter->Q);
		filter->b[0] = (1 + filter->cosOmega) / 2;
		filter->b[1] = -(1 + filter->cosOmega);
		filter->b[2] = filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case BANDPASS:
        filter->alpha = filter->sinOmega * sinhf(logf(2.0) / 2.0 * \
            filter->Q * filter->omega/filter->sinOmega);
		filter->b[0] = filter->sinOmega / 2;
		filter->b[1] = 0;
		filter->b[2] = -filter->b[0];
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = -2 * filter->cosOmega;
		filter->a[2] = 1 - filter->alpha;
		break;

	case ALLPASS:
        filter->alpha = filter->sinOmega / (2.0 * filter->Q);
		filter->b[0] = 1 - filter->alpha;
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1 + filter->alpha;
		filter->a[0] = filter->b[2];
		filter->a[1] = filter->b[1];
		filter->a[2] = filter->b[0];
		break;

	case NOTCH:
        filter->alpha = filter->sinOmega * sinhf(logf(2.0) / 2.0 * \
            filter->Q * filter->omega/filter->sinOmega);
		filter->b[0] = 1;
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1;
		filter->a[0] = 1 + filter->alpha;
		filter->a[1] = filter->b[1];
		filter->a[2] = 1 - filter->alpha;
		break;

	case PEAK:
        filter->alpha = filter->sinOmega * sinhf(logf(2.0) / 2.0 * \
            filter->Q * filter->omega/filter->sinOmega);
		filter->b[0] = 1 + (filter->alpha * filter->A);
		filter->b[1] = -2 * filter->cosOmega;
		filter->b[2] = 1 - (filter->alpha * filter->A);
		filter->a[0] = 1 + (filter->alpha / filter->A);
		filter->a[1] = filter->b[1];
		filter->a[2] = 1 - (filter->alpha / filter->A);
		break;

	case LOW_SHELF:
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

	case HIGH_SHELF:
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
    FTA_VectorScalarMultiply(norm_a, &filter->a[1], factor, 2);
    FTA_VectorScalarMultiply(norm_b, filter->b, factor, 3);
    //printf("b = [ %0.10f %0.10f %0.10f];\n", norm_b[0], norm_b[1], norm_b[2]);
    //printf("a = [ %0.10f %0.10f %0.10f];\n", 1.0, norm_a[0], norm_a[1]);
    FTA_BiquadFilterUpdateKernel(filter->biquad, norm_b, norm_a);
	return FT_NOERR;
}


/* FTA_RBJFilterInit **********************************************************/
FTA_RBJFilter* 
FTA_RBJFilterInit(Filter_t type, float cutoff,long long sampleRate)
{	
	// Create the filter
	FTA_RBJFilter* filter = (FTA_RBJFilter*)malloc(sizeof(FTA_RBJFilter));

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
    filter->biquad = FTA_BiquadFilterInit(b,a);
    
    // Calculate coefficients
	FTA_RBJFilterUpdate(filter);

	return filter;
}


/* FTA_RBJFilterFree *******************************************************/
FTA_Error_t 
FTA_RBJFilterFree(FTA_RBJFilter* 	filter)
{
	FTA_BiquadFilterFree(filter->biquad);
	free(filter);
	return FT_NOERR;
}

/* FTA_RBJFilterSetType ****************************************************/
FTA_Error_t 
FTA_RBJFilterSetType(FTA_RBJFilter*	filter,
						  Filter_t 		type)
{
	filter->type = type;
	FTA_RBJFilterUpdate(filter);
	return FT_NOERR;
}


/* FTA_RBJFilterSetCutoff **************************************************/
FTA_Error_t 
FTA_RBJFilterSetCutoff(FTA_RBJFilter* filter,
						  float 			cutoff)
{
	filter->omega = HZ_TO_RAD(cutoff) / filter->sampleRate;
	FTA_RBJFilterUpdate(filter);
	return FT_NOERR;
}


/* FTA_RBJFilterSetQ *******************************************************/
FTA_Error_t 
FTA_RBJFilterSetQ(FTA_RBJFilter* 	filter, 
					 float 				Q)
{
	filter->Q = Q;
	FTA_RBJFilterUpdate(filter);
	return FT_NOERR;
}

/* FTA_RBJFilterSetParams **************************************************/
FTA_Error_t
FTA_RBJFilterSetParams(FTA_RBJFilter* filter,
                          Filter_t       type,
                          float             cutoff,
                          float             Q)
{
    filter->type = type;
    filter->omega = HZ_TO_RAD(cutoff) / filter->sampleRate;
    filter->Q = Q;
    FTA_RBJFilterUpdate(filter);
    return FT_NOERR;
}

/* FTA_RBJFilterProcess ****************************************************/
FTA_Error_t
FTA_RBJFilterProcess(FTA_RBJFilter* 	filter,
						float* 				outBuffer,
						const float* 		inBuffer, 
						unsigned 			n_samples)
{
	FTA_BiquadFilterProcess(filter->biquad,outBuffer,inBuffer,n_samples);
	return FT_NOERR;
}

/* FTA_RBJFilterFlush ******************************************************/
FTA_Error_t
FTA_RBJFilterFlush(FTA_RBJFilter* filter)
{
    FTA_BiquadFilterFlush(filter->biquad);
    return FT_NOERR;
}