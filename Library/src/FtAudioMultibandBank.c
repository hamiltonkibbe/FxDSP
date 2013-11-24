//
//  FtAudioMultibandBank.c
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#include "FtAudioMultibandBank.h"
#include "FtAudioLinkwitzRileyFilter.h"
#include "FtAudioRBJFilter.h"
#include "FtAudioFilterTypes.h"
#include <stdlib.h>

/* FTA_MultibandFilter ********************************************************/
struct FTA_MultibandFilter
{
    FTA_LRFilter*   LPA;
    FTA_LRFilter*   HPA;
    FTA_LRFilter*   LPB;
    FTA_LRFilter*   HPB;
    FTA_RBJFilter*  APF;
    float           lowCutoff;
    float           highCutoff;
	long long       sampleRate;
};


/* FTA_MultibandFilterInit ****************************************************/
FTA_MultibandFilter*
FTA_MultibandFilterInit(float       lowCutoff,
                        float       highCutoff,
                        long long   sampleRate)
{
    FTA_MultibandFilter* filter = (FTA_MultibandFilter*) malloc(sizeof(FTA_MultibandFilter));
    filter->lowCutoff = lowCutoff;
    filter->highCutoff = highCutoff;
    filter->sampleRate = sampleRate;
    filter->LPA = FTA_LRFilterInit(LOWPASS, filter->lowCutoff, 0.7071, filter->sampleRate);
    filter->HPA = FTA_LRFilterInit(HIGHPASS, filter->lowCutoff, 0.7071, filter->sampleRate);
    filter->LPB = FTA_LRFilterInit(LOWPASS, filter->highCutoff, 0.7071, filter->sampleRate);
    filter->HPB = FTA_LRFilterInit(HIGHPASS, filter->highCutoff, 0.7071, filter->sampleRate);
    filter->APF = FTA_RBJFilterInit(ALLPASS, 0, filter->sampleRate);
    FTA_RBJFilterSetQ(filter->APF, 0.7071);
    return filter;
}


/* FTA_MultibandFilterFree ****************************************************/
FTA_Error_t
FTA_MultibandFilterFree(FTA_MultibandFilter* filter)
{
    FTA_LRFilterFree(filter->LPA);
    FTA_LRFilterFree(filter->LPB);
    FTA_LRFilterFree(filter->HPA);
    FTA_LRFilterFree(filter->HPA);
    FTA_RBJFilterFree(filter->APF);
    free(filter);
    
    return FT_NOERR;
}


/* FTA_MultibandFilterFlush ***************************************************/
FTA_Error_t
FTA_MultibandFilterFlush(FTA_MultibandFilter* filter)
{
    FTA_LRFilterFlush(filter->LPA);
    FTA_LRFilterFlush(filter->LPA);
    FTA_LRFilterFlush(filter->HPA);
    FTA_LRFilterFlush(filter->HPB);
    FTA_RBJFilterFlush(filter->APF);
    
    return FT_NOERR;
}



/* FTA_MultibandFilterProcess *************************************************/
FTA_Error_t
FTA_MultibandFilterProcess(FTA_MultibandFilter* filter,
                           float*               lowOut,
                           float*               midOut,
                           float*               highOut,
                           const float*         inBuffer,
                           unsigned             n_samples)
{
    float tempLow[n_samples];
    float tempHi[n_samples];
    
    FTA_LRFilterProcess(filter->LPA, tempLow, inBuffer, n_samples);
    FTA_LRFilterProcess(filter->HPA, tempHi, inBuffer, n_samples);
    
    FTA_RBJFilterProcess(filter->APF, lowOut, tempLow, n_samples);
    FTA_LRFilterProcess(filter->LPB, midOut, tempHi, n_samples);
    FTA_LRFilterProcess(filter->HPB, highOut, tempHi, n_samples);
    
    return FT_NOERR;
}
