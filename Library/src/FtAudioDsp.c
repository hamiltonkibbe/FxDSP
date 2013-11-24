/*
 * FtAudioDsp.c
 * Hamilton Kibbe
 * Copyright 2013 Hamilton Kibbe
 */

#include "FtAudioDsp.h"
#include "FtAudioUtilities.h"
#include <string.h>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif


/* FTA_FloatBufferToInt16 **************************************************/
FTA_Error_t
FTA_FloatBufferToInt16(signed short* dest, const float* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vfix16(src,1,dest,1,length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = floatToInt16(*src++);
    }
#endif
    return FT_NOERR;
}


/* FTA_Int16BufferToFloat **************************************************/
FTA_Error_t
FTA_Int16BufferToFloat(float* dest, const signed short* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vflt16(src,1,dest,1,length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = int16ToFloat(*src++);
    }
#endif
    return FT_NOERR;
}



/* FTA_FillBuffer **********************************************************/
FTA_Error_t 
FTA_FillBuffer(float     *dest, unsigned  length, float     value)
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
    return FT_NOERR;
}


/* FTA_CopyBuffer **********************************************************/
FTA_Error_t
FTA_CopyBuffer(float* dest, const float* src, unsigned length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    cblas_scopy(length, src, 1, dest, 1);
#else
    // Do it the boring way
    memcpy(dest, src, length * sizeof(float));
#endif
    return FT_NOERR;
}
    
/* FTA_BufferAdd ***********************************************************/
FTA_Error_t
FTA_BufferAdd(float      *dest, 
                 float      *in1, 
                 float      *in2, 
                 unsigned   length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vadd((const float*)in1, 1, (const float*)in2, 1, dest, 1, length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) + (*in2++);
    }
    
#endif
    return FT_NOERR;
}




/* FTA_VectorVectorMultiply ************************************************/
FTA_Error_t
FTA_VectorVectorMultiply(float       *dest, 
                            float       *in1, 
                            float       *in2, 
                            unsigned    length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vmul((const float*)in1, 1, (const float*)in2, 1, dest, 1, length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) * (*in2++);
    }
    
#endif
    return FT_NOERR;
}




/* FTA_VectorScalarMultiply ************************************************/
FTA_Error_t
FTA_VectorScalarMultiply(float       *dest, 
                            float       *in1, 
                            float       scalar, 
                            unsigned    length)
{
#ifdef __APPLE__
    // Use the Accelerate framework if we have it
    vDSP_vsmul((const float*)in1, 1, (const float*)&scalar,dest, 1, length);
    
#else
    // Otherwise do it manually
    unsigned i;
    for (i = 0; i < length; ++i)
    {
        *dest++ = (*in1++) * scalar;
    }
    
#endif
    return FT_NOERR;
}



/* FTA_Convolve ************************************************************/
FTA_Error_t
FTA_Convolve(float       *in1,
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
    FTA_FillBuffer(padded, signalLength, 0.0);
    
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
    return FT_NOERR;
}

