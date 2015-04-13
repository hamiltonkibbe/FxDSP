//
//  TestMultibandFilterBank.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/12/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "gtest/gtest.h"
#include "MultibandBank.h"
#include "FFT.h"
#include "Dsp.h"


#ifdef __APPLE__

#define EPSILON (0.006)

#pragma mark -
#pragma mark Single Precision Tests

TEST(MultibandBankSingle, TestMixBackFlat)
{
    float signal[64];
    float inSpectrum[32];
    float low[64];
    float mid[64];
    float high[64];
    float out[64];
    float outSpectrum[32];
    float phase[32];
    
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
        ASSERT_NEAR(inSpectrum[i], outSpectrum[i], EPSILON);
    }
}


#pragma mark -
#pragma mark Double Precision Tests

TEST(MultibandBankDouble, TestMixBackFlat)
{
    double signal[64];
    double inSpectrum[32];
    double low[64];
    double mid[64];
    double high[64];
    double out[64];
    double outSpectrum[32];
    double phase[32];
    
    FFTConfigD*          fft;
    MultibandFilterD*    filter;
    
    // Create delta function
    FillBufferD(signal, 64, 0.0);
    signal[0] = 1.0;
    
    // Spectrum of input
    fft = FFTInitD(64);
    FFTForwardD(fft, signal, inSpectrum, phase);
    
    // process data
    filter = MultibandFilterInitD(1000, 12000, 44100);
    MultibandFilterProcessD(filter, low, mid, high, signal, 64);
    
    // Mix bands back together
    VectorVectorAddD(out, low, mid, 64);
    VectorVectorAddD(out, out, high, 64);
    
    // Calculate spectrum of output
    FFTForwardD(fft, out, outSpectrum, phase);
    
    // Cleanup
    FFTFreeD(fft);
    MultibandFilterFreeD(filter);
    
    
    // Verify output
    for (unsigned i = 0; i < 32; ++i)
    {
        ASSERT_NEAR(inSpectrum[i], outSpectrum[i], EPSILON);
    }
}


#endif