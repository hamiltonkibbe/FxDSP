//
//  DiodeRectifier.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 12/9/14.
//  Copyright (c) 2014 Hamilton Kibbe. All rights reserved.
//

#include "DiodeRectifier.h"
#include "Dsp.h"
#include "Utilities.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>


/*******************************************************************************
 DiodeRectifier */
struct DiodeRectifier
{
    bias_t  bias;
    float   threshold;
    float   vt;
    float   scale;
    float   abs_coeff;
    float*  scratch;
};

struct DiodeRectifierD
{
    bias_t  bias;
    double  threshold;
    double  vt;
    double  scale;
    double abs_coeff;
    double* scratch;
};


/*******************************************************************************
 DiodeRectifierInit */
DiodeRectifier*
DiodeRectifierInit(bias_t bias, float threshold)
{
    /* Allocate memory for diode struct */
    DiodeRectifier* diode = (DiodeRectifier*)malloc(sizeof(DiodeRectifier));

    if (NULL != diode)
    {
        /* Allocate scratch space */
        float* scratch = (float*)malloc(4096 * sizeof(float));

        if (NULL != scratch)
        {
            // Initialization
            diode->bias = bias;
            diode->threshold = threshold;
            diode->scratch = scratch;
            diode->abs_coeff = (bias == FULL_WAVE) ? 1.0 : 0.0;
            DiodeRectifierSetThreshold(diode, threshold);
        }
        else
        {
            free(diode);
            diode = NULL;
        }
    }
    return diode;
}

DiodeRectifierD*
DiodeRectifierInitD(bias_t bias, double threshold)
{
    /* Allocate memory for diode struct */
    DiodeRectifierD* diode = (DiodeRectifierD*)malloc(sizeof(DiodeRectifierD));

    if (NULL != diode)
    {
        /* Allocate scratch space */
        double* scratch = (double*)malloc(4096 * sizeof(double));

        if (NULL != scratch)
        {

            // Initialization
            diode->bias = bias;
            diode->threshold = threshold;
            diode->scratch = scratch;
            diode->abs_coeff = (bias == FULL_WAVE) ? 1.0 : 0.0;
            DiodeRectifierSetThresholdD(diode, threshold);
        }
        else
        {
            free(diode);
            diode = NULL;
        }
    }
    return diode;
}


/*******************************************************************************
 DiodeRectifierFree */
Error_t
DiodeRectifierFree(DiodeRectifier* diode)
{
    if (NULL != diode)
    {
        if (NULL != diode->scratch)
        {
            free(diode->scratch);
        }
        free(diode);
    }
    diode = NULL;
    return NOERR;
}

Error_t
DiodeRectifierFreeD(DiodeRectifierD* diode)
{
    if (NULL != diode)
    {
        if (NULL != diode->scratch)
        {
            free(diode->scratch);
        }
        free(diode);
    }
    diode = NULL;
    return NOERR;
}


/*******************************************************************************
 DiodeRectifierSetThreshold */
Error_t
DiodeRectifierSetThreshold(DiodeRectifier* diode, float threshold)
{
    float scale = 1.0;
    threshold = LIMIT(fabsf(threshold), 0.01, 0.9);
    scale = (1.0 - threshold);
    if (diode->bias== REVERSE_BIAS)
    {
        scale *= -1.0;
        threshold *= -1.0;
    }
    diode->threshold = threshold;
    diode->vt = -0.1738 * threshold + 0.1735;
    diode->scale = scale/(expf((1.0/diode->vt) - 1.));
    return NOERR;
}

Error_t
DiodeRectifierSetThresholdD(DiodeRectifierD* diode, double threshold)
{
    double scale = 1.0;
    threshold = LIMIT(fabs(threshold), 0.01, 0.9);
    scale = (1.0 - threshold);
    if (diode->bias == REVERSE_BIAS)
    {
        scale *= -1.0;
        threshold *= -1.0;
    }
    diode->threshold = threshold;
    diode->vt = -0.1738 * threshold + 0.1735;
    diode->scale = scale/(exp((1.0/diode->vt) - 1.));
    return NOERR;
}


/*******************************************************************************
 DiodeRectifierProcess */
Error_t
DiodeRectifierProcess(DiodeRectifier*   diode,
                      float*            out_buffer,
                      const float*      in_buffer,
                      unsigned          n_samples)
{
    float inv_vt = 1.0 / diode->vt;
    float scale = diode->scale;
    if (diode->bias == FULL_WAVE)
    {
        VectorAbs(diode->scratch, in_buffer, n_samples);
        VectorScalarMultiply(diode->scratch, diode->scratch, inv_vt, n_samples);
    }
    else
    {
        VectorScalarMultiply(diode->scratch, in_buffer, inv_vt, n_samples);
    }
    VectorScalarAdd(diode->scratch, diode->scratch, -1.0, n_samples);
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = expf(diode->scratch[i]) * scale;
    }
    return NOERR;
}

Error_t
DiodeRectifierProcessD(DiodeRectifierD* diode,
                       double*          out_buffer,
                       const double*    in_buffer,
                       unsigned         n_samples)
{
    double inv_vt = 1.0 / diode->vt;
    double scale = diode->scale;
    if (diode->bias == FULL_WAVE)
    {
        VectorAbsD(diode->scratch, in_buffer, n_samples);
        VectorScalarMultiplyD(diode->scratch, diode->scratch, inv_vt, n_samples);
    }
    else
    {
        VectorScalarMultiplyD(diode->scratch, in_buffer, inv_vt, n_samples);
    }
    VectorScalarAddD(diode->scratch, diode->scratch, -1.0, n_samples);
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = exp(diode->scratch[i]) * scale;
    }
    return NOERR;
}

/*******************************************************************************
 DiodeRectifierTick */
float
DiodeRectifierTick(DiodeRectifier* diode, float in_sample)
{
    return expf((in_sample/diode->vt)-1) * diode->scale;
}

double
DiodeRectifierTickD(DiodeRectifierD* diode, double in_sample)
{
    return exp((in_sample/diode->vt)-1) * diode->scale;
}