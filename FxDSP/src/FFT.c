//
//  FFT.c
//  Test
//
//  Created by Hamilton Kibbe on 5/27/13.
//
//


#include "FFT.h"
#include "Dsp.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef __APPLE__
typedef FFTSetup FFT_SETUP;
typedef FFTSetupD FFT_SETUP_D;
#elif defined(__USE_FFTW__)

typedef struct {
    fftwf_plan forward_plan;
    fftwf_plan inverse_plan;
} FFT_SETUP;

typedef struct {
    fftw_plan forward_plan;
    fftw_plan inverse_plan;
} FFT_SETUP_D;

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



#ifdef __APPLE__

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
        
        fft->setup = vDSP_create_fftsetup(fft->log2n, FFT_RADIX2);
        
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
        
        fft->setup = vDSP_create_fftsetupD(fft->log2n, FFT_RADIX2);
        
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
        if (fft->setup)
        {
            vDSP_destroy_fftsetup(fft->setup);
        }
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
        if (fft->setup)
        {
            vDSP_destroy_fftsetupD(fft->setup);
        }
        free(fft);
        fft = NULL;
    }
    return NOERR;
}

Error_t
FFTForward(FFTConfig*       fft,
           const float*     inBuffer,
           float*           outMag,
           float*           outPhase)
{
    // Scratch buffer
    float out[fft->length];
    
    FFTForwardInterleaved(fft,(FFTComplex*)inBuffer,(FFTComplex*)out);

    // Convert real/imaginary to magnitude/phase
    vDSP_polar(out, 2, out, 2, (fft->length / 2));
    
    // Write mag/phase to outputs
    cblas_scopy((fft->length/2), out, 2, outMag, 1);
    cblas_scopy((fft->length / 2), out + 1, 2, outPhase, 1);

    return NOERR;
}


Error_t
FFTForwardD(FFTConfigD*     fft,
            const double*   inBuffer,
            double*         outMag,
            double*         outPhase)
{
    // Scratch buffer
    double out[fft->length];
    
    FFTForwardInterleavedD(fft,(FFTComplexD*)inBuffer,(FFTComplexD*)out);
    
    // Convert real/imaginary to magnitude/phase
    vDSP_polarD(out, 2, out, 2, (fft->length / 2));
    
    // Write mag/phase to outputs
    cblas_dcopy((fft->length/2), out, 2, outMag, 1);
    cblas_dcopy((fft->length / 2), out + 1, 2, outPhase, 1);
    
    return NOERR;
}

Error_t
FFTForwardInterleaved(FFTConfig*    fft,
                      FFTComplex*   inBuffer,
                      FFTComplex*   out)
{
    
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

    return NOERR;
}


Error_t
FFTForwardInterleavedD(FFTConfigD*  fft,
                       FFTComplexD* inBuffer,
                       FFTComplexD* out)
{
    
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
    
    return NOERR;
}


