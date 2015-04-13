//
//  FFT.c
//  Test
//
//  Created by Hamilton Kibbe on 5/27/13.
//
//


#include "FFT.h"
#include "Dsp.h"
#include "Utilities.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>



#ifdef USE_FFTW
typedef struct {
    fftwf_plan forward_plan;
    fftwf_plan inverse_plan;
    fftwf_plan forward_cplan;
    fftwf_plan inverse_cplan;
} FFT_SETUP;

typedef struct {
    fftw_plan forward_plan;
    fftw_plan inverse_plan;
    fftw_plan forward_cplan;
    fftw_plan inverse_cplan;
} FFT_SETUP_D;

#elif defined(__APPLE__)

typedef FFTSetup FFT_SETUP;
typedef FFTSetupD FFT_SETUP_D;

#else

typedef struct {} FFT_SETUP;
typedef struct {} FFT_SETUP_D;
#endif




struct FFTConfig
{
    unsigned        length;
    float           scale;
    float           log2n;
    FFTSplitComplex split;
    FFTSplitComplex split2;
    FFT_SETUP        setup;
};

struct FFTConfigD
{
    unsigned                length;
    double                  scale;
    double                  log2n;
    FFTSplitComplexD        split;
    FFTSplitComplexD        split2;
    FFT_SETUP_D             setup;
};


#if defined(USE_FFTW) || defined(__APPLE__)

FFTConfig*
FFTInit(unsigned length)
{
    FFTConfig* fft = (FFTConfig*)malloc(sizeof(FFTConfig));
    float* split_realp = (float*)malloc(length * sizeof(float));
    float* split2_realp = (float*)malloc(length * sizeof(float));
    
    if (fft && split_realp && split2_realp)
    {
        fft->length = length;
        fft->scale = 1.0 / (fft->length);
        fft->log2n = log2f(fft->length);
        
        // Store these consecutively in memory
        fft->split.realp = split_realp;
        fft->split2.realp = split2_realp;
        fft->split.imagp = fft->split.realp + (fft->length / 2);
        fft->split2.imagp = fft->split2.realp + (fft->length / 2);
        
#ifdef USE_FFTW
        fftwf_complex* c = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * length);
        float* r = (float*) fftwf_malloc(sizeof(float) * length);
        fft->setup.forward_plan = fftwf_plan_dft_r2c_1d(length, r, c, FFTW_MEASURE | FFTW_UNALIGNED);
        fft->setup.inverse_plan = fftwf_plan_dft_c2r_1d(length, c, r, FFTW_MEASURE | FFTW_UNALIGNED);
        fftwf_free(r);
        fftwf_free(c);
#elif defined(__APPLE__)
        fft->setup = vDSP_create_fftsetup(fft->log2n, FFT_RADIX2);
#endif
        ClearBuffer(split_realp, fft->length);
        ClearBuffer(split2_realp, fft->length);
        
        return fft;
    }
    
    else
    {
        // Cleanup
        if (fft)
            free(fft);
        if (split_realp)
            free(split_realp);
        if (split2_realp)
            free(split2_realp);
        return NULL;
    }
}



FFTConfigD*
FFTInitD(unsigned length)
{
    FFTConfigD* fft = (FFTConfigD*)malloc(sizeof(FFTConfigD));
    double* split_realp = (double*)malloc(length * sizeof(double));
    double* split2_realp = (double*)malloc(length * sizeof(double));
    
    if (fft && split_realp && split2_realp)
    {
        fft->length = length;
        fft->scale = 1.0 / (fft->length);
        fft->log2n = log2f(fft->length);
        
        // Store these consecutively in memory
        fft->split.realp = split_realp;
        fft->split2.realp = split2_realp;
        fft->split.imagp = fft->split.realp + (fft->length / 2);
        fft->split2.imagp = fft->split2.realp + (fft->length / 2);
        
#ifdef USE_FFTW
        fftw_complex* c = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * length);
        double* r = (double*) fftw_malloc(sizeof(double) * length);
        fft->setup.forward_plan = fftw_plan_dft_r2c_1d(length, r, c, FFTW_MEASURE | FFTW_UNALIGNED);
        fft->setup.inverse_plan = fftw_plan_dft_c2r_1d(length, c, r, FFTW_MEASURE | FFTW_UNALIGNED);
        fftw_free(r);
        fftw_free(c);
#elif defined(__APPLE__)
        fft->setup = vDSP_create_fftsetupD(fft->log2n, FFT_RADIX2);
#endif
        ClearBufferD(split_realp, fft->length);
        ClearBufferD(split2_realp, fft->length);
        
        return fft;
    }
    
    else
    {
        // Cleanup
        if (fft)
            free(fft);
        if (split_realp)
            free(split_realp);
        if (split2_realp)
            free(split2_realp);
        return NULL;
    }
}


