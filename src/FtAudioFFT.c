//
//  FtAudioFFT.c
//  Test
//
//  Created by Hamilton Kibbe on 5/27/13.
//
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "FtAudioFFT.h"

struct FtAudioFFTConfig
{
    unsigned        length;
    float           scale;
    float           log2n;
    COMPLEX_SPLIT   split;
    FFTSetup        setup;
};

FtAudioFFTConfig*
FtAudioFFTInit(unsigned length)
{
    FtAudioFFTConfig* fft = (FtAudioFFTConfig*)malloc(sizeof(FtAudioFFTConfig));
    fft->length = length;
    fft->scale = 1.0 / (2.0 * fft->length);
    fft->log2n = log2f(fft->length);
    fft->split.realp = (float*)malloc((fft->length / 2) * sizeof(float));
    fft->split.imagp = (float*)malloc((fft->length / 2) * sizeof(float));
    fft->setup = vDSP_create_fftsetup(fft->log2n, FFT_RADIX2);
    return fft;
}


FtAudioError_t
FtAudioFFTFree(FtAudioFFTConfig* fft)
{
    free(fft->setup);
    free(fft->split.realp);
    free(fft->split.imagp);
    return FT_NOERR;
}

FtAudioError_t
FtAudioFFTForward(FtAudioFFTConfig*     fft,
                  const float*          inBuffer,
                  float*                outMag,
                  float*                outPhase)
{
    // Convert input to split complex format
    vDSP_ctoz((COMPLEX*) inBuffer, 2, &fft->split, 1, (fft->length / 2));
    
    // Forward Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);
    
    // Set this explicitly
    fft->split.imagp[0] = 0.0;
    
    // Shorthand pointers
    float* real = fft->split.realp;
    float* imag = fft->split.imagp;
    unsigned buf_idx;
    for(buf_idx = 0; buf_idx < (fft->length / 2); ++buf_idx)
    {
        // Scale by 2 to match textbook implementation
        float r = real[buf_idx] / 2.0;
        float i = imag[buf_idx] / 2.0;
        
        // Calculate power
        float pwr = (r * r) + (i * i);
        
        // Calculate Mag and Phase
        outMag[buf_idx] = sqrtf(pwr);
        outPhase[buf_idx] = atan2f(i,r);
    }
    return FT_NOERR;
}

FtAudioError_t
FtAudioFFTInverse(FtAudioFFTConfig*     fft,
                  const float*          inMag,
                  const float*          inPhase,
                  float*                outBuffer)
{
    float* real = fft->split.realp;
    float* imag = fft->split.imagp;
    
    // Convert mag/phase to split real/complex format
    for (unsigned i = 0; i < fft->length / 2; ++i)
    {
        *real++ = inMag[i] * cosf(inPhase[i]);
        *imag++ = inMag[i] * sinf(inPhase[i]);
    }
    
    // Inverse Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    
    // Split complex to interleaved
    vDSP_ztoc(&fft->split, 1, (COMPLEX*)outBuffer, 2, (fft->length / 2));
    
    // Scale the result...
    vDSP_vsmul(outBuffer, 1, &fft->scale, outBuffer, 1, fft->length);
    return FT_NOERR;
}
