/*
 * Dsp.c
 * Hamilton Kibbe
 * Copyright 2013 Hamilton Kibbe
 */

#include "Dsp.h"
#include "Utilities.h"
#include <string.h>
#include <float.h>
#include <math.h>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#elif defined(USE_BLAS)
#include <cblas.h>
#endif


/*******************************************************************************
 FloatBufferToInt16 */
Error_t
FloatBufferToInt16(signed short* dest, const float* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    float scale = (float)INT16_MAX;
    float temp[length];
    vDSP_vsmul(src, 1, &scale, temp, 1, length);
    vDSP_vfix16(temp,1,dest,1,length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = floatToInt16(*src++);
        dest[i + 1] = floatToInt16(*src++);
        dest[i + 2] = floatToInt16(*src++);
        dest[i + 3] = floatToInt16(*src++);
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = floatToInt16(*src++);
    }
#endif
    return NOERR;
}

/*******************************************************************************
 DoubleBufferToInt16 */
Error_t
DoubleBufferToInt16(signed short* dest, const double* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    double scale = (float)INT16_MAX;
    double temp[length];
    vDSP_vsmulD(src, 1, &scale, temp, 1, length);
    vDSP_vfix16D(temp,1,dest,1,length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = floatToInt16(*src++);
        dest[i + 1] = floatToInt16(*src++);
        dest[i + 2] = floatToInt16(*src++);
        dest[i + 3] = floatToInt16(*src++);
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = floatToInt16(*src++);
    }
#endif
    return NOERR;
}

/*******************************************************************************
 Int16BufferToFloat */
Error_t
Int16BufferToFloat(float* dest, const signed short* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    float temp[length];
    float scale = 1.0 / (float)INT16_MAX;
    vDSP_vflt16(src,1,temp,1,length);
    vDSP_vsmul(temp, 1, &scale, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = int16ToFloat(*src++);
        dest[i + 1] = int16ToFloat(*src++);
        dest[i + 2] = int16ToFloat(*src++);
        dest[i + 3] = int16ToFloat(*src++);
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = int16ToFloat(*src++);
    }
#endif
    return NOERR;
}

/*******************************************************************************
 Int16BufferToDouble */
Error_t
Int16BufferToDouble(double* dest, const signed short* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    double temp[length];
    double scale = 1.0 / (double)INT16_MAX;
    vDSP_vflt16D(src,1,temp,1,length);
    vDSP_vsmulD(temp, 1, &scale, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = int16ToFloat(*src++);
        dest[i + 1] = int16ToFloat(*src++);
        dest[i + 2] = int16ToFloat(*src++);
        dest[i + 3] = int16ToFloat(*src++);
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = int16ToFloat(*src++);
    }
#endif
    return NOERR;
}


/*******************************************************************************
 DoubleToFloat */
Error_t
DoubleToFloat(float* dest, const double* src, unsigned length)
{
#ifdef __APPLE__
    vDSP_vdpsp(src, 1, dest, 1, length);
#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = (float)src[i];
        dest[i + 1] = (float)src[i + 1];
        dest[i + 2] = (float)src[i + 2];
        dest[i + 3] = (float)src[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = (float)src[i];
    }
#endif
    return NOERR;
}


/*******************************************************************************
 Float To Double */
Error_t
FloatToDouble(double* dest, const float* src, unsigned length)
{
#ifdef __APPLE__
    vDSP_vspdp(src, 1, dest, 1, length);
#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = (double)(src[i]);
        dest[i + 1] = (double)(src[i + 1]);
        dest[i + 2] = (double)(src[i + 2]);
        dest[i + 3] = (double)(src[i + 3]);
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = (double)(src[i]);
    }
#endif
    return NOERR;
}


/*******************************************************************************
 FillBuffer */
Error_t
FillBuffer(float *dest, unsigned length, float value)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vfill(&value, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i = 0;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = value;
        dest[i + 1] = value;
        dest[i + 2] = value;
        dest[i + 3] = value;
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = value;
    }
#endif
    return NOERR;
}

/*******************************************************************************
 FillBufferD */
Error_t
FillBufferD(double *dest, unsigned length, double value)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vfillD(&value, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i = 0;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = value;
        dest[i + 1] = value;
        dest[i + 2] = value;
        dest[i + 3] = value;
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = value;
    }
#endif
    return NOERR;
}

/*******************************************************************************
 ClearBuffer */