Error_t
FFTFree(FFTConfig* fft)
{
    if (fft)
    {
        if (fft->split.realp)
        {
            free(fft->split.realp);
            fft->split2.realp = NULL;
        }
        if (fft->split2.realp)
        {
            free(fft->split2.realp);
            fft->split2.realp = NULL;
        }

#ifdef USE_FFTW
        fftwf_destroy_plan(fft->setup.forward_plan);
        fftwf_destroy_plan(fft->setup.inverse_plan);
#elif defined(__APPLE__)
        if (fft->setup)
        {
            vDSP_destroy_fftsetup(fft->setup);
        }
#endif
        free(fft);
        fft = NULL;
    }
    return NOERR;
}

Error_t
FFTFreeD(FFTConfigD* fft)
{
    if (fft)
    {
        if (fft->split.realp)
        {
            free(fft->split.realp);
            fft->split2.realp = NULL;
        }
        if (fft->split2.realp)
        {
            free(fft->split2.realp);
            fft->split2.realp = NULL;
        }
#ifdef USE_FFTW
        fftw_destroy_plan(fft->setup.forward_plan);
        fftw_destroy_plan(fft->setup.inverse_plan);
#elif defined(__APPLE__)
        if (fft->setup)
        {
            vDSP_destroy_fftsetupD(fft->setup);
        }
#endif
        free(fft);
        fft = NULL;
    }
    return NOERR;
}



Error_t
RFFT(FFTConfig*     fft,
     const float*   inBuffer,
     float*         outBuffer)
{
    float phase[fft->length/2];
    FFTForward(fft, inBuffer, outBuffer, phase);
    return NOERR;
}

Error_t
RFFTD(FFTConfigD*   fft,
      const double* inBuffer,
      double*       outBuffer)
{
    double phase[fft->length/2];
    FFTForwardD(fft, inBuffer, outBuffer, phase);
    return NOERR;
}


Error_t
RIFFT(FFTConfig*    fft,
      const float*  inBuffer,
      float*        outBuffer)
{
    float phase[fft->length/2];
    ClearBuffer(phase, fft->length/2.);
    FFTInverse(fft, inBuffer, phase, outBuffer);
    return NOERR;
}

Error_t
RIFFTD(FFTConfigD*      fft,
       const double*    inBuffer,
       double*          outBuffer)
{
    double phase[fft->length/2];
    ClearBufferD(phase, fft->length/2.);
    FFTInverseD(fft, inBuffer, phase, outBuffer);
    return NOERR;
}


Error_t
FFTForward(FFTConfig*       fft,
           const float*     inBuffer,
           float*           outMag,
           float*           outPhase)
{

#ifdef USE_FFTW
    FFTComplex out[fft->length];
    fftwf_execute_dft_r2c(fft->setup.forward_plan, (float*)inBuffer, out);
    for (unsigned i = 0; i < fft->length/2; i++)
    {
        RectToPolar(out[i][0], out[i][1], &outMag[i], &outPhase[i]);
    }
    
#elif defined(__APPLE__)
    float out[fft->length];
    FFTForwardInterleaved(fft,(FFTComplex*)inBuffer,(FFTComplex*)out);
    
    // Convert real/imaginary to magnitude/phase
    vDSP_polar(out, 2, out, 2, (fft->length / 2));
    
    // Write mag/phase to outputs
    cblas_scopy((fft->length/2), out, 2, outMag, 1);
    cblas_scopy((fft->length / 2), out + 1, 2, outPhase, 1);
#endif
    return NOERR;
}