Error_t
FFTForwardSplit(FFTConfig*          fft,
                FFTComplex*         in_buffer,
                FFTSplitComplex*    out)
{
    if (fft)
    {
        // convert real input to split complex
        vDSP_ctoz((FFTComplex*)in_buffer, 2, out, 1, (fft->length/2));
        
        // Calculate FFT of the two signals
        vDSP_fft_zrip(fft->setup, out, 1, fft->log2n, FFT_FORWARD);
    
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
                 FFTSplitComplexD*  out)
{
    if (fft)
    {
        // convert real input to split complex
        vDSP_ctozD((FFTComplexD*)in_buffer, 2, out, 1, (fft->length/2));
        
        // Calculate FFT of the two signals
        vDSP_fft_zripD(fft->setup, out, 1, fft->log2n, FFT_FORWARD);
        
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
    FFTInverseInterleaved(fft, in, outBuffer);

    return NOERR;
}


Error_t
FFTInverseD(FFTConfigD*     fft,
            const double*   inMag,
            const double*   inPhase,
            double*         outBuffer)
{
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
    FFTInverseInterleavedD(fft, in, outBuffer);
    
    return NOERR;
}

Error_t
FFTInverseInterleaved(FFTConfig*    fft,
                      const float*  inBuffer,
                      float*        outBuffer)
{
    // Convert input to split complex format
    vDSP_ctoz((const DSPComplex*)inBuffer, 2, (const DSPSplitComplex*)&fft->split, 1, (fft->length / 2));
    
    // Inverse Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    
    // Split complex to interleaved
    vDSP_ztoc(&fft->split, 1, (FFTComplex*)outBuffer, 2, (fft->length / 2));
    
    // Scale the result...
    vDSP_vsmul(outBuffer, 1, &fft->scale, outBuffer, 1, fft->length);
    
    return NOERR;

}

Error_t
FFTInverseInterleavedD(FFTConfigD*      fft,
                       const double*    inBuffer,
                       double*          outBuffer)
{
    // Convert input to split complex format
    vDSP_ctozD((const FFTComplexD*)inBuffer, 2, (const FFTSplitComplexD*)&fft->split, 1, (fft->length / 2));
    
    // Inverse Real FFT
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    
    // Split complex to interleaved
    vDSP_ztocD(&fft->split, 1, (FFTComplexD*)outBuffer, 2, (fft->length / 2));
    
    // Scale the result...
    vDSP_vsmulD(outBuffer, 1, &fft->scale, outBuffer, 1, fft->length);
    
    return NOERR;
    
}


Error_t
FFTInverseSplit(FFTConfig*          fft,
                FFTSplitComplex*    in_buffer,
                FFTComplex*         out_buffer)
{
    vDSP_fft_zrip(fft->setup, in_buffer, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztoc(in_buffer, 1, (FFTComplex*)out_buffer, 2, fft->length/2);
    
    return NOERR;
}


Error_t
FFTInverseSplitD(FFTConfigD*        fft,
                 FFTSplitComplexD*  in_buffer,
                 FFTComplexD*       out_buffer)
{
    vDSP_fft_zripD(fft->setup, in_buffer, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztocD(in_buffer, 1, (FFTComplexD*)out_buffer, 2, fft->length/2);
    
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
    FFTForwardSplit(fft, (FFTComplex*)in1_padded, &fft->split);
    FFTForwardSplit(fft, (FFTComplex*)in2_padded, &fft->split2);
    
    
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
    FFTInverseSplit(fft, &fft->split, (DSPComplex*)dest);
    
    // Scale the output
    float scale = (fft->scale / 4.0);
    vDSP_vsmul(dest, 1, &scale, dest, 1, fft->length/2);
    
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
    FFTForwardSplitD(fft, (FFTComplexD*)in1_padded, &fft->split);
    FFTForwardSplitD(fft, (FFTComplexD*)in2_padded, &fft->split2);
    
    
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
    FFTInverseSplitD(fft, &fft->split, (FFTComplexD*)dest);
    
    // Scale the output
    double scale = (fft->scale / 4.0);
    vDSP_vsmulD(dest, 1, &scale, dest, 1, fft->length/2);
    
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
    cblas_scopy(in_length, in, 1, in_padded, 1);
    
    // Calculate FFT of the input
    FFTForwardSplit(fft, (FFTComplex*)in_padded, &fft->split);
   
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
    
    // Do the inverse FFT and convert the split complex output to real
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztoc(&fft->split, 1, (FFTComplex*)dest, 2, fft->length/2);
    
    // Scale the output
    float scale = (fft->scale / 4.0);
    vDSP_vsmul(dest, 1, &scale, dest, 1, fft->length/2);
    

    // So fast convolution.
    // Wow.
    // Such N log N.
    // Much aglorithm.
    
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
    cblas_dcopy(in_length, in, 1, in_padded, 1);
    
    // Calculate FFT of the input
    FFTForwardSplitD(fft, (FFTComplexD*)in_padded, &fft->split);
    
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
    
    // Do the inverse FFT and convert the split complex output to real
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztocD(&fft->split, 1, (FFTComplexD*)dest, 2, fft->length/2);
    
    // Scale the output
    double scale = (fft->scale / 4.0);
    vDSP_vsmulD(dest, 1, &scale, dest, 1, fft->length/2);
    
    
    // So fast convolution.
    // Wow.
    // Such N log N.
    // Much aglorithm.
    
    return NOERR;
}

Error_t
FFTdemo(FFTConfig * fft, float* buffer)
{
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
    return NOERR;
    
}

#endif
