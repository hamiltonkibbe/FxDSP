/*
 * FIRFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FIRFilter.h"
#include "Dsp.h"
#include "Utilities.h"
#include "FFT.h"
#include <stddef.h>
#include <stdlib.h>


/* FIRFilter ***********************************************************/
struct FIRFilter
{
    float*              kernel;
    const float*        kernel_end;
    float*              overlap;
    unsigned            kernel_length;
    unsigned            overlap_length;
    ConvolutionMode_t   conv_mode;
    FFTConfig*          fft_config;
    FFTSplitComplex     fft_kernel;
    unsigned            fft_length;
};

struct FIRFilterD
{
    double*             kernel;
    const double*       kernel_end;
    double*             overlap;
    unsigned            kernel_length;
    unsigned            overlap_length;
    ConvolutionMode_t   conv_mode;
    FFTConfigD*         fft_config;
    FFTSplitComplexD    fft_kernel;
    unsigned            fft_length;
};

/* FIRFilterInit *******************************************************/
FIRFilter*
FIRFilterInit(const float*       filter_kernel,
                     unsigned           length,
                     ConvolutionMode_t  convolution_mode)
{

    // Array lengths and sizes
    unsigned kernel_length = length;                    // IN SAMPLES!
    unsigned overlap_length = kernel_length - 1;        // IN SAMPLES!



    // Allocate Memory
    FIRFilter* filter = (FIRFilter*)malloc(sizeof(FIRFilter));
    float* kernel = (float*)malloc(kernel_length * sizeof(float));
    float* overlap = (float*)malloc(overlap_length * sizeof(float));
    if (filter && kernel && overlap)
    {
        // Initialize Buffers
        CopyBuffer(kernel, filter_kernel, kernel_length);
        ClearBuffer(overlap, overlap_length);

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

    else
    {
        free(filter);
        free(kernel);
        free(overlap);
        return NULL;
    }
}


FIRFilterD*
FIRFilterInitD(const double*        filter_kernel,
               unsigned             length,
               ConvolutionMode_t    convolution_mode)
{

    // Array lengths and sizes
    unsigned kernel_length = length;                    // IN SAMPLES!
    unsigned overlap_length = kernel_length - 1;        // IN SAMPLES!



    // Allocate Memory
    FIRFilterD* filter = (FIRFilterD*)malloc(sizeof(FIRFilterD));
    double* kernel = (double*)malloc(kernel_length * sizeof(double));
    double* overlap = (double*)malloc(overlap_length * sizeof(double));
    if (filter && kernel && overlap)
    {
        // Initialize Buffers
        CopyBufferD(kernel, filter_kernel, kernel_length);
        ClearBufferD(overlap, overlap_length);

        // Set up the struct
        filter->kernel = kernel;
        filter->kernel_end = filter_kernel + (kernel_length); //- 1);
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

    else
    {
        free(filter);
        free(kernel);
        free(overlap);
        return NULL;
    }
}

/* FIRFilterFree *******************************************************/
Error_t
FIRFilterFree(FIRFilter * filter)
{
    if (filter)
    {
        if (filter->kernel)
        {
            free(filter->kernel);
            filter->kernel = NULL;
        }
        if (filter->overlap)
        {
            free(filter->overlap);
            filter->overlap = NULL;
        }

        if (filter->fft_config)
        {
            //FFTFree(filter->fft_config);
            filter->fft_config = NULL;
        }

        if (filter->fft_kernel.realp)
        {
            free(filter->fft_kernel.realp);
            filter->fft_kernel.realp = NULL;
        }
        free(filter);
        filter = NULL;
    }
    return NOERR;
}

Error_t
FIRFilterFreeD(FIRFilterD * filter)
{
    if (filter)
    {
        if (filter->kernel)
        {
            free(filter->kernel);
            filter->kernel = NULL;
        }
        if (filter->overlap)
        {
            free(filter->overlap);
            filter->overlap = NULL;
        }

        if (filter->fft_config)
        {
            FFTFreeD(filter->fft_config);
            filter->fft_config = NULL;
        }

        if (filter->fft_kernel.realp)
        {
            free(filter->fft_kernel.realp);
            filter->fft_kernel.realp = NULL;
        }

        free(filter);
        filter = NULL;
    }
    return NOERR;
}


/* FIRFilterFlush ******************************************************/
Error_t
FIRFilterFlush(FIRFilter* filter)
{
    // The only stateful part of this is the overlap buffer, so this just
    //zeros it out
    ClearBuffer(filter->overlap, filter->overlap_length);
    return NOERR;
}

Error_t
FIRFilterFlushD(FIRFilterD* filter)
{
    // The only stateful part of this is the overlap buffer, so this just
    //zeros it out
    ClearBufferD(filter->overlap, filter->overlap_length);
    return NOERR;
}


/* FIRFilterProcess ****************************************************/
Error_t
FIRFilterProcess(FIRFilter*     filter,
                 float*         outBuffer,
                 const float*   inBuffer,
                 unsigned       n_samples)
{

    if (filter)
    {
        // Do direct convolution
        if (filter->conv_mode == DIRECT)
        {
            unsigned resultLength = n_samples + (filter->kernel_length - 1);
            // Temporary buffer to store full result of filtering..
            float buffer[resultLength];

            Convolve((float*)inBuffer, n_samples,
                            filter->kernel, filter->kernel_length, buffer);

            // Add in the overlap from the last block
            VectorVectorAdd(buffer, filter->overlap, buffer, filter->overlap_length);
            CopyBuffer(filter->overlap, buffer + n_samples, filter->overlap_length);
            CopyBuffer(outBuffer, buffer, n_samples);
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
                filter->fft_length = next_pow2(n_samples + filter->kernel_length - 1);
                filter->fft_config = FFTInit(filter->fft_length);

                // fft kernel buffers
                float padded_kernel[filter->fft_length];

                // Allocate memory for filter kernel
                filter->fft_kernel.realp = (float*) malloc(filter->fft_length * sizeof(float));
                filter->fft_kernel.imagp = filter->fft_kernel.realp +(filter->fft_length / 2);

                // Write zero padded kernel to buffer
                CopyBuffer(padded_kernel, filter->kernel, filter->kernel_length);
                ClearBuffer((padded_kernel + filter->kernel_length), (filter->fft_length - filter->kernel_length));

                // Calculate FFT of filter kernel
                FFT_IR_R2C(filter->fft_config, padded_kernel, filter->fft_kernel);
            }

            // Buffer for transformed input
            float buffer[filter->fft_length];

            // Convolve
            FFTFilterConvolve(filter->fft_config, (float*)inBuffer, n_samples, filter->fft_kernel, buffer);

            // Add in the overlap from the last block
            VectorVectorAdd(buffer, filter->overlap, buffer, filter->overlap_length);
            CopyBuffer(filter->overlap, buffer + n_samples, filter->overlap_length);
            CopyBuffer(outBuffer, buffer, n_samples);

        }
        return NOERR;
    }

    else
    {
        return ERROR;
    }
}


Error_t
FIRFilterProcessD(FIRFilterD*   filter,
                 double*        outBuffer,
                 const double*  inBuffer,
                 unsigned       n_samples)
{

    if (filter)
    {

        // Do direct convolution
        if (filter->conv_mode == DIRECT)
        {
            unsigned resultLength = n_samples + (filter->kernel_length - 1);

            // Temporary buffer to store full result of filtering..
            double buffer[resultLength];

            ConvolveD((double*)inBuffer, n_samples,
                     filter->kernel, filter->kernel_length, buffer);
            // Add in the overlap from the last block
            VectorVectorAddD(buffer, filter->overlap, buffer, filter->overlap_length);
            CopyBufferD(filter->overlap, buffer + n_samples, filter->overlap_length);
            CopyBufferD(outBuffer, buffer, n_samples);
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
                filter->fft_config = FFTInitD(filter->fft_length);

                // fft kernel buffers
                double padded_kernel[filter->fft_length];

                // Allocate memory for filter kernel
                filter->fft_kernel.realp = (double*) malloc(filter->fft_length * sizeof(double));
                filter->fft_kernel.imagp = filter->fft_kernel.realp +(filter->fft_length / 2);

                // Write zero padded kernel to buffer
                CopyBufferD(padded_kernel, filter->kernel, filter->kernel_length);
                ClearBufferD((padded_kernel + filter->kernel_length), (filter->fft_length - filter->kernel_length));

                // Calculate FFT of filter kernel
                FFT_IR_R2CD(filter->fft_config, padded_kernel, filter->fft_kernel);
            }

            // Buffer for transformed input
            double buffer[filter->fft_length];

            // Convolve
            FFTFilterConvolveD(filter->fft_config, (double*)inBuffer, n_samples, filter->fft_kernel, buffer);

            // Add in the overlap from the last block
            VectorVectorAddD(buffer, filter->overlap, buffer, filter->overlap_length);
            CopyBufferD(filter->overlap, buffer + n_samples, filter->overlap_length);
            CopyBufferD(outBuffer, buffer, n_samples);

        }
        return NOERR;
    }

    else
    {
        return ERROR;
    }
}

/* FIRFilterUpdateKernel ***********************************************/
Error_t
FIRFilterUpdateKernel(FIRFilter*  filter, const float* filter_kernel)
{
    // Copy the new kernel into the filter
    CopyBuffer(filter->kernel, filter_kernel, filter->kernel_length);
    return NOERR;
}

Error_t
FIRFilterUpdateKernelD(FIRFilterD*  filter, const double* filter_kernel)
{
    // Copy the new kernel into the filter
    CopyBufferD(filter->kernel, filter_kernel, filter->kernel_length);
    return NOERR;
}