Error_t
FFTForwardD(FFTConfigD*     fft,
            const double*   inBuffer,
            double*         outMag,
            double*         outPhase)
{
#ifdef USE_FFTW
    FFTComplexD out[fft->length];
    fftw_execute_dft_r2c(fft->setup.forward_plan, (double*)inBuffer, out);
    for (unsigned i = 0; i < fft->length/2; i++)
    {
        RectToPolarD(out[i][0], out[i][1], &outMag[i], &outPhase[i]);
    }

#elif defined(__APPLE__)
    // Scratch buffer
    double out[fft->length];
    
    FFTForwardInterleavedD(fft,(FFTComplexD*)inBuffer,(FFTComplexD*)out);
    // Convert real/imaginary to magnitude/phase
    vDSP_polarD(out, 2, out, 2, (fft->length / 2));
    
    // Write mag/phase to outputs
    cblas_dcopy((fft->length/2), out, 2, outMag, 1);
    cblas_dcopy((fft->length / 2), out + 1, 2, outPhase, 1);
#endif
    return NOERR;
}

Error_t
FFTForwardInterleaved(FFTConfig*    fft,
                      FFTComplex*   inBuffer,
                      FFTComplex*   out)
{
    
#ifdef USE_FFTW
    fftwf_execute_dft_r2c(fft->setup.forward_plan, (float*)inBuffer, out);
#elif defined(__APPLE__)
    
    // Convert input to split complex format
    vDSP_ctoz(inBuffer, 2, &fft->split, 1, (fft->length / 2));
    
    // Forward Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);
    
    // Set this explicitly
    fft->split.imagp[0] = 0.0;
    
    // convert split-complex format to interleaved
    vDSP_ztoc(&fft->split, 1, out, 2, (fft->length / 2));
    
    // Scale to match textbook implementation
    float half = 0.5;
    vDSP_vsmul((float*)out, 1, &half, (float*)out, 1, fft->length);
#endif
    return NOERR;
}


Error_t
FFTForwardInterleavedD(FFTConfigD*  fft,
                       FFTComplexD* inBuffer,
                       FFTComplexD* out)
{
#ifdef USE_FFTW
    fftw_execute_dft_r2c(fft->setup.forward_plan, (double*)inBuffer, out);
#elif defined(__APPLE__)
    // Convert input to split complex format
    vDSP_ctozD(inBuffer, 2, &fft->split, 1, (fft->length / 2));
    
    // Forward Real FFT
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);
    
    // Set this explicitly
    fft->split.imagp[0] = 0.0;
    
    // convert split-complex format to interleaved
    vDSP_ztocD(&fft->split, 1, out, 2, (fft->length / 2));
    
    // Scale to match textbook implementation
    double half = 0.5;
    vDSP_vsmulD((double*)out, 1, &half, (double*)out, 1, fft->length);
#endif
    return NOERR;
}


Error_t
FFTForwardSplit(FFTConfig*      fft,
                FFTComplex*     in_buffer,
                FFTSplitComplex out)
{
    if (fft)
    {
#ifdef USE_FFTW
        FFTComplex iout[fft->length];
        FFTForwardInterleaved(fft, in_buffer, iout);
        for (unsigned i = 0; i < fft->length / 2; ++i)
        {
            out.realp[i] = iout[i][0];
            out.imagp[i] = iout[i][1];
        }
#elif defined(__APPLE__)
        // convert real input to split complex
        vDSP_ctoz((FFTComplex*)in_buffer, 2, &out, 1, (fft->length/2));
        
        // Calculate FFT of the two signals
        vDSP_fft_zrip(fft->setup, &out, 1, fft->log2n, FFT_FORWARD);
        VectorScalarMultiply(out.realp, out.realp, 0.5, fft->length/2);
        VectorScalarMultiply(out.imagp, out.imagp, 0.5, fft->length/2);
#endif
        return NOERR;
    }
    
    else
    {
        return ERROR;
    }
}

