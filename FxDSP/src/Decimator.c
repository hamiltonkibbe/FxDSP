//
//  Decimator.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/10/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "Decimator.h"
#include "FIRFilter.h"
#include "Dsp.h"
#include <stddef.h>
#include <stdlib.h>



/* Upsampler **********************************************************/
struct Decimator
{
    unsigned factor;
    FIRFilter** polyphase;
};


/* DecimatorInit *******************************************************/
Decimator*
DecimatorInit(ResampleFactor_t factor)
{
    unsigned n_filters = 1;
    switch(factor)
    {
        case X2:
            n_filters = 2;
            break;
        case X4:
            n_filters = 4;
            break;
        case X8:
            n_filters = 8;
            break;
        case X16:
            n_filters = 16;
            break;
        default:
            break;
    }
    
    // Allocate memory for the upsampler
    Decimator* decimator = (Decimator*)malloc(sizeof(Decimator));
    
    // Allocate memory for the polyphase array
    FIRFilter** polyphase = (FIRFilter**)malloc(n_filters * sizeof(FIRFilter*));
    
    if (decimator && polyphase)
    {
        decimator->polyphase = polyphase;
        
        // Create polyphase filters
        unsigned idx;
        for(idx = 0; idx < n_filters; ++idx)
        {
            decimator->polyphase[idx] = FIRFilterInit(PolyphaseCoeffs[factor][idx], 64, BEST);
        }
        
        // Add factor
        decimator->factor = n_filters;
        
        return decimator;
    }
    else
    {
        if (polyphase)
        {
            free(polyphase);
        }
        if (decimator)
        {
            free(decimator);
        }
        return NULL;
    }
}


/* *****************************************************************************
 DecimatorFree */
Error_t
DecimatorFree(Decimator* decimator)
{
    if (decimator)
    {
        if (decimator->polyphase)
        {
            for (unsigned i = 0; i < decimator->factor; ++i)
            {
                FIRFilterFree(decimator->polyphase[i]);
            }
            free(decimator->polyphase);
        }
        free(decimator);
    }
    return NOERR;
}


/* *****************************************************************************
 DecimatorFlush */
Error_t
DecimatorFlush(Decimator* decimator)
{
    unsigned idx;
    for (idx = 0; idx < decimator->factor; ++idx)
    {
        FIRFilterFlush(decimator->polyphase[idx]);
    }
    return NOERR;
}


/* *****************************************************************************
 DecimatorProcess */
Error_t
DecimatorProcess(Decimator      *decimator,
                 float          *outBuffer,
                 const float    *inBuffer,
                 unsigned       n_samples)
{
    if (decimator && outBuffer)
    {
        unsigned sampleIdx;
        unsigned filterIdx;
        unsigned offset;
        
        for (sampleIdx = 0; sampleIdx < n_samples; ++sampleIdx)
        {
            offset = decimator->factor * sampleIdx;
            for(filterIdx = 0; filterIdx < decimator->factor; ++filterIdx)
            {
                FIRFilterProcess(decimator->polyphase[filterIdx],
                                 &outBuffer[filterIdx + offset],
                                 &inBuffer[sampleIdx], 1);
            }
        }
        VectorScalarMultiply(outBuffer, (const float*)outBuffer,
                             decimator->factor, n_samples * decimator->factor);
        return NOERR;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}








