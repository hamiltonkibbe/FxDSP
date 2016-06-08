/*
 * BiquadFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "BiquadFilter.h"
#include "Dsp.h"

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

#include <stddef.h>
#include <stdlib.h>

/*******************************************************************************
 BiquadFilter */
struct BiquadFilter
{
    float b[3];     // b0, b1, b2
    float a[2];     // a1, a2
    float x[2];     //
    float y[2];
    float w[2];
};

struct BiquadFilterD
{
    double b[3];     // b0, b1, b2
    double a[2];     // a1, a2
    double  x[2];     //
    double  y[2];
    double  w[2];
};

/*******************************************************************************
 BiquadFilterInit */
BiquadFilter*
BiquadFilterInit(const float *bCoeff, const float *aCoeff)
{

    // Allocate Memory
    BiquadFilter* filter = (BiquadFilter*)malloc(sizeof(BiquadFilter));

    if (filter)
    {
        // Initialize Buffers
        CopyBuffer(filter->b, bCoeff, 3);
        CopyBuffer(filter->a, aCoeff, 2);

        ClearBuffer(filter->x, 2);
        ClearBuffer(filter->y, 2);
        ClearBuffer(filter->w, 2);
    }
    return filter;
}

BiquadFilterD*
BiquadFilterInitD(const double  *bCoeff, const double  *aCoeff)
{

    // Allocate Memory
    BiquadFilterD* filter = (BiquadFilterD*)malloc(sizeof(BiquadFilterD));

    if (filter)
    {
        // Initialize Buffers
        CopyBufferD(filter->b, bCoeff, 3);
        CopyBufferD(filter->a, aCoeff, 2);

        ClearBufferD(filter->x, 2);
        ClearBufferD(filter->y, 2);
        ClearBufferD(filter->w, 2);
    }
    return filter;
}


/*******************************************************************************
 BiquadFilterFree */
Error_t
BiquadFilterFree(BiquadFilter * filter)
{
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    return NOERR;
}

Error_t
BiquadFilterFreeD(BiquadFilterD* filter)
{
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    return NOERR;
}

/*******************************************************************************
 BiquadFilterFlush */
Error_t
BiquadFilterFlush(BiquadFilter* filter)
{
    FillBuffer(filter->x, 2, 0.0);
	FillBuffer(filter->y, 2, 0.0);
    FillBuffer(filter->w, 2, 0.0);
    return NOERR;
}

Error_t
BiquadFilterFlushD(BiquadFilterD* filter)
{
    FillBufferD(filter->x, 2, 0.0);
    FillBufferD(filter->y, 2, 0.0);
    FillBufferD(filter->w, 2, 0.0);
    return NOERR;
}


/*******************************************************************************
 BiquadFilterProcess */
Error_t
BiquadFilterProcess(BiquadFilter    *filter,
                    float           *outBuffer,
                    const float     *inBuffer,
                    unsigned        n_samples)
{

#ifdef __APPLE__
    // Use accelerate if we have it
    float coeffs[5] = {
        filter->b[0], filter->b[1], filter->b[2],
        filter->a[0], filter->a[1]
    };
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
        filter->w[0] = filter->b[1] * inBuffer[buffer_idx] - filter->a[0] * \
        buffer[buffer_idx] + filter->w[1];
        filter->w[1] = filter->b[2] * inBuffer[buffer_idx] - filter->a[1] * \
        buffer[buffer_idx];

    }

    // Write output
    CopyBuffer(outBuffer, buffer, n_samples);

#endif
    return NOERR;
}


Error_t
BiquadFilterProcessD(BiquadFilterD  *filter,
                     double         *outBuffer,
                     const double   *inBuffer,
                     unsigned       n_samples)
{

#ifdef __APPLE__
    // Use accelerate if we have it
    double coeffs[5] = {
        filter->b[0], filter->b[1], filter->b[2],
        filter->a[0], filter->a[1]
    };
    double temp_in[n_samples + 2];
    double temp_out[n_samples + 2];


    // Put filter overlaps into beginning of input and output vectors
    cblas_dcopy(2, filter->x, 1, temp_in, 1);
    cblas_dcopy(2, filter->y, 1, temp_out, 1);
    cblas_dcopy(n_samples, inBuffer, 1, (temp_in + 2), 1);

    // Process
    vDSP_deq22D(temp_in, 1, coeffs, temp_out, 1, n_samples);

    // Write overlaps to filter x and y arrays
    cblas_dcopy(2, (temp_in + n_samples), 1, filter->x, 1);
    cblas_dcopy(2, (temp_out + n_samples), 1, filter->y, 1);

    // Write output
    cblas_dcopy(n_samples, (temp_out + 2), 1, outBuffer, 1);


#else

    double buffer[n_samples];
    for (unsigned buffer_idx = 0; buffer_idx < n_samples; ++buffer_idx)
    {

        // DF-II Implementation
        buffer[buffer_idx] = filter->b[0] * inBuffer[buffer_idx] + filter->w[0];
        filter->w[0] = filter->b[1] * inBuffer[buffer_idx] - filter->a[0] * \
        buffer[buffer_idx] + filter->w[1];
        filter->w[1] = filter->b[2] * inBuffer[buffer_idx] - filter->a[1] * \
        buffer[buffer_idx];

    }

    // Write output
    CopyBufferD(outBuffer, buffer, n_samples);

#endif
    return NOERR;
}


/*******************************************************************************
 BiquadFilterTick */
float
BiquadFilterTick(BiquadFilter* filter, float in_sample)
{
    float out = filter->b[0] * in_sample + filter->w[0];
    filter->w[0] = filter->b[1] * in_sample - filter->a[0] * out + filter->w[1];
    filter->w[1] = filter->b[2] * in_sample - filter->a[1] * out;

    return out;
}

double
BiquadFilterTickD(BiquadFilterD* filter, double in_sample)
{
    double out = filter->b[0] * in_sample + filter->w[0];
    filter->w[0] = filter->b[1] * in_sample - filter->a[0] * out + filter->w[1];
    filter->w[1] = filter->b[2] * in_sample - filter->a[1] * out;

    return out;
}


/*******************************************************************************
 BiquadFilterUpdateKernel */
Error_t
BiquadFilterUpdateKernel(BiquadFilter   *filter,
                         const float    *bCoeff,
                         const float    *aCoeff)
{

    CopyBuffer(filter->b, bCoeff, 3);
    CopyBuffer(filter->a, aCoeff, 2);
    return NOERR;
}

Error_t
BiquadFilterUpdateKernelD(BiquadFilterD *filter,
                          const double  *bCoeff,
                          const double  *aCoeff)
{

    CopyBufferD(filter->b, bCoeff, 3);
    CopyBufferD(filter->a, aCoeff, 2);
    return NOERR;
}


