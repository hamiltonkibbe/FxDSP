//
//  TestFFT.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/12/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "gtest/gtest.h"
#include "testFFT.h"
#include "FFT.h"
#include "Dsp.h"

#define EPSILON (0.000001)

#pragma mark -
#pragma mark Double Precision Tests

TEST(FFTSingle, TestForwardFFTAgainstMatlab)
{
    float magnitude[32];
    float phase[32];
    
    
    FFTConfig* fft = FFTInit(64);
    
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFTForward(fft, matlabInputVector, magnitude, phase);
        FFTFree(fft);
        for (unsigned i = 0; i < 32; ++i)
        {
            ASSERT_FLOAT_EQ(matlabMagnitude[i], magnitude[i]);
            ASSERT_FLOAT_EQ(matlabPhase[i], phase[i]);
        }
    }
}


TEST(FFTSingle, TestForwardToInverseFFT)
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
    
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFTForward(fft, signal, magnitude, phase);
        FFTInverse(fft, magnitude, phase, output);
        FFTFree(fft);
        for (unsigned i = 0; i < 64; ++i)
        {
            ASSERT_NEAR(signal[i], output[i], EPSILON);
        }
    }
}



TEST(FFTSingle, TestFFTConvolution)
{
    float output[15];
    float in[3] = {1.0, 2.0, 3.0};
    float in2[4] = {4.0, 5.0, 6.0, 7.0};
    
    FFTConfig* fft = FFTInit(16);
    
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFTConvolve(fft, in, 3, in2, 4, output);
        FFTFree(fft);
        for (unsigned i = 0; i < 15; ++i)
        {
            ASSERT_NEAR(matlabConvolution[i], output[i], 0.00025);
        }
    }
}

TEST(FFTSingle, TestFFTFilterConvolution)
{
    float in[3] = {1.0, 2.0, 3.0};
    float in2[4] = {4.0,5.0,6.0,7};
    float padded[16];
    float dest[16];
    FillBuffer(padded, 16, 0.0);
    CopyBuffer(padded, in2, 4);
    
    FFTSplitComplex splitcomplex;
    splitcomplex.realp = (float*) malloc(16 * sizeof(float));
    splitcomplex.imagp = splitcomplex.realp + 8;
    
    FFTConfig* fft = FFTInit(16);
    
    ASSERT_TRUE(fft);
    if (fft)
    {
        FFTForwardSplit(fft, (FFTComplex*)padded, &splitcomplex);
        FFTFilterConvolve(fft, in, 3, splitcomplex, dest);
        FFTFree(fft);
        for (unsigned i = 0; i < 15; ++i)
        {
            ASSERT_NEAR(matlabConvolution[i], dest[i], 0.0025);
        }
    }
}


#pragma mark -
#pragma mark Double Precision Tests

TEST(FFTDouble, TestForwardFFTAgainstMatlab)
{
    double magnitude[32];
    double phase[32];
    
    FFTConfigD* fft = FFTInitD(64);
    
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFTForwardD(fft, matlabInputVectorD, magnitude, phase);
        FFTFreeD(fft);
        for (unsigned i = 0; i < 32; ++i)
        {
            ASSERT_FLOAT_EQ(matlabMagnitudeD[i], magnitude[i]);
            ASSERT_FLOAT_EQ(matlabPhaseD[i], phase[i]);
        }
    }
}


TEST(FFTDouble, TestForwardToInverseFFT)
{
    double signal[64];
    double magnitude[32];
    double phase[32];
    double output[64];
    
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = sin(((4*M_PI)/64) * i) + 0.5 * sin(((8*M_PI)/64) * i);
    }
    
    FFTConfigD* fft = FFTInitD(64);
    
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFTForwardD(fft, signal, magnitude, phase);
        FFTInverseD(fft, magnitude, phase, output);
        FFTFreeD(fft);
        for (unsigned i = 0; i < 64; ++i)
        {
            ASSERT_NEAR(signal[i], output[i], EPSILON);
        }
    }
}


TEST(FFTDouble, TestFFTConvolution)
{
    double output[16];
    double in[3] = {1.0, 2.0, 3.0};
    double in2[4] = {4.0, 5.0, 6.0, 7.0};
    
    FFTConfigD* fft = FFTInitD(16);
    
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFTConvolveD(fft, in, 3, in2, 4, output);
        FFTFreeD(fft);
        for (unsigned i = 0; i < 15; ++i)
        {
            ASSERT_NEAR(matlabConvolutionD[i], output[i], EPSILON);
        }
    }
}

TEST(FFTDouble, TestFFTFilterConvolution)
{
    double in[3] = {1.0, 2.0, 3.0};
    double in2[4] = {4.0,5.0,6.0,7};
    double padded[16];
    double dest[16];
    FillBufferD(padded, 16, 0.0);
    CopyBufferD(padded, in2, 4);
    
    FFTSplitComplexD splitcomplex;
    splitcomplex.realp = (double*) malloc(16 * sizeof(double));
    splitcomplex.imagp = splitcomplex.realp + 8;

    FFTConfigD* fft = FFTInitD(16);
    
    ASSERT_TRUE(fft);
    if (fft)
    {
        FFTForwardSplitD(fft, (FFTComplexD*)padded, &splitcomplex);
        FFTFilterConvolveD(fft, in, 3, splitcomplex, dest);
        FFTFreeD(fft);
        for (unsigned i = 0; i < 15; ++i)
        {
            ASSERT_NEAR(matlabConvolutionD[i], dest[i], EPSILON);
        }
    }
}
