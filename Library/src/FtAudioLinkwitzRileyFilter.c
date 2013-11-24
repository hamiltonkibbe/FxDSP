//
//  FtAudioLinkwitzRileyFilter.c
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#include "FtAudioLinkwitzRileyFilter.h"
#include "FtAudioRBJFilter.h"
#include <stdlib.h>

/* FTA_LRFilter ***************************************************************/
struct FTA_LRFilter
{
    FTA_RBJFilter*  filterA;
    FTA_RBJFilter*  filterB;
    Filter_t        type;
    float           cutoff;
    float           Q;
    long long       sampleRate;
};


/* FTA_LRFilterInit ***********************************************************/
FTA_LRFilter*
FTA_LRFilterInit(Filter_t 	type,
                 float 		cutoff,
                 float      Q,
                 long long 	sampleRate)
{
    FTA_LRFilter *filter = (FTA_LRFilter*) malloc(sizeof(FTA_LRFilter));
    filter->type = type;
    filter->cutoff = cutoff;
    filter->Q = 0.7071;
    filter->sampleRate = sampleRate;
    filter->filterA = FTA_RBJFilterInit(filter->type, filter->cutoff, filter->sampleRate);
    filter->filterB = FTA_RBJFilterInit(filter->type, filter->cutoff, filter->sampleRate);
    FTA_RBJFilterSetQ(filter->filterA, filter->Q);
    FTA_RBJFilterSetQ(filter->filterB, filter->Q);
    return filter;
}


/* FTA_LRFilterFree ***********************************************************/
FTA_Error_t
FTA_LRFilterFree(FTA_LRFilter* 	filter)
{
    FTA_RBJFilterFree(filter->filterA);
    FTA_RBJFilterFree(filter->filterA);
    free(filter);
    
    return FT_NOERR;
}

/* FTA_LRFilterFlush **********************************************************/
FTA_Error_t
FTA_LRFilterFlush(FTA_LRFilter* filter)
{
    FTA_RBJFilterFlush(filter->filterA);
    FTA_RBJFilterFlush(filter->filterB);
    
    return FT_NOERR;
}

/* FTA_LRFilterSetParams ******************************************************/
FTA_Error_t
FTA_LRFilterSetParams(FTA_LRFilter* filter,
                      Filter_t  type,
                      float     cutoff,
                      float     Q)
{
    filter->type = type;
    filter->cutoff = cutoff;
    filter->Q = Q;
    FTA_RBJFilterSetParams(filter->filterA, filter->type, filter->cutoff, filter->Q);
    FTA_RBJFilterSetParams(filter->filterB, filter->type, filter->cutoff, filter->Q);
    
    return FT_NOERR;
}



/* FTA_LRFilterProcess ********************************************************/
FTA_Error_t
FTA_LRFilterProcess(FTA_LRFilter*   filter,
                    float*          outBuffer,
                    const float* 	inBuffer,
                    unsigned 		n_samples)

{
    float tempBuffer[n_samples];
    FTA_RBJFilterProcess(filter->filterA, tempBuffer, inBuffer, n_samples);
    FTA_RBJFilterProcess(filter->filterB, outBuffer, tempBuffer, n_samples);
    return FT_NOERR;
}
