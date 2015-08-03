//
//  TestMultibandFilterBank.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/12/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#include "MultibandBank.h"
#include "FFT.h"
#include "Dsp.h"
#include "Utilities.h"
#include "Signals.h"

#include <gtest/gtest.h>
#include <cmath>


#define EPSILON (0.02)

#pragma mark -
#pragma mark Single Precision Tests


TEST(MultibandBankSingle, TestFlush)
{
    float signal[64];
    float low1[64];
    float low2[64];
    float mid1[64];
    float mid2[64];
    float high1[64];
    float high2[64];
    sinewave(signal, 64, 2000, 0, 1, 44100);
    MultibandFilter * filter = MultibandFilterInit(1000, 12000, 44100);
    MultibandFilterProcess(filter, low1, mid1, high1, signal, 64);
    MultibandFilterFlush(filter);
    MultibandFilterProcess(filter, low2, mid2, high2, signal, 64);
    MultibandFilterFree(filter);
    
    for (unsigned i = 0; i < 64; ++i)
    {
        ASSERT_FLOAT_EQ(low1[i], low2[i]);
        ASSERT_FLOAT_EQ(mid1[i], mid2[i]);
        ASSERT_FLOAT_EQ(high1[i], high2[i]);
    }
}

TEST(MultibandBankSingle, TestSetters)
{
    float signal[64];
    float low1[64];
    float low2[64];
    float mid1[64];
    float mid2[64];
    float high1[64];
    float high2[64];
    sinewave(signal, 64, 2000, 0, 1, 44100);
    MultibandFilter * filter = MultibandFilterInit(1000, 12000, 44100);
    MultibandFilter * filter2 = MultibandFilterInit(500, 10000, 44100);
    MultibandFilterSetLowCutoff(filter2, 1000);
    MultibandFilterSetHighCutoff(filter2, 12000);
    MultibandFilterProcess(filter, low1, mid1, high1, signal, 64);
    MultibandFilterProcess(filter2, low2, mid2, high2, signal, 64);
    MultibandFilterFree(filter);
    MultibandFilterFree(filter2);
    
    for (unsigned i = 0; i < 64; ++i)
    {
        ASSERT_FLOAT_EQ(low1[i], low2[i]);
        ASSERT_FLOAT_EQ(mid1[i], mid2[i]);
        ASSERT_FLOAT_EQ(high1[i], high2[i]);
    }}

TEST(MultibandBankSingle, TestUpdate)
{
    float signal[64];
    float low1[64];
    float low2[64];
    float mid1[64];
    float mid2[64];
    float high1[64];
    float high2[64];
    sinewave(signal, 64, 2000, 0, 1, 44100);
    MultibandFilter * filter = MultibandFilterInit(1000, 12000, 44100);
    MultibandFilter * filter2 = MultibandFilterInit(500, 10000, 44100);
    MultibandFilterUpdate(filter2, 1000, 12000);
    MultibandFilterProcess(filter, low1, mid1, high1, signal, 64);
    MultibandFilterProcess(filter2, low2, mid2, high2, signal, 64);
    MultibandFilterFree(filter);
    MultibandFilterFree(filter2);
    
    for (unsigned i = 0; i < 64; ++i)
    {
        ASSERT_FLOAT_EQ(low1[i], low2[i]);
        ASSERT_FLOAT_EQ(mid1[i], mid2[i]);
        ASSERT_FLOAT_EQ(high1[i], high2[i]);
    }
}


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
    FFT_R2C(fft, signal, inSpectrum, phase);
    for (unsigned i = 0; i < 32; ++i)
    {
        float real = inSpectrum[i];
        float imag = phase[i];
        RectToPolar(real, imag, &inSpectrum[i], &phase[i]);
    }
    
    // process data
    filter = MultibandFilterInit(1000, 12000, 44100);
    MultibandFilterProcess(filter, low, mid, high, signal, 64);
    
    // Mix bands back together
    VectorVectorAdd(out, low, mid, 64);
    VectorVectorAdd(out, out, high, 64);
    
    // Calculate spectrum of output
    FFT_R2C(fft, out, outSpectrum, phase);
    for (unsigned i = 0; i < 32; ++i)
    {
        float real = outSpectrum[i];
        float imag = phase[i];
        RectToPolar(real, imag, &outSpectrum[i], &phase[i]);
    }
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