Error_t
ClearBuffer(float *dest, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vclr(dest, 1, length);

#else
    // Otherwise do it manually. Yes this works for floats
    memset(dest, 0, length * sizeof(float));
#endif
    return NOERR;
}

/*******************************************************************************
 ClearBufferD */
Error_t
ClearBufferD(double *dest, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vclrD(dest, 1, length);

#else
    // Otherwise do it manually. Yes this works for doubles
    memset(dest, 0, length * sizeof(double));
#endif
    return NOERR;
}


/*******************************************************************************
 CopyBuffer */
Error_t
CopyBuffer(float* dest, const float* src, unsigned length)
{
    if (src != dest)
    {
#if defined(__APPLE__) || defined(USE_BLAS)
        // Use the Accelerate framework if we have it
        cblas_scopy(length, src, 1, dest, 1);
#else
        // Do it the boring way. Prefer memmove to memcpy in case src and dest
        // overlap
        memmove(dest, src, length * sizeof(float));
#endif
    }
    return NOERR;
}


/*******************************************************************************
 CopyBufferD */
Error_t
CopyBufferD(double* dest, const double* src, unsigned length)
{
    if (src != dest)
    {
#if defined(__APPLE__) || defined(USE_BLAS)
        // Use the Accelerate framework if we have it
        cblas_dcopy(length, src, 1, dest, 1);
#else
        // Do it the boring way. Prefer memmove to memcpy in case src and dest
        // overlap
        memmove(dest, src, length * sizeof(double));
#endif
    }
    return NOERR;
}


/*******************************************************************************
 CopyBufferStride */
Error_t
CopyBufferStride(float*         dest,
                 unsigned       dest_stride,
                 const float*   src,
                 unsigned       src_stride,
                 unsigned       length)
{
#if defined(__APPLE__) || defined(USE_BLAS)
    // Use the Accelerate framework if we have it
    cblas_scopy(length, src, src_stride, dest, dest_stride);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        dest[i * dest_stride] = src[i * src_stride];
    }
#endif
    return NOERR;
}


/*******************************************************************************
 CopyBufferStrideD */
Error_t
CopyBufferStrideD(double*       dest,
                  unsigned      dest_stride,
                  const double* src,
                  unsigned      src_stride,
                  unsigned      length)
{
#if defined(__APPLE__) || defined(USE_BLAS)
    // Use the Accelerate framework if we have it
    cblas_dcopy(length, src, src_stride, dest, dest_stride);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        dest[i * dest_stride] = src[i * src_stride];
    }
#endif
    return NOERR;
}


/*******************************************************************************
 SplitToInterleaved */
Error_t
SplitToInterleaved(float* dest, const float* real, const float* imag, unsigned length)
{
#if defined(__APPLE__)
    DSPSplitComplex in = {.realp = (float*)real, .imagp = (float*)imag};
    vDSP_ztoc(&in, 1, (DSPComplex*)dest, 2, length);

#elif defined(USE_BLAS)
    cblas_scopy(length, real, 1, dest, 2);
    cblas_scopy(length, imag, 1, dest + 1, 2);
#else
    unsigned i;
    unsigned i2;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        i2 = i * 2;
        dest[i2] = real[i];
        dest[i2 + 2] = real[i + 1];
        dest[i2 + 4] = real[i + 2];
        dest[i2 + 6] = real[i + 3];

        dest[i2 + 1] = imag[i];
        dest[i2 + 3] = imag[i + 1];
        dest[i2 + 5] = imag[i + 2];
        dest[i2 + 7] = imag[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        i2 = i * 2;
        dest[i2] = real[i];
        dest[i2 + 1] = imag[i];
    }
#endif
    return NOERR;
}


Error_t
SplitToInterleavedD(double* dest, const double* real, const double* imag, unsigned length)
{
#if defined(__APPLE__)
    DSPDoubleSplitComplex in = {.realp = (double*)real, .imagp = (double*)imag};
    vDSP_ztocD(&in, 1, (DSPDoubleComplex*)dest, 2, length);

#elif defined(USE_BLAS)
    cblas_dcopy(length, real, 1, dest, 2);
    cblas_dcopy(length, imag, 1, dest + 1, 2);
#else
    unsigned i;
    unsigned i2;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        i2 = i * 2;
        dest[i2] = real[i];
        dest[i2 + 2] = real[i + 1];
        dest[i2 + 4] = real[i + 2];
        dest[i2 + 6] = real[i + 3];

        dest[i2 + 1] = imag[i];
        dest[i2 + 3] = imag[i + 1];
        dest[i2 + 5] = imag[i + 2];
        dest[i2 + 7] = imag[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        i2 = i * 2;
        dest[i2] = real[i];
        dest[i2 + 1] = imag[i];
    }
#endif
    return NOERR;
}


