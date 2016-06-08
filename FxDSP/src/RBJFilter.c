/*
 * RBJFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "RBJFilter.h"
#include "BiquadFilter.h"
#include "Dsp.h"
#include "Utilities.h"
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* RBJFilter ***********************************************************/
struct RBJFilter
{
    BiquadFilter* biquad;
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
    float sampleRate;
};

struct RBJFilterD
{
    BiquadFilterD* biquad;
    Filter_t type;
    double omega;
    double Q;
    double cosOmega;
    double sinOmega;
    double alpha;
    double A;
    double dbGain;
    double b[3];
    double a[3];
    double sampleRate;
};


/* RBJFilterUpdate *****************************************************/

static Error_t
RBJFilterUpdate(RBJFilter* filter)
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
        return ERROR;
        break;
    }

    // Normalize filter coefficients
    float factor = 1.0 / filter->a[0];
    float norm_a[2];
    float norm_b[3];
    VectorScalarMultiply(norm_a, &filter->a[1], factor, 2);
    VectorScalarMultiply(norm_b, filter->b, factor, 3);
    BiquadFilterUpdateKernel(filter->biquad, norm_b, norm_a);
    return NOERR;
}


static Error_t
RBJFilterUpdateD(RBJFilterD* filter)
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
            filter->alpha = filter->sinOmega * sinh(logf(2.0) / 2.0 * \
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
            filter->alpha = filter->sinOmega * sinh(logf(2.0) / 2.0 * \
                                                     filter->Q * filter->omega/filter->sinOmega);
            filter->b[0] = 1;
            filter->b[1] = -2 * filter->cosOmega;
            filter->b[2] = 1;
            filter->a[0] = 1 + filter->alpha;
            filter->a[1] = filter->b[1];
            filter->a[2] = 1 - filter->alpha;
            break;

        case PEAK:
            filter->alpha = filter->sinOmega * sinh(logf(2.0) / 2.0 * \
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
            return ERROR;
            break;
    }

    // Normalize filter coefficients
    double factor = 1.0 / filter->a[0];
    double norm_a[2];
    double norm_b[3];
    VectorScalarMultiplyD(norm_a, &filter->a[1], factor, 2);
    VectorScalarMultiplyD(norm_b, filter->b, factor, 3);
    BiquadFilterUpdateKernelD(filter->biquad, norm_b, norm_a);
    return NOERR;
}


/* RBJFilterInit **********************************************************/
RBJFilter*
RBJFilterInit(Filter_t type, float cutoff, float sampleRate)
{
    // Create the filter
    RBJFilter* filter = (RBJFilter*)malloc(sizeof(RBJFilter));

    if (filter)
    {
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
        filter->biquad = BiquadFilterInit(b,a);

        // Calculate coefficients
        RBJFilterUpdate(filter);
    }

    return filter;
}

RBJFilterD*
RBJFilterInitD(Filter_t type, double cutoff, double sampleRate)
{
    // Create the filter
    RBJFilterD* filter = (RBJFilterD*)malloc(sizeof(RBJFilterD));

    if (filter)
    {
        // Initialization
        filter->type = type;
        filter->omega =  HZ_TO_RAD(cutoff) / sampleRate;
        filter->Q = 1;
        filter->A = 1;
        filter->dbGain = 0;
        filter->sampleRate = sampleRate;


        // Initialize biquad
        double b[3] = {0, 0, 0};
        double a[2] = {0, 0};
        filter->biquad = BiquadFilterInitD(b, a);

        // Calculate coefficients
        RBJFilterUpdateD(filter);
    }

    return filter;
}


/* RBJFilterFree *******************************************************/
Error_t
RBJFilterFree(RBJFilter* filter)
{
    BiquadFilterFree(filter->biquad);
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    return NOERR;
}

Error_t
RBJFilterFreeD(RBJFilterD* filter)
{
    BiquadFilterFreeD(filter->biquad);
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    return NOERR;
}

/* RBJFilterSetType ****************************************************/
Error_t
RBJFilterSetType(RBJFilter* filter, Filter_t type)
{
    filter->type = type;
    RBJFilterUpdate(filter);
    return NOERR;
}


Error_t
RBJFilterSetTypeD(RBJFilterD* filter, Filter_t type)
{
    filter->type = type;
    RBJFilterUpdateD(filter);
    return NOERR;
}

/* RBJFilterSetCutoff **************************************************/
Error_t
RBJFilterSetCutoff(RBJFilter* filter, float cutoff)
{
    filter->omega = HZ_TO_RAD(cutoff) / filter->sampleRate;
    RBJFilterUpdate(filter);
    return NOERR;
}


Error_t
RBJFilterSetCutoffD(RBJFilterD* filter, double cutoff)
{
    filter->omega = HZ_TO_RAD(cutoff) / filter->sampleRate;
    RBJFilterUpdateD(filter);
    return NOERR;
}


/* RBJFilterSetQ *******************************************************/
Error_t
RBJFilterSetQ(RBJFilter* filter, float Q)
{
    filter->Q = Q;
    RBJFilterUpdate(filter);
    return NOERR;
}

Error_t
RBJFilterSetQD(RBJFilterD* filter, double Q)
{
    filter->Q = Q;
    RBJFilterUpdateD(filter);
    return NOERR;
}

/* RBJFilterSetParams **************************************************/
Error_t
RBJFilterSetParams(RBJFilter*   filter,
                   Filter_t     type,
                   float        cutoff,
                   float        Q)
{
    filter->type = type;
    filter->omega = HZ_TO_RAD(cutoff) / filter->sampleRate;
    filter->Q = Q;
    RBJFilterUpdate(filter);
    return NOERR;
}

Error_t
RBJFilterSetParamsD(RBJFilterD* filter,
                    Filter_t    type,
                    double      cutoff,
                    double      Q)
{
    filter->type = type;
    filter->omega = HZ_TO_RAD(cutoff) / filter->sampleRate;
    filter->Q = Q;
    RBJFilterUpdateD(filter);
    return NOERR;
}

/* RBJFilterProcess ****************************************************/
Error_t
RBJFilterProcess(RBJFilter*     filter,
                        float*              outBuffer,
                        const float*        inBuffer,
                        unsigned            n_samples)
{
    BiquadFilterProcess(filter->biquad,outBuffer,inBuffer,n_samples);
    return NOERR;
}

Error_t
RBJFilterProcessD(RBJFilterD*   filter,
                  double*       outBuffer,
                  const double* inBuffer,
                  unsigned      n_samples)
{
    BiquadFilterProcessD(filter->biquad,outBuffer,inBuffer,n_samples);
    return NOERR;
}

/* RBJFilterFlush ******************************************************/
Error_t
RBJFilterFlush(RBJFilter* filter)
{
    BiquadFilterFlush(filter->biquad);
    return NOERR;
}

Error_t
RBJFilterFlushD(RBJFilterD* filter)
{
    BiquadFilterFlushD(filter->biquad);
    return NOERR;
}

