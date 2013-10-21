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
#include "FtAudioDsp.h"

struct FtAudioFFTConfig
{
    unsigned        length;
    float           scale;
    float           log2n;
    DSPSplitComplex split;
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
    
    free(fft->split.realp);
    free(fft->split.imagp);
    vDSP_destroy_fftsetup(fft->setup);
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
    
    FtAudioFFTForwardInterleaved(fft,(DSPComplex*)inBuffer,(DSPComplex*)out);

    // Convert real/imaginary to magnitude/phase
    vDSP_polar(out, 2, out, 2, (fft->length / 2));
    
    // Write mag/phase to outputs
    cblas_scopy((fft->length/2), out, 2, outMag, 1);
    cblas_scopy((fft->length / 2), out + 1, 2, outPhase, 1);

    return FT_NOERR;
}




FtAudioError_t
FtAudioFFTForwardInterleaved(FtAudioFFTConfig*      fft,
                             DSPComplex*            inBuffer,
                             DSPComplex*            out)
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
    
    // Write magnitude/phase interleaved buffer
    cblas_scopy((fft->length/2), inMag, 1, in , 2);
    cblas_scopy((fft->length/2), inPhase, 1, (in + 1) , 2);
    
    // Convert from magnitude/phase to real/imaginary
    vDSP_rect(in, 2, in, 2, (fft->length / 2));
    
    // Write real and imaginary parts to their respective fft buffers
    cblas_scopy((fft->length/2), in, 2, fft->split.realp , 1);
    cblas_scopy((fft->length/2), in + 1, 2, fft->split.imagp , 1);
    
    // do this!
    FtAudioFFTInverseInterleaved(fft, in, outBuffer);

    return FT_NOERR;
}


FtAudioError_t
FtAudioFFTInverseInterleaved(FtAudioFFTConfig*     fft,
                             const float*          inBuffer,
                              float*                outBuffer)
{
    // Inverse Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    
    // Split complex to interleaved
    vDSP_ztoc(&fft->split, 1, (COMPLEX*)outBuffer, 2, (fft->length / 2));
    
    // Scale the result...
    vDSP_vsmul(outBuffer, 1, &fft->scale, outBuffer, 1, fft->length);
    
    return FT_NOERR;

}


FtAudioError_t
FtAudioFFTConvolve(FtAudioFFTConfig* fft,
                   float       *in1, 
                   unsigned    in1_length, 
                   float       *in2, 
                   unsigned    in2_length, 
                   float       *dest)
{
    // Padded buffer
    float in1_padded[fft->length];
    float in2_padded[fft->length];
    
    // Allocate & initialize FFT Buffers
    DSPSplitComplex fft1;
    DSPSplitComplex fft2;
    
    // Set these up as consecutive buffers
    //float real1[fft->length/2];
    //float real2[fft->length/2];
    //float imag1[fft->length/2];
    //float imag2[fft->length/2];
    float buf1[fft->length];
    float buf2[fft->length];
    
    //fft1.realp = real1;
    //fft2.realp = real2;
    //fft1.imagp = imag1;
    //ft2.imagp = imag2;
    fft1.realp = buf1;
    fft2.realp = buf2;
    fft1.imagp = buf1 + (fft->length/2);
    fft2.imagp = buf2 + (fft->length/2);
    
    
    
    // So we can do this more efficiently
    //FtAudioFillBuffer(fft1.realp, fft->length/2, 0.0);
    //FtAudioFillBuffer(fft2.realp, fft->length/2, 0.0);
    //FtAudioFillBuffer(fft1.imagp, fft->length/2, 0.0);
    //FtAudioFillBuffer(fft2.imagp, fft->length/2, 0.0);
    FtAudioFillBuffer(fft1.realp, fft->length, 0.0);
    FtAudioFillBuffer(fft1.realp, fft->length, 0.0);
    
    // Zero pad the inputs to FFT length
    FtAudioFillBuffer(in1_padded,fft->length, 0.0);
    FtAudioFillBuffer(in2_padded,fft->length, 0.0);
    
    FtAudioCopyBuffer(in1_padded, in1, in1_length);
    FtAudioCopyBuffer(in2_padded, in2, in2_length);
    
    // Interleaved to split-complex
    vDSP_ctoz((DSPComplex*)in1_padded, 2, &fft1, 1, (fft->length/2));
    vDSP_ctoz((DSPComplex*)in2_padded, 2, &fft2, 1, (fft->length/2));

   
    //Calculate FFT of the two signals
    vDSP_fft_zrip(fft->setup, &fft1, 1, fft->log2n, FFT_FORWARD);
    vDSP_fft_zrip(fft->setup, &fft2, 1, fft->log2n, FFT_FORWARD);

    
    // Store value at Nyquist. the FFT packs the real value at nyquist into the
    // imaginary DC location because DC phase is explicitly zero. We need to
    // store the nyquist value for later because this format doesn't work with
    // vDSP_zvmul
    float in1_nyquist = fft1.imagp[0];
    float in2_nyquist = fft2.imagp[0];
    
    // Set these phase components to a zvmul-friendly 0.0!
    fft1.imagp[0] = 0.0;
    fft2.imagp[0] = 0.0;
    
    // Multiply The two FFTs
    vDSP_zvmul(&fft1, 1, &fft2, 1, &fft1, 1, fft->length/2, 1);
    
    // And do the nyquist multiplication ourselves!
    fft1.imagp[0] = in1_nyquist * in2_nyquist;
    
    // Do the inverse FFT and convert the split complex output to real!
    vDSP_fft_zrip(fft->setup, &fft1, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztoc(&fft1, 1, (DSPComplex*)dest, 2, fft->length/2);
    
    // Scale the output
    float scale = 1.0/(4.0 * fft->length);
    vDSP_vsmul(dest, 1, &scale, dest, 1, fft->length/2);
    
    return FT_NOERR;
}



FtAudioError_t
FtAudioFFTFilterConvolve(FtAudioFFTConfig* fft,
                   float       *in1, 
                   unsigned    in1_length, 
                   float       *in2, 
                   unsigned    in2_length, 
                   float       *dest)
{
    // Padded buffer
    float in1_padded[fft->length];
    
    // FFT Buffers
    float in1_fft[fft->length];
    float convolved[fft->length];

    // Zero pad the inputs to FFT length
    FtAudioFillBuffer(in1_padded + in1_length, (fft->length - in1_length), 0.0);
    FtAudioCopyBuffer(in1_padded, in1, in1_length);
    
    // Calculate FFT of the two signals
    FtAudioFFTForwardInterleaved(fft, (DSPComplex*)in1_padded, (DSPComplex*)in1_fft);
    
    // Multiply the FFTs
    FtAudioVectorVectorMultiply(convolved, in1_fft, in2, fft->length);
    
    // Calculate IFFT of convolved signals and write to output
    FtAudioFFTInverseInterleaved(fft, convolved, dest);
    return FT_NOERR;
}
    


FtAudioError_t
FtAudioFFTdemo(FtAudioFFTConfig * fft,
               float*           buffer)
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
    return FT_NOERR;
    
}