/*******************************************************************************
 InterleavedToSplit */
Error_t
InterleavedToSplit(float*       real,
                   float*       imag,
                   const float* input,
                   unsigned     length)
{
#if defined(__APPLE__)
    DSPSplitComplex out = {.realp = real, .imagp = imag};
    vDSP_ctoz((const DSPComplex*)input, 2, &out, 1, length);

#elif defined(USE_BLAS)
    cblas_scopy(length, input, 2, real, 1);
    cblas_scopy(length, input + 1, 2, imag, 1);
#else
    unsigned i;
    unsigned i2;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        i2 = i * 2;
        real[i] = input[i2];
        real[i + 1] = input[i2 + 2];
        real[i + 2] = input[i2 + 4];
        real[i + 3] = input[i2 + 6];

        imag[i] = input[i2 + 1];
        imag[i + 1] = input[i2 + 3];
        imag[i + 2] = input[i2 + 5];
        imag[i + 3] = input[i2 + 7];
    }
    for (i = end; i < length; ++i)
    {
        i2 = i * 2;
        real[i] = input[i2];
        imag[i] = input[i2 + 1];
    }

#endif
    return NOERR;
}


Error_t
InterleavedToSplitD(double*         real,
                    double*         imag,
                    const double*   input,
                    unsigned        length)
{
#if defined(__APPLE__)
    DSPDoubleSplitComplex out = {.realp = real, .imagp = imag};
    vDSP_ctozD((const DSPDoubleComplex*)input, 2, &out, 1, length);

#elif defined(USE_BLAS)
    cblas_dcopy(length, input, 2, real, 1);
    cblas_dcopy(length, input + 1, 2, imag, 1);
#else

    unsigned i;
    unsigned i2;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        i2 = i * 2;
        real[i] = input[i2];
        real[i + 1] = input[i2 + 2];
        real[i + 2] = input[i2 + 4];
        real[i + 3] = input[i2 + 6];

        imag[i] = input[i2 + 1];
        imag[i + 1] = input[i2 + 3];
        imag[i + 2] = input[i2 + 5];
        imag[i + 3] = input[i2 + 7];
    }
    for (i = end; i < length; ++i)
    {
        i2 = i * 2;
        real[i] = input[i2];
        imag[i] = input[i2 + 1];
    }

#endif
    return NOERR;
}


/*******************************************************************************
 VectorAbs */
Error_t
VectorAbs(float *dest, const float *in, unsigned length)
{
#ifdef __APPLE__
    vDSP_vabs(in, 1, dest, 1, length);
#else
    // Otherwise do it manually
    unsigned i = 0;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = fabsf(in[i]);
        dest[i + 1] = fabsf(in[i + 1]);
        dest[i + 2] = fabsf(in[i + 2]);
        dest[i + 3] = fabsf(in[i + 3]);
    }
    for (unsigned i = end; i < length; ++i)

    {
        dest[i] = fabsf(in[i]);
    }
#endif
    return NOERR;
}


/*******************************************************************************
 VectorAbsD */
Error_t
VectorAbsD(double *dest, const double *in, unsigned length)
{
#ifdef __APPLE__
    vDSP_vabsD(in, 1, dest, 1, length);
#else
    // Otherwise do it manually
    unsigned i = 0;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = fabs(in[i]);
        dest[i + 1] = fabs(in[i + 1]);
        dest[i + 2] = fabs(in[i + 2]);
        dest[i + 3] = fabs(in[i + 3]);
    }
    for (unsigned i = end; i < length; ++i)

    {
        dest[i] = fabs(in[i]);
    }
#endif
    return NOERR;
}


/*******************************************************************************
 VectorNegate */
Error_t
VectorNegate(float          *dest,
             const float    *in,
             unsigned       length)
{
#if defined(__APPLE__)
    // Use the Accelerate framework if we have it
    vDSP_vneg(in, 1, dest, 1, length);
#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = -in[i];
        dest[i + 1] = -in[i + 1];
        dest[i + 2] = -in[i + 2];
        dest[i + 3] = -in[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = -in[i];
    }

#endif
    return NOERR;
}


/*******************************************************************************
 VectorNegateD */
