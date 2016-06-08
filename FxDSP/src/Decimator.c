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

struct DecimatorD
{
    unsigned factor;
    FIRFilterD** polyphase;
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
      /*  case X16:
            n_filters = 16;
            break; */
        default:
            return NULL;
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
            decimator->polyphase[idx] = FIRFilterInit(PolyphaseCoeffs[factor][idx], 64, DIRECT);
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

DecimatorD*
DecimatorInitD(ResampleFactor_t factor)
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
        /*
        case X16:
            n_filters = 16;
            break;
        */
        default:
            return NULL;
    }

    // Allocate memory for the upsampler
    DecimatorD* decimator = (DecimatorD*)malloc(sizeof(DecimatorD));

    // Allocate memory for the polyphase array
    FIRFilterD** polyphase = (FIRFilterD**)malloc(n_filters * sizeof(FIRFilterD*));

    if (decimator && polyphase)
    {
        decimator->polyphase = polyphase;

        // Create polyphase filters
        unsigned idx;
        for(idx = 0; idx < n_filters; ++idx)
        {
            decimator->polyphase[idx] = FIRFilterInitD(PolyphaseCoeffsD[factor][idx], 64, DIRECT);
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

Error_t
DecimatorFreeD(DecimatorD* decimator)
{
    if (decimator)
    {
        if (decimator->polyphase)
        {
            for (unsigned i = 0; i < decimator->factor; ++i)
            {
                FIRFilterFreeD(decimator->polyphase[i]);
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

Error_t
DecimatorFlushD(DecimatorD* decimator)
{
    unsigned idx;
    for (idx = 0; idx < decimator->factor; ++idx)
    {
        FIRFilterFlushD(decimator->polyphase[idx]);
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
        unsigned declen = n_samples / decimator->factor;
        float temp_buf[declen];
        ClearBuffer(outBuffer, declen);

        for (unsigned filt = 0; filt < decimator->factor; ++filt)
        {
            CopyBufferStride(temp_buf, 1, inBuffer, decimator->factor, declen);
            FIRFilterProcess(decimator->polyphase[filt], temp_buf, temp_buf, declen);
            VectorVectorAdd(outBuffer, (const float*)outBuffer, temp_buf, declen);
        }
        return NOERR;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}


Error_t
DecimatorProcessD(DecimatorD*   decimator,
                  double*       outBuffer,
                  const double* inBuffer,
                  unsigned      n_samples)
{
    if (decimator && outBuffer)
    {
        unsigned declen = n_samples / decimator->factor;
        double temp_buf[declen];
        ClearBufferD(outBuffer, declen);

        for (unsigned filt = 0; filt < decimator->factor; ++filt)
        {
            CopyBufferStrideD(temp_buf, 1, inBuffer, decimator->factor, declen);
            FIRFilterProcessD(decimator->polyphase[filt], temp_buf, temp_buf, declen);
            VectorVectorAddD(outBuffer, (const double*)outBuffer, temp_buf, declen);
        }
        return NOERR;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}
