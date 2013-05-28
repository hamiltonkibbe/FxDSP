/*
 * FtAudioFIRFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioFIRFilter.h"
#include "FtAudioDsp.h"
#include "FtAudioUtilities.h"

#include <stdlib.h>



/* FtAudioFIRFilter ***********************************************************/
struct FtAudioFIRFilter
{
    float * kernel;
    const float * kernel_end;
    float * overlap;
    unsigned kernel_length;
    unsigned overlap_length;
};


/* FtAudioFIRFilterInit *******************************************************/
FtAudioFIRFilter* 
FtAudioFIRFilterInit(const float*   filter_kernel,
                     unsigned		length)
{

    // Array lengths and sizes
    unsigned kernel_length = length;            // IN SAMPLES!
    unsigned overlap_length = kernel_length - 1;        // IN SAMPLES!

    // Allocate Memory
    FtAudioFIRFilter* filter = (FtAudioFIRFilter*)malloc(sizeof(FtAudioFIRFilter));
    float* kernel = (float*)malloc(kernel_length * sizeof(float));
    float* overlap = (float*)malloc(overlap_length * sizeof(float));

    // Initialize Buffers
    FtAudioCopyBuffer(kernel, filter_kernel, kernel_length);
    
    //float zero = 0.0
    //vDSP_vfill(&zero, overlap, 1, overlap_length);
    FtAudioFillBuffer(overlap, overlap_length, 0.0);

    // Set up the struct
    filter->kernel = kernel;
    filter->kernel_end = filter_kernel + (kernel_length - 1);
    filter->overlap = overlap;
    filter->kernel_length = kernel_length;
    filter->overlap_length = overlap_length;
    return filter;
}


/* FtAudioFIRFilterFree *******************************************************/
FtAudioError_t 
FtAudioFIRFilterFree(FtAudioFIRFilter * filter)
{
    free(filter->kernel);
    free(filter->overlap);
    free(filter);
    return FT_NOERR;
}

/* FtAudioFIRFilterFlush ******************************************************/
FtAudioError_t
FtAudioFIRFilterFlush(FtAudioFIRFilter* filter)
{
    // The only stateful part of this is the overlap buffer, so this just 
    //zeros it out
    FtAudioFillBuffer(filter->overlap, filter->overlap_length, 0.0);
    return FT_NOERR;
}


/* FtAudioFIRFilterProcess ****************************************************/
FtAudioError_t
FtAudioFIRFilterProcess(FtAudioFIRFilter* filter,
                        float*  outBuffer, 
                        const float* inBuffer, 
                        unsigned n_samples)
{

    unsigned resultLength = n_samples + (filter->kernel_length - 1);
    
    // Temporary buffer to store full result of filtering..
    float buffer[resultLength];
    
    FtAudioConvolve((float*)inBuffer, n_samples, 
                    filter->kernel, filter->kernel_length, buffer);
    
    // Add in the overlap from the last block
    FtAudioBufferAdd(buffer, filter->overlap, buffer, filter->overlap_length);

                     
    // Save the overlap from this block
    FtAudioCopyBuffer(filter->overlap, buffer + n_samples, filter->overlap_length);
    
    // write output
    FtAudioCopyBuffer(outBuffer, buffer, n_samples);
    return FT_NOERR;
}


/* FtAudioFIRFilterUpdateKernel ***********************************************/
FtAudioError_t
FtAudioFIRFilterUpdateKernel(FtAudioFIRFilter*  filter, const float* filter_kernel)
{
    // Copy the new kernel into the filter
    FtAudioCopyBuffer(filter->kernel, filter_kernel, filter->kernel_length);
    return FT_NOERR;
}

