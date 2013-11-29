//
//  TestMultibandBank.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/28/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioMultibandBank.h"
#include "FtAudioFFT.h"
#include "FtAudioDsp.h"


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
    //FTA_VectorScalarMultiply(signal, signal, (1.0/32.0), 64);
    FTA_FFTConfig*          fft;
    FTA_MultibandFilter*    filter;
    
    // Create delta function
    FTA_FillBuffer(signal, 64, 0.0);
    signal[0] = 1.0;
    
    // Spectrum of input
    fft = FTA_FFTInit(64);
    FTA_FFTForward(fft, signal, inSpectrum, phase);
    
    // process data
    filter = FTA_MultibandFilterInit(1000, 12000, 44100);
    FTA_MultibandFilterProcess(filter, low, mid, high, signal, 64);
    
    // Mix bands back together
    FTA_BufferAdd(out, low, mid, 64);
    FTA_BufferAdd(out, out, high, 64);

    // Calculate spectrum of output
    FTA_FFTForward(fft, out, outSpectrum, phase);
    
    // Cleanup
    FTA_FFTFree(fft);
    FTA_MultibandFilterFree(filter);
    
    
    // Print Matlab-readable values for each band
    printf("\n\nlow = 20*log10(abs(fft([ ");
    for (unsigned i = 0; i < 64; ++i)
    {
        printf("%f\n", low[i]);
    }
    printf("])));\n\n");
    
    // ...
    printf("\n\nmid = 20*log10(abs(fft([ ");
    for (unsigned i = 0; i < 64; ++i)
    {
        printf("%f\n", mid[i]);
    }
    printf("])));\n\n");
    
    // ...
    printf("\n\nhigh = 20*log10(abs(fft([ ");
    for (unsigned i = 0; i < 64; ++i)
    {
        printf("%f\n", high[i]);
    }
    printf("])));\n\n");
    
    
    
    // Verify output
    for (unsigned i = 0; i < 32; ++i)
    {
        XCTAssertEqualWithAccuracy(outSpectrum[i], inSpectrum[i], 0.05, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, outSpectrum[i], inSpectrum[i]);
    }
}

@end
