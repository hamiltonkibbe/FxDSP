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

// Sqrt(2)/2
#define FILT_Q (0.70710681186548)

/*******************************************************************************
 MultibandFilter */
struct MultibandFilter
{
    LRFilter*   LPA;
    LRFilter*   HPA;
    LRFilter*   LPB;
    LRFilter*   HPB;
    RBJFilter*  APF;
    float       lowCutoff;
    float       highCutoff;
    float       sampleRate;
};

struct MultibandFilterD
{
    LRFilterD*  LPA;
    LRFilterD*  HPA;
    LRFilterD*  LPB;
    LRFilterD*  HPB;
    RBJFilterD* APF;
    double      lowCutoff;
    double      highCutoff;
    double      sampleRate;
};


/*******************************************************************************
 MultibandFilterInit */

MultibandFilter*
MultibandFilterInit(float   lowCutoff,
                    float   highCutoff,
                    float  sampleRate)
{
    MultibandFilter* filter = (MultibandFilter*) malloc(sizeof(MultibandFilter));
    filter->lowCutoff = lowCutoff;
    filter->highCutoff = highCutoff;
    filter->sampleRate = sampleRate;
    filter->LPA = LRFilterInit(LOWPASS, filter->lowCutoff, FILT_Q, filter->sampleRate);
    filter->HPA = LRFilterInit(HIGHPASS, filter->lowCutoff, FILT_Q, filter->sampleRate);
    filter->LPB = LRFilterInit(LOWPASS, filter->highCutoff, FILT_Q, filter->sampleRate);
    filter->HPB = LRFilterInit(HIGHPASS, filter->highCutoff, FILT_Q, filter->sampleRate);
    filter->APF = RBJFilterInit(ALLPASS, filter->sampleRate/2.0, filter->sampleRate);
    RBJFilterSetQ(filter->APF, 0.5);
    return filter;
}


MultibandFilterD*
MultibandFilterInitD(double lowCutoff,
                     double highCutoff,
                     double sampleRate)
{
    MultibandFilterD* filter = (MultibandFilterD*) malloc(sizeof(MultibandFilterD));
    filter->lowCutoff = lowCutoff;
    filter->highCutoff = highCutoff;
    filter->sampleRate = sampleRate;
    filter->LPA = LRFilterInitD(LOWPASS, filter->lowCutoff, FILT_Q, filter->sampleRate);
    filter->HPA = LRFilterInitD(HIGHPASS, filter->lowCutoff, FILT_Q, filter->sampleRate);
    filter->LPB = LRFilterInitD(LOWPASS, filter->highCutoff, FILT_Q, filter->sampleRate);
    filter->HPB = LRFilterInitD(HIGHPASS, filter->highCutoff, FILT_Q, filter->sampleRate);
    filter->APF = RBJFilterInitD(ALLPASS, filter->sampleRate/2.0, filter->sampleRate);
    RBJFilterSetQD(filter->APF, 0.5);
    return filter;
}


/*******************************************************************************
 MultibandFilterFree */

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

Error_t
MultibandFilterFreeD(MultibandFilterD* filter)
{
    LRFilterFreeD(filter->LPA);
    LRFilterFreeD(filter->LPB);
    LRFilterFreeD(filter->HPA);
    LRFilterFreeD(filter->HPB);
    RBJFilterFreeD(filter->APF);
    if (filter)
    {
        free(filter);
        filter = NULL;
    }

    return NOERR;
}


/*******************************************************************************
 MultibandFilterFlush */

Error_t
MultibandFilterFlush(MultibandFilter* filter)
{
    LRFilterFlush(filter->LPA);
    LRFilterFlush(filter->LPB);
    LRFilterFlush(filter->HPA);
    LRFilterFlush(filter->HPB);
    RBJFilterFlush(filter->APF);

    return NOERR;
}

Error_t
MultibandFilterFlushD(MultibandFilterD* filter)
{
    LRFilterFlushD(filter->LPA);
    LRFilterFlushD(filter->LPB);
    LRFilterFlushD(filter->HPA);
    LRFilterFlushD(filter->HPB);
    RBJFilterFlushD(filter->APF);

    return NOERR;
}


