//
//  TestFFT.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/12/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "testFFT.h"
#include "FFT.h"
#include "Dsp.h"
#include <gtest/gtest.h>
#include <cmath>

#define EPSILON (0.00001)


#pragma mark - Single Precision Tests


TEST(FFTSingle, TestFFT)
{
    float real[32];
    float imag[32];
    FFTConfig* fft = FFTInit(64);
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFT_R2C(fft, matlabInputVector, real, imag);
        FFTFree(fft);
        for (unsigned i = 0; i < 32; ++i)
        {
            ASSERT_NEAR(matlabReal[i], real[i], 0.0001);
            ASSERT_NEAR(matlabImag[i], imag[i], 0.0001);
        }
    }
    
}

TEST(FFTSingle, TestIFFT)
{
    float result[64];
    FFTConfig* fft = FFTInit(64);
    ASSERT_TRUE(fft);
    if(fft)
    {
        IFFT_C2R(fft, matlabReal, matlabImag, result);
        FFTFree(fft);
        
        for (unsigned i = 0; i < 64; ++i)
        {
            ASSERT_NEAR(matlabInputVector[i], result[i], 0.0001);
        }
    }
}



TEST(FFTSingle, TestForwardToInverseFFT)
{
    float signal[64];
    float real[32];
    float imag[32];
    float output[64];
    
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = sinf(((4*M_PI)/64) * i) + 0.5 * sinf(((8*M_PI)/64) * i);
    }
    
    FFTConfig* fft = FFTInit(64);
    
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFT_R2C(fft, signal, real, imag);
        IFFT_C2R(fft, real, imag, output);
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
            ASSERT_NEAR(matlabConvolution[i], output[i], EPSILON);
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
    
    if (!fft)
    {
        free(splitcomplex.realp);
        ASSERT_TRUE(fft);   // Still want this test to fail
    }
    
    else
    {
        FFT_IR_R2C(fft, padded, splitcomplex);
        FFTFilterConvolve(fft, in, 3, splitcomplex, dest);
        FFTFree(fft);
        for (unsigned i = 0; i < 15; ++i)
        {
            ASSERT_NEAR(matlabConvolution[i], dest[i], EPSILON);
        }
    }

}


#pragma mark - Double Precision Tests

TEST(FFTDouble, TestFFT)
{
    double real[32];
    double imag[32];
    FFTConfigD* fft = FFTInitD(64);
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFT_R2CD(fft, matlabInputVectorD, real, imag);
        FFTFreeD(fft);
        for (unsigned i = 0; i < 32; ++i)
        {
            ASSERT_NEAR(matlabRealD[i], real[i], 0.0001);
            ASSERT_NEAR(matlabImagD[i], imag[i], 0.0001);
        }
    }
    
}


TEST(FFTDouble, TestIFFT)
{
    double result[64];
    FFTConfigD* fft = FFTInitD(64);
    ASSERT_TRUE(fft);
    if(fft)
    {
        IFFT_C2RD(fft, matlabRealD, matlabImagD, result);
        FFTFreeD(fft);
        for (unsigned i = 0; i < 64; ++i)
        {
            ASSERT_NEAR(matlabInputVectorD[i], result[i], 0.0001);
        }
    }
}

TEST(FFTDouble, TestForwardToInverseFFT)
{
    double signal[64];
    double real[32];
    double imag[32];
    double output[64];
    
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = sin(((4*M_PI)/64) * i) + 0.5 * sin(((8*M_PI)/64) * i);
    }
    
    FFTConfigD* fft = FFTInitD(64);
    
    ASSERT_TRUE(fft);
    if(fft)
    {
        FFT_R2CD(fft, signal, real, imag);
        IFFT_C2RD(fft, real, imag, output);
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
    
    if (!fft)
    {
        free(splitcomplex.realp);
        ASSERT_TRUE(fft);
    }
    else
    {
        FFT_IR_R2CD(fft, padded, splitcomplex);
        FFTFilterConvolveD(fft, in, 3, splitcomplex, dest);
        FFTFreeD(fft);
        free(splitcomplex.realp);
        for (unsigned i = 0; i < 15; ++i)
        {
            ASSERT_NEAR(matlabConvolutionD[i], dest[i], EPSILON);
        }
    }
}