Error_t
FFTForwardSplitD(FFTConfigD*        fft,
                 FFTComplexD*       in_buffer,
                 FFTSplitComplexD   out)
{
    if (fft)
    {
#ifdef USE_FFTW
        FFTComplexD iout[fft->length];
        FFTForwardInterleavedD(fft, in_buffer, iout);
        for (unsigned i = 0; i < fft->length / 2; ++i)
        {
            out.realp[i] = iout[i][0];
            out.imagp[i] = iout[i][1];
        }
#elif defined(__APPLE__)
        // convert real input to split complex
        vDSP_ctozD((FFTComplexD*)in_buffer, 2, &out, 1, (fft->length/2));
        
        // Calculate FFT of the two signals
        vDSP_fft_zripD(fft->setup, &out, 1, fft->log2n, FFT_FORWARD);
        VectorScalarMultiplyD(out.realp, out.realp, 0.5, fft->length/2);
        VectorScalarMultiplyD(out.imagp, out.imagp, 0.5, fft->length/2);
        
#endif
        return NOERR;
    }
    
    else
    {
        return ERROR;
    }
}


Error_t
FFTInverse(FFTConfig*       fft,
           const float*     inMag,
           const float*     inPhase,
           float*           outBuffer)
{
#ifdef USE_FFTW
    
    FFTComplex in[fft->length];
    for (unsigned i = 0; i < fft->length/2; i++)
    {
        PolarToRect(inMag[i], inPhase[i], &in[i][0], &in[i][1]);
    }
    fftwf_execute_dft_c2r(fft->setup.inverse_plan, in, outBuffer);
    VectorScalarMultiply(outBuffer, outBuffer, -1.0/fft->length, fft->length);
    
#elif defined(__APPLE__)
    // Scratch buffer
    float in[fft->length];
    
    // Write magnitude/phase interleaved buffer
    cblas_scopy((fft->length/2), inMag, 1, in , 2);
    cblas_scopy((fft->length/2), inPhase, 1, (in + 1) , 2);
    
    // Convert from magnitude/phase to real/imaginary
    vDSP_rect(in, 2, in, 2, (fft->length / 2));
    
    // Write real and imaginary parts to their respective fft buffers
    cblas_scopy((fft->length/2), in, 2, fft->split.realp , 1);
    cblas_scopy((fft->length/2), in + 1, 2, fft->split.imagp , 1);
    
    // do this!
    FFTInverseInterleaved(fft, (FFTComplex*)in, (FFTComplex*)outBuffer);
#endif
    return NOERR;
}


Error_t
FFTInverseD(FFTConfigD*     fft,
            const double*   inMag,
            const double*   inPhase,
            double*         outBuffer)
{
    
#ifdef USE_FFTW
    FFTComplexD in[fft->length];
    for (unsigned i = 0; i < fft->length/2; i++)
    {
        PolarToRectD(inMag[i], inPhase[i], &in[i][0], &in[i][1]);
    }
    fftw_execute_dft_c2r(fft->setup.inverse_plan, in, outBuffer);
    VectorScalarMultiplyD(outBuffer, outBuffer, -1.0/fft->length, fft->length);
#elif defined(__APPLE__)
    // Scratch buffer
    double in[fft->length];
    
    // Write magnitude/phase interleaved buffer
    cblas_dcopy((fft->length/2), inMag, 1, in , 2);
    cblas_dcopy((fft->length/2), inPhase, 1, (in + 1) , 2);
    
    // Convert from magnitude/phase to real/imaginary
    vDSP_rectD(in, 2, in, 2, (fft->length / 2));
    
    // Write real and imaginary parts to their respective fft buffers
    cblas_dcopy((fft->length/2), in, 2, fft->split.realp , 1);
    cblas_dcopy((fft->length/2), in + 1, 2, fft->split.imagp , 1);
    
    // do this!
    FFTInverseInterleavedD(fft, (FFTComplexD*)in, (FFTComplexD*)outBuffer);
#endif
    return NOERR;
}