Error_t
VectorNegateD(double          *dest,
              const double    *in,
              unsigned       length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vnegD(in, 1, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = -in[i];
        dest[i + 1] = -in[i + 1];
        dest[i + 2] = -in[i + 2];
        dest[i + 3] = -in[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = -in[i];
    }
#endif
    return NOERR;
}


/*******************************************************************************
 VectorSum */
float
VectorSum(const float* src, unsigned length)
{
    float res = 0.0;
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_sve(src, 1, &res, length);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        res += src[i];
    }
#endif
    return res;
}

/*******************************************************************************
 VectorSumD */
double
VectorSumD(const double* src, unsigned length)
{
    double res = 0.0;
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_sveD(src, 1, &res, length);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        res += src[i];
    }
#endif
    return res;
}


/*******************************************************************************
 VectorMax */
float
VectorMax(const float* src, unsigned length)
{
    float res = FLT_MIN;
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_maxv(src, 1, &res, length);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        if (src[i] > res)
        {
            res = src[i];
        }
    }
#endif
    return res;
}


/*******************************************************************************
 VectorMax */
double
VectorMaxD(const double* src, unsigned length)
{
    double res = DBL_MIN;
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_maxvD(src, 1, &res, length);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        if (src[i] > res)
        {
            res = src[i];
        }
    }
#endif
    return res;
}


/*******************************************************************************
 VectorMaxVI */
Error_t
VectorMaxVI(float* value, unsigned* index, const float* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_maxvi(src, 1, value, (unsigned long*)index, length);
#else
    float res = FLT_MIN;
    for (unsigned i = 0; i < length; ++i)
    {
        if (src[i] > res)
        {
            *value = res = src[i];
            *index = i;
        }
    }
#endif
    return NOERR;
}


/*******************************************************************************
 VectorMaxVID*/
Error_t
VectorMaxVID(double* value, unsigned* index, const double* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_maxviD(src, 1, value, (unsigned long*)index, length);
#else
    double res = src[0];
    for (unsigned i = 0; i < length; ++i)
    {
        if (src[i] > res)
        {
            *value = res = src[i];
            *index = i;
        }
    }
#endif
    return NOERR;
}


/*******************************************************************************
 VectorMin */
float
VectorMin(const float* src, unsigned length)
{
    float res = FLT_MAX;
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_minv(src, 1, &res, length);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        if (src[i] < res)
        {
            res = src[i];
        }
    }
#endif
    return res;
}


/*******************************************************************************
 VectorMinD */
double
VectorMinD(const double* src, unsigned length)
{
    double res = DBL_MAX;
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_minvD(src, 1, &res, length);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        if (src[i] < res)
        {
            res = src[i];
        }
    }
#endif
    return res;
}


/*******************************************************************************
 VectorMinVI */
Error_t
VectorMinVI(float* value, unsigned* index, const float* src, unsigned length)
{

#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_minvi(src, 1, value, (unsigned long*)index, length);
#else
    float res = src[0];
    for (unsigned i = 0; i < length; ++i)
    {
        if (src[i] < res)
        {
            *value = res = src[i];
            *index = i;
        }
    }
#endif
    return NOERR;
}

/*******************************************************************************
 VectorMinVID */
Error_t
VectorMinVID(double* value, unsigned* index, const double* src, unsigned length)
{

#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_minviD(src, 1, value, (unsigned long*)index, length);
#else
    double res = src[0];
    for (unsigned i = 0; i < length; ++i)
    {
        if (src[i] < res)
        {
            *value = res = src[i];
            *index = i;
        }
    }
#endif
    return NOERR;
}

/*******************************************************************************
 VectorVectorAdd */
