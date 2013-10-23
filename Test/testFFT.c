//
//  testFFT.c
//  Test
//
//  Created by Hamilton Kibbe on 5/28/13.
//
//

#include "testFFT.h"
#include "FtAudioFFT.h"
#include <math.h>
#include <stdio.h>
#include "signals.h"
#include "FtAudioDsp.h"
#include "testFIR.h"

unsigned
runFFTTests(void)
{
    
    unsigned passed = 1;
	printf("\n[FtAudioFFT] RUNNING TESTS\n");
	
    for (unsigned i = 0; i < NUMBER_OF_FFT_TESTS; ++i)
    {
        if (fftTests[i]())
            printf("\t\tPASSED\n");
        else
        {
            printf("\t\tFAILED\n");
            passed = 0;
        }
    }
	return passed;
}



unsigned
testForwardFFTAgainstMatlab()
{
    printf("Testing FFT against Matlab...");
    float signal[64];
    float magnitude[32];
    float phase[32];
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = (float)i + 1.0;
    }
    
    FtAudioFFTConfig* fft = FtAudioFFTInit(64);
    FtAudioFFTForward(fft, signal, magnitude, phase);
    FtAudioFFTFree(fft);
    return (CompareFloatBuffers(magnitude, matlabMagnitude, 32, 0.001) && CompareFloatBuffers(phase, matlabPhase, 32, 0.001));
}

unsigned
testForwardToInverseFFT()
{
    printf("Testing Forward<->Inverse FFT...");
    float signal[64];
    float magnitude[32];
    float phase[32];
    float output[64];
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = sinf(((4*M_PI)/64) * i) + 0.5 * sinf(((8*M_PI)/64) * i);
    }
    
    FtAudioFFTConfig* fft = FtAudioFFTInit(64);
    FtAudioFFTForward(fft, signal, magnitude, phase);
    FtAudioFFTInverse(fft, magnitude, phase, output);
    FtAudioFFTFree(fft);
    return CompareFloatBuffers(signal, output, 64, 0.00001);
}


unsigned
testFFTConvolution()
{
    printf("Testing FFT Convolution...");
    float dest[15];
    float in[3] = {1.0, 2.0, 3.0};
    float in2[4] = {4.0,5.0,6.0, 7};
    FtAudioFFTConfig* fft = FtAudioFFTInit(16);
    FtAudioFFTConvolve(fft, in, 3, in2, 4, dest);
    return CompareFloatBuffers(dest, matlabConvolution, 15, 0.00025);
}


unsigned
testFFTFilterConvolution()
{
    printf("Testing FFT Filter Convolution...");
    float in[3] = {1.0, 2.0, 3.0};
    float in2[4] = {4.0,5.0,6.0,7};
    FtAudioFFTConfig* fft = FtAudioFFTInit(16);
    float padded[16];
    float dest[16];
    FtAudioFillBuffer(padded, 16, 0.0);
    FtAudioCopyBuffer(padded, in2, 4);
   
    DSPSplitComplex splitcomplex;
    splitcomplex.realp = (float*) malloc(16 * sizeof(float));
    splitcomplex.imagp = splitcomplex.realp + 8;
    
    //FtAudioFillBuffer(splitcomplex.realp, 16, 0.0);
    FtAudioFFTForwardSplit(fft, (DSPComplex*)padded, &splitcomplex);
    FtAudioFFTFilterConvolve(fft, in, 3, splitcomplex, dest);
    return CompareFloatBuffers(dest, matlabConvolution, 15, 0.00025);
    return 0;
}
