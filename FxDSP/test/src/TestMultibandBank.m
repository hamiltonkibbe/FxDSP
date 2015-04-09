//
//  TestMultibandBank.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/28/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "MultibandBank.h"
#include "FFT.h"
#include "Dsp.h"


@interface TestMultibandBank : XCTestCase

@end

@implementation TestMultibandBank


- (void)testMixBack
{
    float signal[64];
    float inSpectrum[32];
    float low[64];
    float mid[64];
    float high[64];
    float out[64];
    float outSpectrum[32];
    float phase[32];
    //for (unsigned i = 0; i < 64; ++i)
    //{
    //    signal[i] = (float)i + 1.0;
    //}
    //VectorScalarMultiply(signal, signal, (1.0/32.0), 64);
    FFTConfig*          fft;
    MultibandFilter*    filter;
    
    // Create delta function
    FillBuffer(signal, 64, 0.0);
    signal[0] = 1.0;
    
    // Spectrum of input
    fft = FFTInit(64);
    FFTForward(fft, signal, inSpectrum, phase);
    
    // process data
    filter = MultibandFilterInit(1000, 12000, 44100);
    MultibandFilterProcess(filter, low, mid, high, signal, 64);
    
    // Mix bands back together
    VectorVectorAdd(out, low, mid, 64);
    VectorVectorAdd(out, out, high, 64);

    // Calculate spectrum of output
    FFTForward(fft, out, outSpectrum, phase);
    
    // Cleanup
    FFTFree(fft);
    MultibandFilterFree(filter);
    
    
    // Verify output
    for (unsigned i = 0; i < 32; ++i)
    {
        XCTAssertEqualWithAccuracy(outSpectrum[i], inSpectrum[i], 0.01, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, outSpectrum[i], inSpectrum[i]);
    }
}

@end