Error_t
VectorVectorAdd(float         *dest,
                const float   *in1,
                const float   *in2,
                unsigned      length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vadd(in1, 1, in2, 1, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in1[i] + in2[i];
        dest[i + 1] = in1[i + 1] + in2[i + 1];
        dest[i + 2] = in1[i + 2] + in2[i + 2];
        dest[i + 3] = in1[i + 3] + in2[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in1[i] + in2[i];
    }

#endif
    return NOERR;
}

/*******************************************************************************
 VectorVectorAddD */
Error_t
VectorVectorAddD(double           *dest,
                 const double     *in1,
                 const double     *in2,
                 unsigned         length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vaddD(in1, 1, in2, 1, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in1[i] + in2[i];
        dest[i + 1] = in1[i + 1] + in2[i + 1];
        dest[i + 2] = in1[i + 2] + in2[i + 2];
        dest[i + 3] = in1[i + 3] + in2[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in1[i] + in2[i];
    }

#endif
    return NOERR;
}

/*******************************************************************************
 VectorVectorSub */
Error_t
VectorVectorSub(float         *dest,
                const float   *in1,
                const float   *in2,
                unsigned      length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vsub(in1, 1, in2, 1, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in2[i] - in1[i];
        dest[i + 1] = in2[i + 1] - in1[i + 1];
        dest[i + 2] = in2[i + 2] - in1[i + 2];
        dest[i + 3] = in2[i + 3] - in1[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in2[i] - in1[i];
    }

#endif
    return NOERR;
}

/*******************************************************************************
 VectorVectorSubD */
Error_t
VectorVectorSubD(double           *dest,
                 const double     *in1,
                 const double     *in2,
                 unsigned         length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vsubD(in1, 1, in2, 1, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in2[i] - in1[i];
        dest[i + 1] = in2[i + 1] - in1[i + 1];
        dest[i + 2] = in2[i + 2] - in1[i + 2];
        dest[i + 3] = in2[i + 3] - in1[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in2[i] - in1[i];
    }

#endif
    return NOERR;
}


/*******************************************************************************
 VectorScalarAdd */
Error_t
VectorScalarAdd(float           *dest,
                const float     *in1,
                const float     scalar,
                unsigned        length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vsadd(in1, 1, &scalar, dest, 1, length);
#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in1[i] + scalar;
        dest[i + 1] = in1[i + 1] + scalar;
        dest[i + 2] = in1[i + 2] + scalar;
        dest[i + 3] = in1[i + 3] + scalar;
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in1[i] + scalar;
    }

#endif
    return NOERR;
}


/*******************************************************************************
 VectorScalarAddD */
Error_t
VectorScalarAddD(double         *dest,
                 const double   *in1,
                 const double   scalar,
                 unsigned       length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vsaddD(in1, 1, &scalar, dest, 1, length);
#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in1[i] + scalar;
        dest[i + 1] = in1[i + 1] + scalar;
        dest[i + 2] = in1[i + 2] + scalar;
        dest[i + 3] = in1[i + 3] + scalar;
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in1[i] + scalar;
    }

#endif
    return NOERR;
}




/*******************************************************************************
 VectorVectorMultiply */
Error_t
VectorVectorMultiply(float          *dest,
                     const float    *in1,
                     const float    *in2,
                     unsigned       length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vmul(in1, 1, in2, 1, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in1[i] * in2[i];
        dest[i + 1] = in1[i + 1] * in2[i + 1];
        dest[i + 2] = in1[i + 2] * in2[i + 2];
        dest[i + 3] = in1[i + 3] * in2[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in1[i] * in2[i];
    }

#endif
    return NOERR;
}


/*******************************************************************************
 VectorVectorMultiplyD */
Error_t
VectorVectorMultiplyD(double        *dest,
                      const double  *in1,
                      const double  *in2,
                      unsigned      length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vmulD(in1, 1, in2, 1, dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in1[i] * in2[i];
        dest[i + 1] = in1[i + 1] * in2[i + 1];
        dest[i + 2] = in1[i + 2] * in2[i + 2];
        dest[i + 3] = in1[i + 3] * in2[i + 3];
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in1[i] * in2[i];
    }

#endif
    return NOERR;
}


/*******************************************************************************
 VectorScalarMultiply */
Error_t
VectorScalarMultiply(float          *dest,
                     const float    *in1,
                     const float    scalar,
                     unsigned       length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vsmul(in1, 1, &scalar,dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in1[i] * scalar;
        dest[i + 1] = in1[i + 1] * scalar;
        dest[i + 2] = in1[i + 2] * scalar;
        dest[i + 3] = in1[i + 3] * scalar;
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in1[i] * scalar;
    }
#endif
    return NOERR;
}

/*******************************************************************************
 VectorScalarMultiplyD */
Error_t
VectorScalarMultiplyD(double        *dest,
                      const double  *in1,
                      const double  scalar,
                      unsigned      length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vsmulD(in1, 1, &scalar,dest, 1, length);

#else
    // Otherwise do it manually
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = in1[i] * scalar;
        dest[i + 1] = in1[i + 1] * scalar;
        dest[i + 2] = in1[i + 2] * scalar;
        dest[i + 3] = in1[i + 3] * scalar;
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = in1[i] * scalar;
    }

#endif
    return NOERR;
}


/*******************************************************************************
 VectorVectorMix */

Error_t
VectorVectorMix(float        *dest,
                const float  *in1,
                const float  *scalar1,
                const float  *in2,
                const float  *scalar2,
                unsigned     length)
{
#ifdef __APPLE__
  // Use the Accelerate framework if we have it
  vDSP_vsmsma(in1, 1, scalar1, in2, 1, scalar2, dest, 1, length);
#else
  unsigned i;
  for (i = 0; i < length; ++i)
  {
    dest[i] = (in1[i] * (*scalar1)) + (in2[i] * (*scalar2));
  }
#endif
  return NOERR;
}


/*******************************************************************************
 VectorVectorMixD */

Error_t
VectorVectorMixD(double        *dest,
                 const double  *in1,
                 const double  *scalar1,
                 const double  *in2,
                 const double  *scalar2,
                 unsigned     length)
{
#ifdef __APPLE__
  // Use the Accelerate framework if we have it
  vDSP_vsmsmaD(in1, 1, scalar1, in2, 1, scalar2, dest, 1, length);
#else
  unsigned i;
  for (i = 0; i < length; ++i)
  {
    dest[i] = (in1[i] * (*scalar1)) + (in2[i] * (*scalar2));
  }
#endif
  return NOERR;
}


/*******************************************************************************
 VectorVectorSumScale */

Error_t
VectorVectorSumScale(float        *dest,
                     const float  *in1,
                     const float  *in2,
                     const float  *scalar,
                     unsigned     length)
{
#ifdef __APPLE__
// Use the Accelerate framework if we have it
vDSP_vasm(in1, 1, in2, 1, scalar, dest, 1, length);

#else
unsigned i;
for (i = 0; i < length; ++i)
{
  dest[i] = (in1[i] + in2[i]) * (*scalar);
}
#endif
return NOERR;
}


/*******************************************************************************
 VectorVectorSumScaleD */
Error_t
VectorVectorSumScaleD(double        *dest,
                      const double  *in1,
                      const double  *in2,
                      const double  *scalar,
                      unsigned     length)
{
#ifdef __APPLE__
  // Use the Accelerate framework if we have it
  vDSP_vasmD(in1, 1, in2, 1, scalar, dest, 1, length);

#else
  unsigned i;
  for (i = 0; i < length; ++i)
  {
    dest[i] = (in1[i] + in2[i]) * (*scalar);
  }
#endif
  return NOERR;
}



/*******************************************************************************
 VectorPower */
Error_t
VectorPower(float* dest, const float* in, float power, unsigned length)
{
#ifdef __APPLE__
    if (power == 2.0)
    {
        vDSP_vsq(in, 1, dest, 1, length);
    }
    else
    {
        unsigned i;
        const unsigned end = 4 * (length / 4);
        for (i = 0; i < end; i+=4)
        {
            dest[i] = powf(in[i], power);
            dest[i + 1] = powf(in[i + 1], power);
            dest[i + 2] = powf(in[i + 2], power);
            dest[i + 3] = powf(in[i + 3], power);
        }
        for (i = end; i < length; ++i)
        {
            dest[i] = powf(in[i], power);
        }
    }
#else
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = powf(in[i], power);
        dest[i + 1] = powf(in[i + 1], power);
        dest[i + 2] = powf(in[i + 2], power);
        dest[i + 3] = powf(in[i + 3], power);
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = powf(in[i], power);
    }
#endif
    return NOERR;

}


/*******************************************************************************
 VectorPowerD */
Error_t
VectorPowerD(double* dest, const double* in, double power, unsigned length)
{
#ifdef __APPLE__
    if (power == 2.0)
    {
        vDSP_vsqD(in, 1, dest, 1, length);
    }
    else
    {
        unsigned i;
        const unsigned end = 4 * (length / 4);
        for (i = 0; i < end; i+=4)
        {
            dest[i] = pow(in[i], power);
            dest[i + 1] = pow(in[i + 1], power);
            dest[i + 2] = pow(in[i + 2], power);
            dest[i + 3] = pow(in[i + 3], power);
        }
        for (i = end; i < length; ++i)
        {
            dest[i] = pow(in[i], power);
        }
    }
#else
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        dest[i] = pow(in[i], power);
        dest[i + 1] = pow(in[i + 1], power);
        dest[i + 2] = pow(in[i + 2], power);
        dest[i + 3] = pow(in[i + 3], power);
    }
    for (i = end; i < length; ++i)
    {
        dest[i] = pow(in[i], power);
    }
#endif
    return NOERR;

}