/*******************************************************************************
 MultibandFilterSetLowCutoff */
Error_t
MultibandFilterSetLowCutoff(MultibandFilter* filter, float lowCutoff)
{
    filter->lowCutoff = lowCutoff;
    LRFilterSetParams(filter->LPA, LOWPASS, lowCutoff, FILT_Q);
    LRFilterSetParams(filter->HPA, HIGHPASS, lowCutoff, FILT_Q);
    return NOERR;
}

Error_t
MultibandFilterSetLowCutoffD(MultibandFilterD* filter, double lowCutoff)
{
    filter->lowCutoff = lowCutoff;
    LRFilterSetParamsD(filter->LPA, LOWPASS, lowCutoff, FILT_Q);
    LRFilterSetParamsD(filter->HPA, HIGHPASS, lowCutoff, FILT_Q);
    return NOERR;
}

/*******************************************************************************
 MultibandFilterSetHighCutoff */

Error_t
MultibandFilterSetHighCutoff(MultibandFilter* filter, float highCutoff)
{
    filter->highCutoff = highCutoff;
    LRFilterSetParams(filter->LPB, LOWPASS, highCutoff, FILT_Q);
    LRFilterSetParams(filter->HPB, HIGHPASS, highCutoff, FILT_Q);
    return NOERR;
}


Error_t
MultibandFilterSetHighCutoffD(MultibandFilterD* filter, double highCutoff)
{
    filter->highCutoff = highCutoff;
    LRFilterSetParamsD(filter->LPB, LOWPASS, highCutoff, FILT_Q);
    LRFilterSetParamsD(filter->HPB, HIGHPASS, highCutoff, FILT_Q);
    return NOERR;
}


/*******************************************************************************
 MultibandFilterUpdate */

Error_t
MultibandFilterUpdate(MultibandFilter*  filter,
                      float             lowCutoff,
                      float             highCutoff)
{
    filter->lowCutoff = lowCutoff;
    filter->highCutoff = highCutoff;
    LRFilterSetParams(filter->LPA, LOWPASS, lowCutoff, FILT_Q);
    LRFilterSetParams(filter->HPA, HIGHPASS, lowCutoff, FILT_Q);
    LRFilterSetParams(filter->LPB, LOWPASS, highCutoff, FILT_Q);
    LRFilterSetParams(filter->HPB, HIGHPASS, highCutoff, FILT_Q);
    return NOERR;
}


Error_t
MultibandFilterUpdateD(MultibandFilterD*    filter,
                       double               lowCutoff,
                       double               highCutoff)
{
    filter->lowCutoff = lowCutoff;
    filter->highCutoff = highCutoff;
    LRFilterSetParamsD(filter->LPA, LOWPASS, lowCutoff, FILT_Q);
    LRFilterSetParamsD(filter->HPA, HIGHPASS, lowCutoff, FILT_Q);
    LRFilterSetParamsD(filter->LPB, LOWPASS, highCutoff, FILT_Q);
    LRFilterSetParamsD(filter->HPB, HIGHPASS, highCutoff, FILT_Q);
    return NOERR;
}



/*******************************************************************************
 MultibandFilterProcess */

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


Error_t
MultibandFilterProcessD(MultibandFilterD*   filter,
                        double*             lowOut,
                        double*             midOut,
                        double*             highOut,
                        const double*       inBuffer,
                        unsigned            n_samples)
{
    double tempLow[n_samples];
    double tempHi[n_samples];

    LRFilterProcessD(filter->LPA, tempLow, inBuffer, n_samples);
    LRFilterProcessD(filter->HPA, tempHi, inBuffer, n_samples);

    RBJFilterProcessD(filter->APF, lowOut, tempLow, n_samples);
    LRFilterProcessD(filter->LPB, midOut, tempHi, n_samples);
    LRFilterProcessD(filter->HPB, highOut, tempHi, n_samples);

    return NOERR;
}
