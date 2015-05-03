//
//  TestDSP.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/8/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "Signals.h"
#include "TestFIRFilter.h"
#include "Dsp.h"
#include "Utilities.h"
#include <gtest/gtest.h>


TEST(DSP, FloatDoubleConversion)
{
    float in[10] = {1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0};
    double out[10];
    float fout[10];
    
    FloatToDouble(out, in, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ((double)in[i], out[i]);
    }
    
    DoubleToFloat(fout, out, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(in[i], fout[i]);
    }
}

TEST(DSPSingle, FloatIntConversion)
{
    short out[10];
    float in[10] = {1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0};
    
    FloatBufferToInt16(out, in, 10);
    
    for (unsigned i = 0; i < 5; i+=2)
    {
        ASSERT_EQ(32767, out[i]);
        ASSERT_EQ(-32767, out[i+1]);
    }
}

TEST(DSPSingle, IntFloatConversion)
{
    float out[10];
    short in[10] = {32767, -32767, 32767, -32767, 32767, -32767, 32767, -32767, 32767, -32767};
    
    Int16BufferToFloat(out, in, 10);
    
    for (unsigned i = 0; i < 5; i+=2)
    {
        ASSERT_FLOAT_EQ(1.0, out[i]);
        ASSERT_FLOAT_EQ(-1.0, out[i+1]);
    }
}


TEST(DSPSingle, TestFillBuffer)
{
    float out[10];
    FillBuffer(out, 10, 1.0);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(ones[i], out[i]);
    }
}

TEST(DSPSingle, TestClearBuffer)
{
    float out[10];
    for (unsigned i = 0; i < 10; ++i)
    {
        out[i] = i;
    }
    ClearBuffer(out, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(0.0, out[i]);
    }
}


TEST(DSPSingle, TestCopyBuffer)
{
    float out[10];
    CopyBuffer(out, ones, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(ones[i], out[i]);
    }
}


TEST(DSPSingle, TestSplitToInterleaved)
{
    float out[10];
    float real[5] = {-1.0, -0.6, -0.2, 0.2, 0.6};
    float imag[5] = {-0.8, -0.4, 0.0, 0.4, 0.8};
    SplitToInterleaved(out, real, imag, 5);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(ramp[i], out[i]);
    }
}

TEST(DSPSingle, TestInterleavedToSplit)
{
    float real[5];
    float imag[5];
    InterleavedToSplit(real, imag, ramp, 5);
    
    for (unsigned i = 0; i < 5; ++i)
    {
        ASSERT_FLOAT_EQ(ramp[2*i], real[i]);
        ASSERT_FLOAT_EQ(ramp[2 * i + 1], imag[i]);
    }
}


TEST(DSPSingle, TestVectorAbs)
{
    float out[10];
    VectorAbs(out, ramp, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(fabsf(ramp[i]), out[i]);
    }
}

TEST(DSPSingle, TestVectorNegate)
{
    float out[10];
    VectorNegate(out, ramp, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(-ramp[i], out[i]);
    }
}


TEST(DSPSingle, TestVectorVectorAdd)
{
    float out[10];
    VectorVectorAdd(out, zeros, ramp, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(ramp[i], out[i]);
    }
}


TEST(DSPSingle, TestVectorScalarAdd)
{
    float out[10];
    VectorScalarAdd(out, zeros, 1, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(ones[i], out[i]);
    }
}


TEST(DSPSingle, TestVectorVectorMultiply)
{
    float out[10];
    VectorVectorMultiply(out, ones, ramp, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(ramp[i], out[i]);
    }
}


TEST(DSPSingle, TestVectorScalarMultiply)
{
    float out[10];
    VectorScalarMultiply(out, ramp, 2.0, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(2.0 * ramp[i], out[i]);
    }
}


TEST(DSPSingle, TestVectorPower)
{
    float out[10];
    VectorPower(out, ramp, 2.0, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(powf(ramp[i], 2.0), out[i]);
    }
}



TEST(DSPSingle, TestConvolve)
{
    float out[121];
    float in1[5] =
    {
        0.0, 0.5, 1.0, 0.5, 0.0
    };
    
    float in2[6] =
    {
        0.5, 0.5, 0.5, 1.0, 1.0, 1.0
    };
    
    // >> conv([0 0.5 1 0.5 0], [0.5 0.5 0.5 1.0 1.0 1.0])
    float res[10] =
    {
        0.0, 0.25, 0.75, 1.0, 1.25, 1.75, 2.0, 1.5, 0.5, 0.0
    };
    
    Convolve(in1, 5, in2, 6, out);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(res[i], out[i]);
    }
}

TEST(DSPSingle, TestDBConversion)
{
    float out[5];
    float in[5] = {1.0, 0.5, 0.25, 0.125, 0.0625};
    float expected[5] = {0.0, -6.0206003, -12.041201, -18.061801, -24.082401};
    VectorDbConvert(out, in, 1, 5);
    for (unsigned i = 0; i < 5; ++i)
    {
        ASSERT_FLOAT_EQ(expected[i], out[i]);
    }
}


TEST(DSPSingle, TestComplexMultiply)
{
    float re1[10] = {1, 2, 3, 4, 5, -1, -2, -3, -4, -5};
    float im1[10] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    float re2[10] = {1, 2, 4, 8, 16, -1, -2, -4, -8, -16};
    float im2[10] = {-1, -2, 4, 8, 16, 1, 2, -4, -8, -16};
    float result_re[10];
    float result_im[10];
    float expected_re[10] = {2., 8., 0., 0., 0., 0., 0., 24., 64., 160.};
    float expected_im[10] = {0., 0., 24., 64., 160., -2., -8., 0., 0., 0.};
    ComplexMultiply(result_re, result_im, re1, im1, re2, im2, 10);
    for(unsigned i = 0; i < 10; ++i)
    {
        //printf("GOT %f + j%f\n", result_re[i], result_im[i]);
        ASSERT_FLOAT_EQ(expected_re[i], result_re[i]);
        ASSERT_FLOAT_EQ(expected_im[i], result_im[i]);
    }
    
}


