//
//  FtAudioOnePoleFilter.c
//
//  Created by Hamilton Kibbe on 5/26/13.
//
//

#include "FtAudioOnePoleFilter.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


/* FTA_OnePoleFilter ********************************************************/
struct FTA_OnePoleFilter
{
    float a0;
    float b1;
    float y1;
    float cutoff;
    float sampleRate;
};

/* FTA_OnePoleFilterInit ***************************************************/
FTA_OnePoleFilter*
FTA_OnePoleFilterInit(float cutoff, float sampleRate)
{
    FTA_OnePoleFilter *filter = (FTA_OnePoleFilter*)malloc(sizeof(FTA_OnePoleFilter));
    filter->a0 = 1;
    filter->b1 = 0;
    filter->y1 = 0;
    filter->sampleRate = sampleRate;
    FTA_OnePoleFilterSetCutoff(filter, cutoff);
    return filter;
}

/* FTA_OnePoleFilterSetCutoff **********************************************/
FTA_Error_t
FTA_OnePoleFilterSetCutoff(FTA_OnePoleFilter* filter, float cutoff)
{
    filter->cutoff = cutoff;
    filter->b1 = exp(-2.0 * M_PI * (filter->cutoff / filter->sampleRate));
    filter->a0 = 1.0 - filter->b1;
    return FT_NOERR;
}

/* FTA_OnePoleFilterFree ***************************************************/
FTA_Error_t
FTA_OnePoleFilterFree(FTA_OnePoleFilter *filter)
{
    free(filter);
    return FT_NOERR;
}


/* FTA_OnePoleFilterProcess ************************************************/
inline FTA_Error_t
FTA_OnePoleFilterProcess(FTA_OnePoleFilter* filter,
                            float*               outBuffer,
                            const float*         inBuffer,
                            unsigned             n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        outBuffer[i] = filter->y1 = inBuffer[i] * filter->a0 + filter->y1 * filter->b1;
        //printf("OUT: %0.10f\n", outBuffer[i]);
    }
    return FT_NOERR;
}

/* FTA_OnePoleFilterTick ***************************************************/
inline float
FTA_OnePoleFilterTick(FTA_OnePoleFilter*  filter,
                         float                  inSample)
{
    return  filter->y1 = inSample * filter->a0 + filter->y1 * filter->b1;
}
