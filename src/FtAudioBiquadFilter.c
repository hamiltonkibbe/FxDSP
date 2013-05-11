/*
 * FtAudioBiquadFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioBiquadFilter.h"
#include "FtAudioDsp.h"
#include "FtAudioError.h"
#include <stdlib.h>
#include <string.h>

/* FtAudioBiquadFilter ********************************************************/
struct FtAudioBiquadFilter
{
    float b[3];
    float a[3];
    float x[2];
    float y[2];
};

/* FtAudioBiquadFilterInit ****************************************************/
FtAudioBiquadFilter* FtAudioBiquadFilterInit(const float    *bCoeff, 
                                             const float    *aCoeff)
{

    // Allocate Memory
    FtAudioBiquadFilter* filter = (FtAudioBiquadFilter*)malloc(sizeof(FtAudioBiquadFilter));

    // Initialize Buffers
    memcpy(filter->b, bCoeff, 3 * sizeof(float));
    memcpy(filter->a, aCoeff, 3 * sizeof(float));
	FtAudioFillBuffer(filter->x, 2, 0.0);
	FtAudioFillBuffer(filter->y, 2, 0.0);

        
    return filter;
}


/* FtAudioBiquadFilterFree ****************************************************/
FtAudioError_t 
FtAudioBiquadFilterFree(FtAudioBiquadFilter * filter)
{
    free(filter);
    return FT_NOERR;
}


/* FtAudioBiquadFilterFlush ***************************************************/
FtAudioError_t 
FtAudioBiquadFilterFlush(FtAudioBiquadFilter* filter)
{
    FtAudioFillBuffer(filter->x, 2, 0.0);
	FtAudioFillBuffer(filter->y, 2, 0.0);
    return FT_NOERR;
}


/* FtAudioBiquadFilterProcess *************************************************/
FtAudioError_t
FtAudioBiquadFilterProcess(FtAudioBiquadFilter  *filter, 
                           float                *outBuffer, 
                           const float          *inBuffer, 
                           unsigned             n_samples)
{
#ifdef __APPLE__
    float coeffs[5] = {filter->b[0], filter->b[1], filter->b[2], filter->a[0], filter->a[1]};
    float temp_in[n_samples + 2];
    float temp_out[n_samples + 2];
    
    // Put filter overlaps into beginning of input and output vectors
    memcpy(&temp_in, filter->x, 2 * sizeof(float));
    memcpy(&temp_out filter->y, 2 * sizeof(float));
    memcpy(inbuffer, &temp_in[2], n_samples * sizeof(float));
    
    // Process
    vDSP_deq22(temp_in, 1, coeffs, temp_out, 1, n_samples);
    
    // Write overlaps to filter x and y arrays
    memcpy(filter->x, &temp_in[n_samples], 2 * sizeof(float));
    memcpy(filter->y, &temp_out[n_samples], 2 * sizeof(float));
    
    // Write output
    memcpy(outBuffer, temp_out, n_samples * sizeof(float));

#else
    float buffer[n_samples];

    for (unsigned buffer_idx = 0; buffer_idx < n_samples; ++buffer_idx)
    {
        buffer[buffer_idx] = filter->b[0] * inBuffer[buffer_idx] + 
                            filter->b[1] * filter->x[0] + 
                            filter->b[2] * filter->x[1] - 
                            filter->a[1] * filter->y[0] - 
                            filter->a[2] * filter->y[1];

        /* shift x1 to x2, sample to x1 */
        filter->x[1] = filter->x[0];
        filter->x[0] = inBuffer[buffer_idx];

        /* shift y1 to y2, result to y1 */
        filter->y[1] = filter->y[0];
        filter->y[0] = buffer[buffer_idx];
    }
#endif
    memcpy(outBuffer, buffer, n_samples * sizeof(float));
    return FT_NOERR;
}


/* FtAudioBiquadFilterUpdateKernel ********************************************/
FtAudioError_t
FtAudioBiquadFilterUpdateKernel(FtAudioBiquadFilter *filter, 
                                const float         *bCoeff, 
                                const float         *aCoeff)
{
    memcpy(filter->b, bCoeff, 3);
    memcpy(filter->a, aCoeff, 3);
    return FT_NOERR;
}