TEST(MultibandBankDouble, TestFlush)
{
    double signal[64];
    double low1[64];
    double low2[64];
    double mid1[64];
    double mid2[64];
    double high1[64];
    double high2[64];
    sinewaveD(signal, 64, 2000, 0, 1, 44100);
    MultibandFilterD * filter = MultibandFilterInitD(1000, 12000, 44100);
    MultibandFilterProcessD(filter, low1, mid1, high1, signal, 64);
    MultibandFilterFlushD(filter);
    MultibandFilterProcessD(filter, low2, mid2, high2, signal, 64);
    MultibandFilterFreeD(filter);
    
    for (unsigned i = 0; i < 64; ++i)
    {
        ASSERT_DOUBLE_EQ(low1[i], low2[i]);
        ASSERT_DOUBLE_EQ(mid1[i], mid2[i]);
        ASSERT_DOUBLE_EQ(high1[i], high2[i]);
    }
}


TEST(MultibandBankDouble, TestSetters)
{
    double signal[64];
    double low1[64];
    double low2[64];
    double mid1[64];
    double mid2[64];
    double high1[64];
    double high2[64];
    sinewaveD(signal, 64, 2000, 0, 1, 44100);
    MultibandFilterD * filter = MultibandFilterInitD(1000, 12000, 44100);
    MultibandFilterD * filter2 = MultibandFilterInitD(500, 10000, 44100);
    MultibandFilterSetLowCutoffD(filter2, 1000);
    MultibandFilterSetHighCutoffD(filter2, 12000);
    MultibandFilterProcessD(filter, low1, mid1, high1, signal, 64);
    MultibandFilterProcessD(filter2, low2, mid2, high2, signal, 64);
    MultibandFilterFreeD(filter);
    MultibandFilterFreeD(filter2);
    
    for (unsigned i = 0; i < 64; ++i)
    {
        ASSERT_DOUBLE_EQ(low1[i], low2[i]);
        ASSERT_DOUBLE_EQ(mid1[i], mid2[i]);
        ASSERT_DOUBLE_EQ(high1[i], high2[i]);
    }
}

TEST(MultibandBankDouble, TestUpdate)
{
    double signal[64];
    double low1[64];
    double low2[64];
    double mid1[64];
    double mid2[64];
    double high1[64];
    double high2[64];
    sinewaveD(signal, 64, 2000, 0, 1, 44100);
    MultibandFilterD * filter = MultibandFilterInitD(1000, 12000, 44100);
    MultibandFilterD * filter2 = MultibandFilterInitD(500, 10000, 44100);
    MultibandFilterUpdateD(filter2, 1000, 12000);
    MultibandFilterProcessD(filter, low1, mid1, high1, signal, 64);
    MultibandFilterProcessD(filter2, low2, mid2, high2, signal, 64);
    MultibandFilterFreeD(filter);
    MultibandFilterFreeD(filter2);
    
    for (unsigned i = 0; i < 64; ++i)
    {
        ASSERT_DOUBLE_EQ(low1[i], low2[i]);
        ASSERT_DOUBLE_EQ(mid1[i], mid2[i]);
        ASSERT_DOUBLE_EQ(high1[i], high2[i]);
    }
}

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
    FFT_R2CD(fft, signal, inSpectrum, phase);
    for (unsigned i = 0; i < 32; ++i)
    {
        double real = inSpectrum[i];
        double imag = phase[i];
        RectToPolarD(real, imag, &inSpectrum[i], &phase[i]);
    }
    
    // process data
    filter = MultibandFilterInitD(1000, 12000, 44100);
    MultibandFilterProcessD(filter, low, mid, high, signal, 64);
    
    // Mix bands back together
    VectorVectorAddD(out, low, mid, 64);
    VectorVectorAddD(out, out, high, 64);
    
    // Calculate spectrum of output
    FFT_R2CD(fft, out, outSpectrum, phase);
    for (unsigned i = 0; i < 32; ++i)
    {
        double real = outSpectrum[i];
        double imag = phase[i];
        RectToPolarD(real, imag, &outSpectrum[i], &phase[i]);
    }
    
    // Cleanup
    FFTFreeD(fft);
    MultibandFilterFreeD(filter);
    
    
    // Verify output
    for (unsigned i = 0; i < 32; ++i)
    {
        ASSERT_NEAR(inSpectrum[i], outSpectrum[i], EPSILON);
    }
}
