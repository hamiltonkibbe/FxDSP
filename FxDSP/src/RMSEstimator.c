//
//  RMSEstimator.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#include "RMSEstimator.h"
#include "Utilities.h"
#include <math.h>
#include <stdlib.h>

/*******************************************************************************
 RMSEstimator */
struct RMSEstimator
{
    float       avgTime;
    long long   sampleRate;
    float       avgCoeff;
    float       RMS;
};

struct RMSEstimatorD
{
    double      avgTime;
    long long   sampleRate;
    double      avgCoeff;
    double      RMS;
};
/*******************************************************************************
 RMSEstimatorInit */
RMSEstimator*
RMSEstimatorInit(float      avgTime,
                 long long  sampleRate)
{
    RMSEstimator* rms = (RMSEstimator*) malloc(sizeof(RMSEstimator));
    rms->avgTime = avgTime;
    rms->sampleRate = sampleRate;
    rms->RMS = 1;
    rms->avgCoeff = 0.5 * (1.0 - expf( -1.0 / (rms->sampleRate * rms->avgTime)));
    
    return rms;
}


RMSEstimatorD*
RMSEstimatorInitD(double     avgTime,
                  long long  sampleRate)
{
    RMSEstimatorD* rms = (RMSEstimatorD*) malloc(sizeof(RMSEstimatorD));
    rms->avgTime = avgTime;
    rms->sampleRate = sampleRate;
    rms->RMS = 1;
    rms->avgCoeff = 0.5 * (1.0 - expf( -1.0 / (rms->sampleRate * rms->avgTime)));
    
    return rms;
}

/*******************************************************************************
 RMSEstimatorFree */
Error_t
RMSEstimatorFree(RMSEstimator* rms)
{
    if (rms)
    {
        free(rms);
        rms = NULL;
    }
    return NOERR;
}

Error_t
RMSEstimatorFreeD(RMSEstimatorD* rms)
{
    if (rms)
    {
        free(rms);
        rms = NULL;
    }
    return NOERR;
}


/*******************************************************************************
 RMSEstimatorProcess */
Error_t
RMSEstimatorProcess(RMSEstimator*   rms,
                        float*              outBuffer,
                        const float*        inBuffer,
                        unsigned            n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        rms->RMS += rms->avgCoeff * ((f_abs(inBuffer[i])/rms->RMS) - rms->RMS);
        outBuffer[i] = rms->RMS;
    }
    return NOERR;
}

Error_t
RMSEstimatorProcessD(RMSEstimatorD* rms,
                     double*        outBuffer,
                     const double*  inBuffer,
                     unsigned       n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        rms->RMS += rms->avgCoeff * ((f_abs(inBuffer[i])/rms->RMS) - rms->RMS);
        outBuffer[i] = rms->RMS;
    }
    return NOERR;
}

/*******************************************************************************
 RMSEstimatorTick */
float
RMSEstimatorTick(RMSEstimator*  rms,
                     float              inSample)
{
    rms->RMS += rms->avgCoeff * ((f_abs(inSample/rms->RMS)) - rms->RMS);
    return rms->RMS;
}

double
RMSEstimatorTickD(RMSEstimatorD* rms, double inSample)
{
    rms->RMS += rms->avgCoeff * ((f_abs(inSample/rms->RMS)) - rms->RMS);
    return rms->RMS;
}