/*******************************************************************************
 Convolve */
Error_t
Convolve(float       *in1,
         unsigned    in1_length,
         float       *in2,
         unsigned    in2_length,
         float       *dest)
{

    unsigned resultLength = in1_length + (in2_length - 1);
#ifdef __APPLE__
    //Use Native vectorized convolution function if available
    float    *in2_end = in2 + (in2_length - 1);
    unsigned signalLength = (in2_length + resultLength);

    float padded[signalLength];

    //float zero = 0.0;
    ClearBuffer(padded, signalLength);

    // Pad the input signal with (filter_length - 1) zeros.
    cblas_scopy(in1_length, in1, 1, (padded + (in2_length - 1)), 1);
    vDSP_conv(padded, 1, in2_end, -1, dest, 1, resultLength, in2_length);

#else
    // Use (boring, slow) canonical implementation
    unsigned i;
    for (i = 0; i <resultLength; ++i)
    {
        unsigned kmin, kmax, k;
        dest[i] = 0;

        kmin = (i >= (in2_length - 1)) ? i - (in2_length - 1) : 0;
        kmax = (i < in1_length - 1) ? i : in1_length - 1;
        for (k = kmin; k <= kmax; k++)
        {
            dest[i] += in1[k] * in2[i - k];
        }
    }


#endif
    return NOERR;
}


