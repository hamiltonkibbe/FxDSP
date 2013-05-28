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
    fft->scale = 1.0 / (fft->length);
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
    // Scratch buffer
    float out[fft->length];
    
    // Convert input to split complex format
    vDSP_ctoz((COMPLEX*) inBuffer, 2, &fft->split, 1, (fft->length / 2));
    
    // Forward Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);
    
    // Set this explicitly
    fft->split.imagp[0] = 0.0;


    // convert split-complex format to interleaved
    vDSP_ztoc(&fft->split, 1, (COMPLEX *) out, 2, (fft->length / 2));
    
    // Scale to match textbook implementation
    float half = 0.5;
    vDSP_vsmul(out, 1, &half, out, 1, fft->length);
    
    // Convert real/imaginary to magnitude/phase
    vDSP_polar(out, 2, out, 2, (fft->length / 2));
    
    // Write mag/phase to outputs
    cblas_scopy((fft->length/2), out, 2, outMag, 1);
    cblas_scopy((fft->length / 2), out + 1, 2, outPhase, 1);

    return FT_NOERR;
}

FtAudioError_t
FtAudioFFTInverse(FtAudioFFTConfig*     fft,
                  const float*          inMag,
                  const float*          inPhase,
                  float*                outBuffer)
{
    // Scratch buffer
    float in[fft->length];
    
    // Write interleaved magnitude/phase buffer
    cblas_scopy((fft->length/2), inMag, 1, in , 2);
    cblas_scopy((fft->length/2), inPhase, 1, (in + 1) , 2);
    
    // Convert magnitude/phase to real/imaginary
    vDSP_rect(in, 2, in, 2, (fft->length / 2));
    
    // Write real and imaginary parts to their respective FFT buffers
    cblas_scopy((fft->length/2), in, 2, fft->split.realp , 1);
    cblas_scopy((fft->length/2), in + 1, 2, fft->split.imagp , 1);
    
    
    // Inverse Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    
    // Split complex to interleaved
    vDSP_ztoc(&fft->split, 1, (COMPLEX*)outBuffer, 2, (fft->length / 2));
    
    // Scale the result...
    vDSP_vsmul(outBuffer, 1, &fft->scale, outBuffer, 1, fft->length);
    
    return FT_NOERR;
}