#pragma mark -
#pragma mark Double Precision Tests


TEST(DSPDouble, DoubleIntConversion)
{
    short out[10];
    double in[10] = {1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0};
    
    DoubleBufferToInt16(out, in, 10);
    
    for (unsigned i = 0; i < 5; i+=2)
    {
        ASSERT_EQ(32767, out[i]);
        ASSERT_EQ(-32767, out[i+1]);
    }
}

TEST(DSPDouble, IntDoubleConversion)
{
    double out[10];
    short in[10] = {32767, -32767, 32767, -32767, 32767, -32767, 32767, -32767, 32767, -32767};
    
    Int16BufferToDouble(out, in, 10);
    
    for (unsigned i = 0; i < 5; i+=2)
    {
        ASSERT_DOUBLE_EQ(1.0, out[i]);
        ASSERT_DOUBLE_EQ(-1.0, out[i+1]);
    }
}



TEST(DSPDouble, TestFillBuffer)
{
    double out[10];
    FillBufferD(out, 10, 1.0);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(onesD[i], out[i]);
    }
}

TEST(DSPDouble, TestClearBuffer)
{
    double out[10];
    for (unsigned i = 0; i < 10; ++i)
    {
        out[i] = i;
    }
    ClearBufferD(out, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(0.0, out[i]);
    }
}


TEST(DSPDouble, TestCopyBuffer)
{
    double out[10];
    CopyBufferD(out, onesD, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(onesD[i], out[i]);
    }
}


TEST(DSPDouble, TestSplitToInterleaved)
{
    double out[10];
    double real[5] = {rampD[0], rampD[2], rampD[4], rampD[6], rampD[8]};
    double imag[5] = {rampD[1], rampD[3], rampD[5], rampD[7], rampD[9]};
    SplitToInterleavedD(out, real, imag, 5);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(rampD[i], out[i]);
    }
}


TEST(DSPDouble , TestInterleavedToSplit)
{
    double real[5];
    double imag[5];
    InterleavedToSplitD(real, imag, rampD, 5);
    
    for (unsigned i = 0; i < 5; ++i)
    {
        ASSERT_FLOAT_EQ(rampD[2 * i], real[i]);
        ASSERT_FLOAT_EQ(rampD[2 * i + 1], imag[i]);
    }
}

TEST(DSPDouble, TestVectorAbs)
{
    double out[10];
    VectorAbsD(out, rampD, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(fabs(rampD[i]), out[i]);
    }
}

TEST(DSPDouble, TestVectorNegate)
{
    double out[10];
    VectorNegateD(out, rampD, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(-rampD[i], out[i]);
    }
}


TEST(DSPDouble, TestVectorVectorAdd)
{
    double out[10];
    VectorVectorAddD(out, zerosD, rampD, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(rampD[i], out[i]);
    }
}


TEST(DSPDouble, TestVectorScalarAdd)
{
    double out[10];
    VectorScalarAddD(out, zerosD, 1, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(ones[i], out[i]);
    }
}


TEST(DSPDouble, TestVectorVectorMultiply)
{
    double out[10];
    VectorVectorMultiplyD(out, onesD, rampD, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(rampD[i], out[i]);
    }
}


TEST(DSPDouble, TestVectorScalarMultiply)
{
    double out[10];
    VectorScalarMultiplyD(out, rampD, 2.0, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(2.0 * rampD[i], out[i]);
    }
}


TEST(DSPDouble, TestVectorPower)
{
    double out[10];
    VectorPowerD(out, rampD, 2.0, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(pow(rampD[i], 2.0), out[i]);
    }
}

TEST(DSPDouble, TestConvolve)
{
    double out[10];
    double in1[5] =
    {
        0.0, 0.5, 1.0, 0.5, 0.0
    };
    
    double in2[6] =
    {
        0.5, 0.5, 0.5, 1.0, 1.0, 1.0
    };
    
    // >> conv([0 0.5 1 0.5 0], [0.5 0.5 0.5 1.0 1.0 1.0])
    double res[10] =
    {
        0.0, 0.25, 0.75, 1.0, 1.25, 1.75, 2.0, 1.5, 0.5, 0.0
    };
    
    ConvolveD(in1, 5, in2, 6, out);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(res[i], out[i]);
    }
}


TEST(DSPDouble, TestComplexMultiply)
{
    double re1[10] = {1, 2, 3, 4, 5, -1, -2, -3, -4, -5};
    double im1[10] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    double re2[10] = {1, 2, 4, 8, 16, -1, -2, -4, -8, -16};
    double im2[10] = {-1, -2, 4, 8, 16, 1, 2, -4, -8, -16};
    double result_re[10];
    double result_im[10];
    double expected_re[10] = {2., 8., 0., 0., 0., 0., 0., 24., 64., 160.};
    double expected_im[10] = {0., 0., 24., 64., 160., -2., -8., 0., 0., 0.};
    ComplexMultiplyD(result_re, result_im, re1, im1, re2, im2, 10);
    for(unsigned i = 0; i < 10; ++i)
    {
       // printf("GOT %f + j%f\n", result_re[i], result_im[i]);
        ASSERT_FLOAT_EQ(expected_re[i], result_re[i]);
        ASSERT_FLOAT_EQ(expected_im[i], result_im[i]);
    }
    
}