/*******************************************************************************
 ConvolveD */
Error_t
ConvolveD(double    *in1,
          unsigned  in1_length,
          double    *in2,
          unsigned  in2_length,
          double    *dest)
{

    unsigned resultLength = in1_length + (in2_length - 1);

#ifdef __APPLE__
    //Use Native vectorized convolution function if available
    double    *in2_end = in2 + (in2_length - 1);
    double signalLength = (in2_length + resultLength);

    // So there's some hella weird requirement that the signal input to
    //vDSP_conv has to be larger than (result_length + filter_length - 1),
    // (the output vector length) and it has to be zero-padded. What. The. Fuck!
    double padded[(unsigned)ceil(signalLength)];

    //float zero = 0.0;
    FillBufferD(padded, signalLength, 0.0);

    // Pad the input signal with (filter_length - 1) zeros.
    cblas_dcopy(in1_length, in1, 1, (padded + (in2_length - 1)), 1);
    vDSP_convD(padded, 1, in2_end, -1, dest, 1, resultLength, in2_length);

#else
    // Use (boring, slow) canonical implementation
    unsigned i;
    for (i = 0; i <resultLength; ++i)
    {
        unsigned kmin, kmax, k;
        dest[i] = 0;

        kmin = (i >= (in2_length - 1)) ? i - (in2_length - 1) : 0;
        kmax = (i < in1_length - 1) ? i : in1_length - 1;
        for (k = kmin; k <= kmax; k++)
        {
            dest[i] += in1[k] * in2[i - k];
        }
    }


#endif
    return NOERR;
}


/*******************************************************************************
 VectorDbConvert */
Error_t
VectorDbConvert(float* dest,
                const float* in,
                unsigned length)
{
#ifdef __APPLE__
    float one = 1.0;
    vDSP_vdbcon(in, 1, &one, dest, 1, length, 1);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        dest[i] = AMP_TO_DB(in[i]);
    }
#endif
    return NOERR;
}

/*******************************************************************************
 VectorDbConvertD */
Error_t
VectorDbConvertD(double*        dest,
                 const double*  in,
                 unsigned       length)
{
#ifdef __APPLE__
    double one = 1.0;
    vDSP_vdbconD(in, 1, &one, dest, 1, length, 1);
#else
    for (unsigned i = 0; i < length; ++i)

    {
        dest[i] = AMP_TO_DBD(in[i]);
    }
#endif
    return NOERR;
}

/*******************************************************************************
 ComplexMultiply */
Error_t
ComplexMultiply(float*          re,
                float*          im,
                const float*    re1,
                const float*    im1,
                const float*    re2,
                const float*    im2,
                unsigned        length)
{
#if defined(__APPLE__)
    DSPSplitComplex in1 = {.realp = (float*)re1, .imagp = (float*)im1};
    DSPSplitComplex in2 = {.realp = (float*)re2, .imagp = (float*)im2};
    DSPSplitComplex out = {.realp = re, .imagp = im};
    vDSP_zvmul(&in1, 1, &in2, 1, &out, 1, length, 1);
#else

    for (unsigned i = 0; i < length; ++i)
    {
        float ire1 = re1[i];
        float iim1 = im1[i];
        float ire2 = re2[i];
        float iim2 = im2[i];
        re[i] = (ire1 * ire2 - iim1 * iim2);
        im[i] = (iim1 * ire2 + iim2 * ire1);
    }
#endif
    return NOERR;
}


