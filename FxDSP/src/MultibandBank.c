//
//  MultibandBank.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#include "MultibandBank.h"
#include "LinkwitzRileyFilter.h"
#include "RBJFilter.h"
#include "FilterTypes.h"
#include <stdlib.h>

/* MultibandFilter ********************************************************/
struct MultibandFilter
{
    LRFilter*   LPA;
    LRFilter*   HPA;
    LRFilter*   LPB;
    LRFilter*   HPB;
    RBJFilter*  APF;
    float           lowCutoff;
    float           highCutoff;
	long long       sampleRate;
};


/* MultibandFilterInit ****************************************************/
MultibandFilter*
MultibandFilterInit(float       lowCutoff,
                        float       highCutoff,
                        long long   sampleRate)
{
    MultibandFilter* filter = (MultibandFilter*) malloc(sizeof(MultibandFilter));
    filter->lowCutoff = lowCutoff;
    filter->highCutoff = highCutoff;
    filter->sampleRate = sampleRate;
    filter->LPA = LRFilterInit(LOWPASS, filter->lowCutoff, 0.7071, filter->sampleRate);
    filter->HPA = LRFilterInit(HIGHPASS, filter->lowCutoff, 0.7071, filter->sampleRate);
    filter->LPB = LRFilterInit(LOWPASS, filter->highCutoff, 0.7071, filter->sampleRate);
    filter->HPB = LRFilterInit(HIGHPASS, filter->highCutoff, 0.7071, filter->sampleRate);
    filter->APF = RBJFilterInit(ALLPASS, filter->sampleRate/2.0, filter->sampleRate);
    RBJFilterSetQ(filter->APF, 0.5);
    return filter;
}


/* MultibandFilterFree ****************************************************/
Error_t
MultibandFilterFree(MultibandFilter* filter)
{
    LRFilterFree(filter->LPA);
    LRFilterFree(filter->LPB);
    LRFilterFree(filter->HPA);
    LRFilterFree(filter->HPB);
    RBJFilterFree(filter->APF);
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    
    return NOERR;
}


/* MultibandFilterFlush ***************************************************/
Error_t
MultibandFilterFlush(MultibandFilter* filter)
{
    LRFilterFlush(filter->LPA);
    LRFilterFlush(filter->LPA);
    LRFilterFlush(filter->HPA);
    LRFilterFlush(filter->HPB);
    RBJFilterFlush(filter->APF);
    
    return NOERR;
}

Error_t
MultibandFilterSetLowCutoff(MultibandFilter* filter, float lowCutoff)
{
    filter->lowCutoff = lowCutoff;
    LRFilterSetParams(filter->LPA, LOWPASS, lowCutoff, 0.7071);
    LRFilterSetParams(filter->HPA, HIGHPASS, lowCutoff, 0.7071);
    return NOERR;
}

Error_t
MultibandFilterSetHighCutoff(MultibandFilter* filter, float highCutoff)
{
    filter->highCutoff = highCutoff;
    LRFilterSetParams(filter->LPB, LOWPASS, highCutoff, 0.7071);
    LRFilterSetParams(filter->HPB, HIGHPASS, highCutoff, 0.7071);
    return NOERR;
}

Error_t
MultibandFilterUpdate(MultibandFilter*  filter,
                      float             lowCutoff,
                      float             highCutoff)
{
    filter->lowCutoff = lowCutoff;
    filter->highCutoff = highCutoff;
    LRFilterSetParams(filter->LPA, LOWPASS, lowCutoff, 0.7071);
    LRFilterSetParams(filter->HPA, HIGHPASS, lowCutoff, 0.7071);
    LRFilterSetParams(filter->LPB, LOWPASS, highCutoff, 0.7071);
    LRFilterSetParams(filter->HPB, HIGHPASS, highCutoff, 0.7071);
    return NOERR;
}

/* MultibandFilterProcess *************************************************/
Error_t
MultibandFilterProcess(MultibandFilter* filter,
                           float*               lowOut,
                           float*               midOut,
                           float*               highOut,
                           const float*         inBuffer,
                           unsigned             n_samples)
{
    float tempLow[n_samples];
    float tempHi[n_samples];
    
    LRFilterProcess(filter->LPA, tempLow, inBuffer, n_samples);
    LRFilterProcess(filter->HPA, tempHi, inBuffer, n_samples);
    
    RBJFilterProcess(filter->APF, lowOut, tempLow, n_samples);
    LRFilterProcess(filter->LPB, midOut, tempHi, n_samples);
    LRFilterProcess(filter->HPB, highOut, tempHi, n_samples);
    
    return NOERR;
}