Error_t
FFTInverseInterleaved(FFTConfig*        fft,
                      const FFTComplex* inBuffer,
                      FFTComplex*       outBuffer)
{
#ifdef USE_FFTW
    fftwf_execute_dft_c2r(fft->setup.inverse_plan, (FFTComplex*)inBuffer, (float*)outBuffer);
    VectorScalarMultiply((float*)outBuffer, (float*)outBuffer, 1.0/fft->length, fft->length);
#elif defined(__APPLE__)
    // Convert input to split complex format
    vDSP_ctoz((const DSPComplex*)inBuffer, 2, (const DSPSplitComplex*)&fft->split, 1, (fft->length / 2));
    
    // Inverse Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    
    // Split complex to interleaved
    vDSP_ztoc(&fft->split, 1, (FFTComplex*)outBuffer, 2, (fft->length / 2));
    
    // Scale the result...
    vDSP_vsmul((float*)outBuffer, 1, &fft->scale, (float*)outBuffer, 1, fft->length);
#endif
    return NOERR;

}

Error_t
FFTInverseInterleavedD(FFTConfigD*          fft,
                       const FFTComplexD*   inBuffer,
                       FFTComplexD*         outBuffer)
{
#ifdef USE_FFTW
    fftw_execute_dft_c2r(fft->setup.inverse_plan, (FFTComplexD*)inBuffer, (double*)outBuffer);
    VectorScalarMultiplyD((double*)outBuffer, (double*)outBuffer, 1.0/fft->length, fft->length);
#elif defined(__APPLE__)
    // Convert input to split complex format
    vDSP_ctozD((const FFTComplexD*)inBuffer, 2, (const FFTSplitComplexD*)&fft->split, 1, (fft->length / 2));
    
    // Inverse Real FFT
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    
    // Split complex to interleaved
    vDSP_ztocD(&fft->split, 1, (FFTComplexD*)outBuffer, 2, (fft->length / 2));
    
    // Scale the result...
    vDSP_vsmulD((double*)outBuffer, 1, &fft->scale, (double*)outBuffer, 1, fft->length);
#endif
    return NOERR;
    
}