/*******************************************************************************
 ComplexMultiplyD */
Error_t
ComplexMultiplyD(double*        re,
                 double*        im,
                 const double*  re1,
                 const double*  im1,
                 const double*  re2,
                 const double*  im2,
                 unsigned       length)
{
#if defined(__APPLE__)
    DSPDoubleSplitComplex in1 = {.realp = (double*)re1, .imagp = (double*)im1};
    DSPDoubleSplitComplex in2 = {.realp = (double*)re2, .imagp = (double*)im2};
    DSPDoubleSplitComplex out = {.realp = re, .imagp = im};
    vDSP_zvmulD(&in1, 1, &in2, 1, &out, 1, length, 1);
#else
    for (unsigned i = 0; i < length; ++i)
    {
        double ire1 = re1[i];
        double iim1 = im1[i];
        double ire2 = re2[i];
        double iim2 = im2[i];
        re[i] = (ire1 * ire2 - iim1 * iim2);
        im[i] = (iim1 * ire2 + iim2 * ire1);
    }
#endif
    return NOERR;
}


/*******************************************************************************
 VectorRectToPolar */
Error_t
VectorRectToPolar(float*        magnitude,
                  float*        phase,
                  const float*  real,
                  const float*  imaginary,
                  unsigned      length)
{
#ifdef __APPLE__
    float dest[2*length];
    SplitToInterleaved(dest, real, imaginary, length);
    vDSP_polar(dest, 2, dest, 2, length);
    InterleavedToSplit(magnitude, phase, dest, length);
#else
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        RectToPolar(real[i], imaginary[i], &magnitude[i], &phase[i]);
        RectToPolar(real[i+1], imaginary[i+1], &magnitude[i+1], &phase[i+1]);
        RectToPolar(real[i+2], imaginary[i+2], &magnitude[i+2], &phase[i+2]);
        RectToPolar(real[i+3], imaginary[i+3], &magnitude[i+3], &phase[i+3]);
    }
    for (i = end; i < length; ++i)
    {
        RectToPolar(real[i], imaginary[i], &magnitude[i], &phase[i]);
    }
#endif
    return NOERR;
}


/*******************************************************************************
 VectorRectToPolarD */
Error_t
VectorRectToPolarD(double*          magnitude,
                   double*          phase,
                   const double*    real,
                   const double*    imaginary,
                   unsigned         length)
{
#ifdef __APPLE__
    double dest[2*length];
    SplitToInterleavedD(dest, real, imaginary, length);
    vDSP_polarD(dest, 2, dest, 2, length);
    InterleavedToSplitD(magnitude, phase, dest, length);
#else
    unsigned i;
    const unsigned end = 4 * (length / 4);
    for (i = 0; i < end; i+=4)
    {
        RectToPolarD(real[i], imaginary[i], &magnitude[i], &phase[i]);
        RectToPolarD(real[i+1], imaginary[i+1], &magnitude[i+1], &phase[i+1]);
        RectToPolarD(real[i+2], imaginary[i+2], &magnitude[i+2], &phase[i+2]);
        RectToPolarD(real[i+3], imaginary[i+3], &magnitude[i+3], &phase[i+3]);
    }
    for (i = end; i < length; ++i)
    {
        RectToPolarD(real[i], imaginary[i], &magnitude[i], &phase[i]);
    }
#endif
    return NOERR;
}


/*******************************************************************************
 MeanSquare */
float
MeanSquare(const float* data, unsigned length)
{
    float result = 0.;
#ifdef __APPLE__
    vDSP_measqv(data, 1, &result, length);
#else
    float scratch[length];
    VectorPower(scratch, data, 2, length);
    result = VectorSum(scratch, length) / length;

#endif
    return result;
}

/*******************************************************************************
 MeanSquareD */
double
MeanSquareD(const double* data, unsigned length)
{
    double result = 0.;
#ifdef __APPLE__
    vDSP_measqvD(data, 1, &result, length);
#else
    double scratch[length];
    VectorPowerD(scratch, data, 2, length);
    result = VectorSumD(scratch, length) / length;
#endif
    return result;
}
