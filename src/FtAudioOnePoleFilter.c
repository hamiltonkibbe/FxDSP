//
//  FtAudioOnePoleFilter.c
//  Test
//
//  Created by Hamilton Kibbe on 5/26/13.
//
//

#include "FtAudioOnePoleFilter.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


/* FtAudioOnePoleFilter ********************************************************/
struct FtAudioOnePoleFilter
{
    float a0;
    float b1;
    float y1;
    float cutoff;
    float sampleRate;
};

/* FtAudioOnePoleFilterInit ***************************************************/
FtAudioOnePoleFilter*
FtAudioOnePoleFilterInit(float cutoff, float sampleRate)
{
    FtAudioOnePoleFilter *filter = (FtAudioOnePoleFilter*)malloc(sizeof(FtAudioOnePoleFilter));
    filter->a0 = 1;
    filter->b1 = 0;
    filter->y1 = 0;
    filter->sampleRate = sampleRate;
    FtAudioOnePoleFilterSetCutoff(filter, cutoff);
    return filter;
}

/* FtAudioOnePoleFilterSetCutoff **********************************************/
FtAudioError_t
FtAudioOnePoleFilterSetCutoff(FtAudioOnePoleFilter* filter, float cutoff)
{
    filter->cutoff = cutoff;
    filter->b1 = exp(2.0 * M_PI * (filter->cutoff / filter->sampleRate));
    filter->a0 = 1.0 - filter->b1;
    return FT_NOERR;
}

/* FtAudioOnePoleFilterFree ***************************************************/
FtAudioError_t
FtAudioOnePoleFilterFree(FtAudioOnePoleFilter *filter)
{
    free(filter);
    return FT_NOERR;
}


/* FtAudioOnePoleFilterProcess ************************************************/
FtAudioError_t
FtAudioOnePoleFilterProcess(FtAudioOnePoleFilter* filter,
                            float*               outBuffer,
                            const float*         inBuffer,
                            unsigned             n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        outBuffer[i] = filter->y1 = inBuffer[i] * filter->a0 + filter->y1 * filter->b1;
    }
    return FT_NOERR;
}
