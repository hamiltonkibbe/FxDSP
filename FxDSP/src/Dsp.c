/*
 * Dsp.c
 * Hamilton Kibbe
 * Copyright 2013 Hamilton Kibbe
 */

#include "Dsp.h"
#include <string.h>
#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = floatToInt16(*src++);
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = floatToInt16(*src++);
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
    float scale = -1.0 / (float)INT16_MIN;
    vDSP_vflt16(src,1,temp,1,length);
    vDSP_vsmul(temp, 1, &scale, dest, 1, length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = int16ToFloat(*src++);
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
    double scale = -1.0 / (double)INT16_MIN;
    vDSP_vflt16D(src,1,temp,1,length);
    vDSP_vsmulD(temp, 1, &scale, dest, 1, length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = int16ToFloat(*src++);
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
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = value;
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
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = value;
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
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    cblas_scopy(length, src, 1, dest, 1);
#else
    // Do it the boring way
    memcpy(dest, src, length * sizeof(float));
#endif
    return NOERR;
}


/*******************************************************************************
 CopyBufferD */
Error_t
CopyBufferD(double* dest, const double* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    cblas_dcopy(length, src, 1, dest, 1);
#else
    // Do it the boring way
    memcpy(dest, src, length * sizeof(double));
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
    for (unsigned i = 0; i < length; ++i)
        
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
    for (unsigned i = 0; i < length; ++i)
        
    {
        dest[i] = fabsf(in[i]);
    }
#endif
    return NOERR;
}




/*******************************************************************************
 VectorNegate */
Error_t
VectorNegate(float          *dest,
             const float    *in1,
             unsigned       length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vneg(in1, 1, dest, 1, length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = -(*in1++);
    }
    
#endif
    return NOERR;
}

/*******************************************************************************
 VectorNegateD */
Error_t
VectorNegateD(double          *dest,
              const double    *in1,
              unsigned       length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vnegD(in1, 1, dest, 1, length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = -(*in1++);
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) + (*in2++);
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) + (*in2++);
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) + scalar;
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) + scalar;
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) * (*in2++);
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) * (*in2++);
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) * scalar;
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
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) * scalar;
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
    
    // So there's some hella weird requirement that the signal input to
    //vDSP_conv has to be larger than (result_length + filter_length - 1),
    // (the output vector length) and it has to be zero-padded. What. The. Fuck!
    float padded[signalLength];
    
    //float zero = 0.0;
    FillBuffer(padded, signalLength, 0.0);
    
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
 VectorPower */
Error_t
VectorPower(float* dest, const float* in, float power, unsigned length)
{
#ifdef __APPLE__
    if (power == 2)
    {
        vDSP_vsq(in, 1, dest, 1, length);
    }
    
    else
    {
        for (unsigned i = 0; i < length; ++i)
        {
            dest[i] = powf(in[i], power);
        }
    }
#else
    for (unsigned i = 0; i < length; ++i)
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
    if (power == 2)
    {
        vDSP_vsqD(in, 1, dest, 1, length);
    }
    
    else
    {
        for (unsigned i = 0; i < length; ++i)
        {
            dest[i] = pow(in[i], power);
        }
    }
#else
    for (unsigned i = 0; i < length; ++i)
    {
        dest[i] = pow(in[i], power);
    }
#endif
    return NOERR;
    
}

/*******************************************************************************
 VectorDbConvert */
Error_t
VectorDbConvert(float* dest,
                const float* in1,
                unsigned amplitude_flag,
                unsigned length)
{
#ifdef __APPLE__
    float one = 1.0;
    vDSP_vdbcon(in1, 1, &one, dest, 1, length, amplitude_flag);
#else
    float scale = 10.0 + 10 * amplitude_flag;
    for (unsigned i = 0; i < length; ++i)
        
    {
        dest[i] = scale * log10f(in[i]);
    }
#endif
    return NOERR;
}
