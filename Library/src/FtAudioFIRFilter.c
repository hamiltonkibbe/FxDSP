/*
 * FtAudioFIRFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioFIRFilter.h"
#include "FtAudioDsp.h"
#include "FtAudioUtilities.h"
#include "FtAudioFFT.h"
#include <stdlib.h>


/* FTA_FIRFilter ***********************************************************/
struct FTA_FIRFilter
{
    float*              kernel;
    const float*        kernel_end;
    float*              overlap;
    unsigned            kernel_length;
    unsigned            overlap_length;
    ConvolutionMode_t   conv_mode;
    FTA_FFTConfig*   fft_config;
    DSPSplitComplex     fft_kernel;
    unsigned            fft_length;
};


/* FTA_FIRFilterInit *******************************************************/
FTA_FIRFilter* 
FTA_FIRFilterInit(const float*       filter_kernel,
                     unsigned           length,
                     ConvolutionMode_t  convolution_mode)
{

    // Array lengths and sizes
    unsigned kernel_length = length;                    // IN SAMPLES!
    unsigned overlap_length = kernel_length - 1;        // IN SAMPLES!
    
    
    
    // Allocate Memory
    FTA_FIRFilter* filter = (FTA_FIRFilter*)malloc(sizeof(FTA_FIRFilter));
    float* kernel = (float*)malloc(kernel_length * sizeof(float));
    float* overlap = (float*)malloc(overlap_length * sizeof(float));
   
    // Initialize Buffers
    FTA_CopyBuffer(kernel, filter_kernel, kernel_length);
    FTA_FillBuffer(overlap, overlap_length, 0.0);

    // Set up the struct
    filter->kernel = kernel;
    filter->kernel_end = filter_kernel + (kernel_length - 1);
    filter->overlap = overlap;
    filter->kernel_length = kernel_length;
    filter->overlap_length = overlap_length;
    filter->fft_config = NULL;
    filter->fft_kernel.realp = NULL;
    filter->fft_kernel.imagp = NULL;
    
    if (((convolution_mode == BEST) &&
         (kernel_length < USE_FFT_CONVOLUTION_LENGTH)) ||
        (convolution_mode == DIRECT))
    {
        filter->conv_mode = DIRECT;
    }

    else
    {
        filter->conv_mode = FFT;
    }
    
    return filter;
}


/* FTA_FIRFilterFree *******************************************************/
FTA_Error_t 
FTA_FIRFilterFree(FTA_FIRFilter * filter)
{
    free(filter->kernel);
    free(filter->overlap);
    if (filter->fft_config)
        FTA_FFTFree(filter->fft_config);
    if (filter->fft_kernel.realp)
        free(filter->fft_kernel.realp);
    free(filter);
    return FT_NOERR;
}

/* FTA_FIRFilterFlush ******************************************************/
FTA_Error_t
FTA_FIRFilterFlush(FTA_FIRFilter* filter)
{
    // The only stateful part of this is the overlap buffer, so this just 
    //zeros it out
    FTA_FillBuffer(filter->overlap, filter->overlap_length, 0.0);
    return FT_NOERR;
}


/* FTA_FIRFilterProcess ****************************************************/
FTA_Error_t
FTA_FIRFilterProcess(FTA_FIRFilter* filter,
                        float*  outBuffer, 
                        const float* inBuffer, 
                        unsigned n_samples)
{

    // Do direct convolution
    if (filter->conv_mode == DIRECT)
    {
        unsigned resultLength = n_samples + (filter->kernel_length - 1);
    
        // Temporary buffer to store full result of filtering..
        float buffer[resultLength];
    
        FTA_Convolve((float*)inBuffer, n_samples, 
                        filter->kernel, filter->kernel_length, buffer);
        // Add in the overlap from the last block
        FTA_BufferAdd(buffer, filter->overlap, buffer, filter->overlap_length);
        FTA_CopyBuffer(filter->overlap, buffer + n_samples, filter->overlap_length);
        FTA_CopyBuffer(outBuffer, buffer, n_samples);
    }
    
    // Otherwise do FFT Convolution
    else
    {
        // Configure the FFT on the first run, that way we can figure out how
        // long the input blocks are going to be. This makes the filter more 
        // complicated internally in order to make the convolution transparent.
        // Calculate length of FFT
        if(filter->fft_config == 0)
        {
            // Calculate FFT Length
            filter->fft_length = next_pow2(n_samples + filter->kernel_length - 1); //2 * next_pow2(n_samples + filter->kernel_length - 1);
            filter->fft_config = FTA_FFTInit(filter->fft_length);
        
            // fft kernel buffers
            float padded_kernel[filter->fft_length];
        
            // Allocate memory for filter kernel
            filter->fft_kernel.realp = (float*) malloc(filter->fft_length * sizeof(float));
            filter->fft_kernel.imagp = filter->fft_kernel.realp +(filter->fft_length / 2);
        
            // Write zero padded kernel to buffer
            FTA_CopyBuffer(padded_kernel, filter->kernel, filter->kernel_length);
            FTA_FillBuffer((padded_kernel + filter->kernel_length), (filter->fft_length - filter->kernel_length), 0.0);
            
            // Calculate FFT of filter kernel
            FTA_FFTForwardSplit(filter->fft_config, (DSPComplex*)padded_kernel, &filter->fft_kernel);
        }
        
        // Buffer for transformed input
        float buffer[filter->fft_length];
        //FTA_FillBuffer(buffer, filter->fft_length, 0.0);
        //FTA_CopyBuffer(buffer, inBuffer, n_samples);
        
        // Convolve
        FTA_FFTFilterConvolve(filter->fft_config, (float*)inBuffer, n_samples, filter->fft_kernel, buffer);

        // Add in the overlap from the last block
        FTA_BufferAdd(buffer, filter->overlap, buffer, filter->overlap_length);
        FTA_CopyBuffer(filter->overlap, buffer + n_samples, filter->overlap_length);
        FTA_CopyBuffer(outBuffer, buffer, n_samples);
        
    }
        
    return FT_NOERR;
}


/* FTA_FIRFilterUpdateKernel ***********************************************/
FTA_Error_t
FTA_FIRFilterUpdateKernel(FTA_FIRFilter*  filter, const float* filter_kernel)
{
    // Copy the new kernel into the filter
    FTA_CopyBuffer(filter->kernel, filter_kernel, filter->kernel_length);
    return FT_NOERR;
}

