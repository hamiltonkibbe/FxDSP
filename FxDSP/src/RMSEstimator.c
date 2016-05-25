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
    float   avgTime;
    float   sampleRate;
    float   avgCoeff;
    float   RMS;
};

struct RMSEstimatorD
{
    double  avgTime;
    double  sampleRate;
    double  avgCoeff;
    double  RMS;
};
/*******************************************************************************
 RMSEstimatorInit */
RMSEstimator*
RMSEstimatorInit(float avgTime, float sampleRate)
{
    RMSEstimator* rms = (RMSEstimator*) malloc(sizeof(RMSEstimator));
    rms->avgTime = avgTime;
    rms->sampleRate = sampleRate;
    rms->RMS = 1;
    rms->avgCoeff = 0.5 * (1.0 - expf( -1.0 / (rms->sampleRate * rms->avgTime)));

    return rms;
}


RMSEstimatorD*
RMSEstimatorInitD(double avgTime, double sampleRate)
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
 RMSEstimatorFlush */
Error_t
RMSEstimatorFlush(RMSEstimator* rms)
{
    if (rms)
    {
        rms->RMS = 1.0;
        return NOERR;
    }
    return NULL_PTR_ERROR;
}

Error_t
RMSEstimatorFlushD(RMSEstimatorD* rms)
{
    if (rms)
    {
        rms->RMS = 1.0;
        return NOERR;
    }
    return NULL_PTR_ERROR;
}


/*******************************************************************************
 RMSEstimatorSetAvgTime */
Error_t
RMSEstimatorSetAvgTime(RMSEstimator* rms, float avgTime)
{
    rms->avgTime = avgTime;
    rms->avgCoeff = 0.5 * (1.0 - expf( -1.0 / (rms->sampleRate * rms->avgTime)));
    return NOERR;
}

Error_t
RMSEstimatorSetAvgTimeD(RMSEstimatorD* rms, double avgTime)
{
    rms->avgTime = avgTime;
    rms->avgCoeff = 0.5 * (1.0 - expf( -1.0 / (rms->sampleRate * rms->avgTime)));
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
