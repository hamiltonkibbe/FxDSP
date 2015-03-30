//
//  TestFFT.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 10/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "testFFT.h"
#include "FFT.h"
#include "Dsp.h"

@interface FxDSPFFT_Tests : XCTestCase

@end

@implementation FxDSPFFT_Tests


- (void)testForwardFFTAgainstMatlab
{
    float signal[64];
    float magnitude[32];
    float phase[32];
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = (float)i + 1.0;
    }
    
    FFTConfig* fft = FFTInit(64);
    if(fft)
    {
        FFTForward(fft, signal, magnitude, phase);
        FFTFree(fft);
        for (unsigned i = 0; i < 32; ++i)
        {
            XCTAssertEqualWithAccuracy(magnitude[i], matlabMagnitude[i], 0.001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, magnitude[i], matlabMagnitude[i]);
            XCTAssertEqualWithAccuracy(phase[i], matlabPhase[i], 0.001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, phase[i], matlabPhase[i]);
        }
    }
}

- (void)testForwardToInverseFFT
{
    float signal[64];
    float magnitude[32];
    float phase[32];
    float output[64];
    
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = sinf(((4*M_PI)/64) * i) + 0.5 * sinf(((8*M_PI)/64) * i);
    }
    
    FFTConfig* fft = FFTInit(64);
    if(fft)
    {
        FFTForward(fft, signal, magnitude, phase);
        FFTInverse(fft, magnitude, phase, output);
        FFTFree(fft);
        for (unsigned i = 0; i < 64; ++i)
        {
            XCTAssertEqualWithAccuracy(signal[i], output[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, signal[i], output[i]);
        }
    }
}

- (void) testFFTConvolution
{
    float dest[15];
    float in[3] = {1.0, 2.0, 3.0};
    float in2[4] = {4.0,5.0,6.0, 7};
    FFTConfig* fft = FFTInit(16);
    if(fft)
    {
        FFTConvolve(fft, in, 3, in2, 4, dest);
        FFTFree(fft);
        for (unsigned i = 0; i < 15; ++i)
        {
            XCTAssertEqualWithAccuracy(dest[i], matlabConvolution[i], 0.00025, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, dest[i], matlabConvolution[i]);
        }
    }
}

- (void)testFFTFilterConvolution
{
    printf("\nTesting FFTFilterConvolution...\n");
    float in[3] = {1.0, 2.0, 3.0};
    float in2[4] = {4.0,5.0,6.0,7};
    float padded[16];
    float dest[16];
    FillBuffer(padded, 16, 0.0);
    CopyBuffer(padded, in2, 4);
    
    DSPSplitComplex splitcomplex;
    splitcomplex.realp = (float*) malloc(16 * sizeof(float));
    splitcomplex.imagp = splitcomplex.realp + 8;
    
    //FillBuffer(splitcomplex.realp, 16, 0.0);
    FFTConfig* fft = FFTInit(16);
    if (fft)
    {
        FFTForwardSplit(fft, (DSPComplex*)padded, &splitcomplex);
        FFTFilterConvolve(fft, in, 3, splitcomplex, dest);
        FFTFree(fft);
        for (unsigned i = 0; i < 15; ++i)
        {
            XCTAssertEqualWithAccuracy(dest[i], matlabConvolution[i], 0.00025, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, dest[i], matlabConvolution[i]);
        }
    }
}

@end