Error_t
FFTInverseSplit(FFTConfig*      fft,
                FFTSplitComplex inBuffer,
                FFTComplex*     outBuffer)
{
#ifdef USE_FFTW
    FFTComplex iin[fft->length/2];
    for (unsigned i = 0; i < fft->length / 2; ++i)
    {
        iin[i][0] = inBuffer.realp[i];
        iin[i][1] = inBuffer.imagp[i];
    }
    FFTInverseInterleaved(fft, iin, outBuffer);
#elif defined(__APPLE__)
    float scale = (fft->scale);
    vDSP_fft_zrip(fft->setup, &inBuffer, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztoc(&inBuffer, 1, outBuffer, 2, fft->length/2);
    vDSP_vsmul((float*)outBuffer, 1, &scale, (float*)outBuffer, 1, fft->length/2);
#endif
    return NOERR;
}


Error_t
FFTInverseSplitD(FFTConfigD*        fft,
                 FFTSplitComplexD   inBuffer,
                 FFTComplexD*       outBuffer)
{
#ifdef USE_FFTW
    FFTComplexD iin[fft->length/2];
    for (unsigned i = 0; i < fft->length / 2; ++i)
    {
        iin[i][0] = inBuffer.realp[i];
        iin[i][1] = inBuffer.imagp[i];
    }
    FFTInverseInterleavedD(fft, iin, outBuffer);
#elif defined(__APPLE__)
    double scale = (fft->scale);
    vDSP_fft_zripD(fft->setup, &inBuffer, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztocD(&inBuffer, 1, outBuffer, 2, fft->length/2);
    vDSP_vsmulD((double*)outBuffer, 1, &scale, (double*)outBuffer, 1, fft->length/2);
#endif
    return NOERR;
}



Error_t
FFTConvolve(FFTConfig*  fft,
            float       *in1,
            unsigned    in1_length,
            float       *in2,
            unsigned    in2_length,
            float       *dest)
{
    // Padded input buffers
    float in1_padded[fft->length];
    float in2_padded[fft->length];
    
    //  Allocate & initialize FFT Buffers
    //  real and imaginary parts are consecutive in memory so we can zero both
    //  with a single pass
    ClearBuffer(fft->split.realp, fft->length);
    ClearBuffer(fft->split2.realp, fft->length);
    
    // Zero pad the input buffers to FFT length
    CopyBuffer(in1_padded, in1, in1_length);
    CopyBuffer(in2_padded, in2, in2_length);
    ClearBuffer((in1_padded + in1_length) ,(fft->length - in1_length));
    ClearBuffer((in2_padded + in2_length) ,(fft->length - in2_length));
    
   
    //Calculate FFT of the two signals
    FFTForwardSplit(fft, (FFTComplex*)in1_padded, fft->split);
    FFTForwardSplit(fft, (FFTComplex*)in2_padded, fft->split2);
    
#ifdef USE_FFTW
    for (unsigned i = 0; i < fft->length/2; ++i)
    {
        ComplexMultiply(fft->split.realp[i], fft->split.imagp[i], fft->split2.realp[i], fft->split2.imagp[i], &fft->split.realp[i], &fft->split.imagp[i]);
    }
    
    FFTInverseSplit(fft, fft->split, (FFTComplex*)dest);
    
#elif defined(__APPLE__)
    
    // Store Nyquist value. the FFT packs the real value at nyquist into the
    // imaginary DC location because DC phase is explicitly zero. We need to
    // store the nyquist value for later because this format doesn't work with
    // vDSP_zvmul
    float nyquist_out = fft->split.imagp[0] * fft->split2.imagp[0];
    
    // Set these phase components to a zvmul-friendly 0.0!
    fft->split.imagp[0] = 0.0;
    fft->split2.imagp[0] = 0.0;
    
    // Multiply The two FFTs
    vDSP_zvmul(&fft->split, 1, &fft->split2, 1, &fft->split, 1, fft->length/2, 1);
    
    // And do the nyquist multiplication ourselves!
    fft->split.imagp[0] = nyquist_out;
    
    // Do the inverse FFT
    FFTInverseSplit(fft, fft->split, (FFTComplex*)dest);
    
#endif
    return NOERR;
}



Error_t
FFTConvolveD(FFTConfigD*    fft,
             const double*  in1,
             unsigned       in1_length,
             const double*  in2,
             unsigned       in2_length,
             double*        dest)
{

    // Padded input buffers
    double in1_padded[fft->length];
    double in2_padded[fft->length];
    
    //  Allocate & initialize FFT Buffers
    //  real and imaginary parts are consecutive in memory so we can zero both
    //  with a single pass
    ClearBufferD(fft->split.realp, fft->length);
    ClearBufferD(fft->split2.realp, fft->length);
    
    // Zero pad the input buffers to FFT length
    CopyBufferD(in1_padded, in1, in1_length);
    CopyBufferD(in2_padded, in2, in2_length);
    ClearBufferD((in1_padded + in1_length) ,(fft->length - in1_length));
    ClearBufferD((in2_padded + in2_length) ,(fft->length - in2_length));
    
    
    //Calculate FFT of the two signals
    FFTForwardSplitD(fft, (FFTComplexD*)in1_padded, fft->split);
    FFTForwardSplitD(fft, (FFTComplexD*)in2_padded, fft->split2);
    
#ifdef USE_FFTW
    for (unsigned i = 0; i < fft->length/2; ++i)
    {
        ComplexMultiplyD(fft->split.realp[i], fft->split.imagp[i], fft->split2.realp[i], fft->split2.imagp[i], &fft->split.realp[i], &fft->split.imagp[i]);
    }
    FFTInverseSplitD(fft, fft->split, (FFTComplexD*)dest);
    
#elif defined(__APPLE__)
    // Store Nyquist value. the FFT packs the real value at nyquist into the
    // imaginary DC location because DC phase is explicitly zero. We need to
    // store the nyquist value for later because this format doesn't work with
    // vDSP_zvmul
    double nyquist_out = fft->split.imagp[0] * fft->split2.imagp[0];
    
    // Set these phase components to a zvmul-friendly 0.0!
    fft->split.imagp[0] = 0.0;
    fft->split2.imagp[0] = 0.0;
    
    // Multiply The two FFTs
    vDSP_zvmulD(&fft->split, 1, &fft->split2, 1, &fft->split, 1, fft->length/2, 1);
    
    // And do the nyquist multiplication ourselves!
    fft->split.imagp[0] = nyquist_out;
    
    // Do the inverse FFT
    FFTInverseSplitD(fft, fft->split, (FFTComplexD*)dest);
#endif
    return NOERR;
}


Error_t
FFTFilterConvolve(FFTConfig*        fft,
                  float*            in,
                  unsigned          in_length,
                  FFTSplitComplex   fft_ir,
                  float*            dest)
{
    // Padded buffer
    float in_padded[fft->length];
    
    // Zero pad the input to FFT length
    ClearBuffer(in_padded + in_length, (fft->length - in_length));
    CopyBuffer(in_padded, in, in_length);
    
    // Calculate FFT of the input
    FFTForwardSplit(fft, (FFTComplex*)in_padded, fft->split);
   
#ifdef USE_FFTW
    for (unsigned i = 0; i < fft->length/2; ++i)
    {
        ComplexMultiply(fft->split.realp[i], fft->split.imagp[i], fft_ir.realp[i], fft_ir.imagp[i], &fft->split.realp[i], &fft->split.imagp[i]);
    }
#elif defined(__APPLE__)
    // Store Nyquist value. the FFT packs the real value at nyquist into the
    // imaginary DC location because DC phase is explicitly zero. We need to
    // store the nyquist value for later because this format doesn't work with
    // vDSP_zvmul
    float nyquist_out = fft->split.imagp[0] * fft_ir.imagp[0];
    
    // Set these phase components to a zvmul-friendly 0.0!
    fft->split.imagp[0] = 0.0;
    fft_ir.imagp[0] = 0.0;
    
    // Multiply The two FFTs
    vDSP_zvmul(&fft->split, 1, &fft_ir, 1, &fft->split, 1, fft->length/2, 1);
    
    // And do the nyquist multiplication ourselves!
    fft->split.imagp[0] = nyquist_out;
#endif
    
    // Do the inverse FFT and convert the split complex output to real
    FFTInverseSplit(fft, fft->split, (FFTComplex*)dest);

    return NOERR;
}
    

Error_t
FFTFilterConvolveD(FFTConfigD*      fft,
                   double*          in,
                   unsigned         in_length,
                   FFTSplitComplexD fft_ir,
                   double*          dest)
{

    // Padded buffer
    double in_padded[fft->length];
    
    // Zero pad the input to FFT length
    ClearBufferD(in_padded + in_length, (fft->length - in_length));
    CopyBufferD(in_padded, in, in_length);
    
    // Calculate FFT of the input
    FFTForwardSplitD(fft, (FFTComplexD*)in_padded, fft->split);
    
    
#ifdef USE_FFTW
    for (unsigned i = 0; i < fft->length/2; ++i)
    {
        ComplexMultiplyD(fft->split.realp[i], fft->split.imagp[i], fft_ir.realp[i], fft_ir.imagp[i], &fft->split.realp[i], &fft->split.imagp[i]);
    }
#elif defined(__APPLE__)
    // Store Nyquist value. the FFT packs the real value at nyquist into the
    // imaginary DC location because DC phase is explicitly zero. We need to
    // store the nyquist value for later because this format doesn't work with
    // vDSP_zvmul
    double nyquist_out = fft->split.imagp[0] * fft_ir.imagp[0];
    
    // Set these phase components to a zvmul-friendly 0.0!
    fft->split.imagp[0] = 0.0;
    fft_ir.imagp[0] = 0.0;
    
    // Multiply The two FFTs
    vDSP_zvmulD(&fft->split, 1, &fft_ir, 1, &fft->split, 1, fft->length/2, 1);
    
    // And do the nyquist multiplication ourselves!
    fft->split.imagp[0] = nyquist_out;
    
#endif
    // Do the inverse FFT and convert the split complex output to real
    FFTInverseSplitD(fft, fft->split, (FFTComplexD*)dest);

    return NOERR;
}

Error_t
FFTdemo(FFTConfig * fft, float* buffer)
{
#ifdef USE_FFTW
    
#elif defined(__APPLE__)
    // Convert input to split complex format
    vDSP_ctoz((const DSPComplex*)buffer, 2, &fft->split, 1, (fft->length / 2));
    
    // Forward Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);
    
    // Set this explicitly
    fft->split.imagp[0] = 0.0;
    for (unsigned i = 0; i < fft->length; ++i)
    {
        printf("%1.3f + %1.3fj,\n", fft->split.realp[i], fft->split.imagp[i]);
    }
#endif
    return NOERR;
    
}

#endif
