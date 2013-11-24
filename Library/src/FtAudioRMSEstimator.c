//
//  FtAudioRMSEstimator.c
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#include "FtAudioRMSEstimator.h"
#include <math.h>
#include <stdlib.h>

/* FTA_RMSEstimator ***********************************************************/
struct FTA_RMSEstimator
{
    float avgTime;
    long long sampleRate;
    float avgCoeff;
    float RMS;
};


/* FTA_RMSEstimatorInit *******************************************************/
FTA_RMSEstimator*
FTA_RMSEstimatorInit(float      avgTime,
                     long long  sampleRate)
{
    FTA_RMSEstimator* rms = (FTA_RMSEstimator*) malloc(sizeof(FTA_RMSEstimator));
    rms->avgTime = avgTime;
    rms->sampleRate = sampleRate;
    rms->RMS = 1.0;
    rms->avgCoeff = 0.5 * (1.0 - exp(-1.0 / (rms->sampleRate * rms->avgTime)));
    
    return rms;
}


/* FTA_RMSEstimatorFree *******************************************************/
FTA_Error_t
FTA_RMSEstimatorFree(FTA_RMSEstimator* rms)
{
    free(rms);
    return FT_NOERR;
}


/* FTA_RMSEstimatorProcess ****************************************************/
FTA_Error_t
FTA_RMSEstimatorProcess(FTA_RMSEstimator*   rms,
                        float*              outBuffer,
                        const float*        inBuffer,
                        unsigned            n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        rms->RMS += rms->avgCoeff * ((inBuffer[i]/rms->RMS) - rms->RMS);
        outBuffer[i] = rms->RMS;
    }
    return FT_NOERR;
}

/* FTA_RMSEstimatorTick *******************************************************/
float
FTA_RMSEstimatorTick(FTA_RMSEstimator*  rms,
                     float              inSample)
{
    rms->RMS += rms->avgCoeff * ((inSample/rms->RMS) - rms->RMS);
    return rms->RMS;
}
