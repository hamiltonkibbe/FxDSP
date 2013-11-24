/*
 * FtAudioBiquadFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FTABiquadFilter.h"
#include "FtAudioDsp.h"
#include <stdlib.h>


#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif


/* FTA_BiquadFilter ********************************************************/
struct FTA_BiquadFilter
{
    float b[3];     // b0, b1, b2
    float a[2];     // a1, a2
    float x[2];     // 
    float y[2];
    float w[2];
};

/* FTA_BiquadFilterInit ****************************************************/
FTA_BiquadFilter* FTA_BiquadFilterInit(const float    *bCoeff, 
                                             const float    *aCoeff)
{

    // Allocate Memory
    FTA_BiquadFilter* filter = (FTA_BiquadFilter*)malloc(sizeof(FTA_BiquadFilter));

    // Initialize Buffers
    FTA_CopyBuffer(filter->b, bCoeff, 3 * sizeof(float));
    FTA_CopyBuffer(filter->a, aCoeff, 2 * sizeof(float));

	FTA_FillBuffer(filter->x, 2, 0.0);
	FTA_FillBuffer(filter->y, 2, 0.0);
    FTA_FillBuffer(filter->w, 2, 0.0);

        
    return filter;
}


/* FTA_BiquadFilterFree ****************************************************/
FTA_Error_t 
FTA_BiquadFilterFree(FTA_BiquadFilter * filter)
{
    free(filter);
    return FT_NOERR;
}


/* FTA_BiquadFilterFlush ***************************************************/
FTA_Error_t 
FTA_BiquadFilterFlush(FTA_BiquadFilter* filter)
{
    FTA_FillBuffer(filter->x, 2, 0.0);
	FTA_FillBuffer(filter->y, 2, 0.0);
    FTA_FillBuffer(filter->w, 2, 0.0);
    return FT_NOERR;
}


/* FTA_BiquadFilterProcess *************************************************/
FTA_Error_t
FTA_BiquadFilterProcess(FTA_BiquadFilter  *filter, 
                           float                *outBuffer, 
                           const float          *inBuffer, 
                           unsigned             n_samples)
{
    
#ifdef __APPLE__
    // Use accelerate if we have it
    float coeffs[5] = {filter->b[0], filter->b[1], filter->b[2], filter->a[0], filter->a[1]};
    float temp_in[n_samples + 2];
    float temp_out[n_samples + 2];

    
    // Put filter overlaps into beginning of input and output vectors
    cblas_scopy(2, filter->x, 1, temp_in, 1);
    cblas_scopy(2, filter->y, 1, temp_out, 1);
    cblas_scopy(n_samples, inBuffer, 1, (temp_in + 2), 1);

    // Process
    vDSP_deq22(temp_in, 1, coeffs, temp_out, 1, n_samples);
    
    // Write overlaps to filter x and y arrays
    cblas_scopy(2, (temp_in + n_samples), 1, filter->x, 1);
    cblas_scopy(2, (temp_out + n_samples), 1, filter->y, 1);

    // Write output
    cblas_scopy(n_samples, (temp_out + 2), 1, outBuffer, 1);


#else
    
    float buffer[n_samples];
    for (unsigned buffer_idx = 0; buffer_idx < n_samples; ++buffer_idx)
    {
        
        // DF-II Implementation
        buffer[buffer_idx] = filter->b[0] * inBuffer[buffer_idx] + filter->w[0];
        filter->w[0] = filter->b[1] * inBuffer[buffer_idx] - filter->a[0] * buffer[buffer_idx] + filter->w[1];
        filter->w[1] = filter->b[2] * inBuffer[buffer_idx] - filter->a[1] * buffer[buffer_idx];
        
    }
    
    // Write output
    FTA_CopyBuffer(outBuffer, buffer, n_samples);
    
#endif
    return FT_NOERR;
}


/* FTA_BiquadFilterUpdateKernel ********************************************/
FTA_Error_t
FTA_BiquadFilterUpdateKernel(FTA_BiquadFilter *filter, 
                                const float         *bCoeff, 
                                const float         *aCoeff)
{

    FTA_CopyBuffer(filter->b, bCoeff, 3);
    FTA_CopyBuffer(filter->a, aCoeff, 2); 
    return FT_NOERR;
}