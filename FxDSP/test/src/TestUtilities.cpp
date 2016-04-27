//
//  TestUtilities.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/3/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "Utilities.h"
#include <math.h>
#include <gtest/gtest.h>

#define EPSILON (0.00001)


TEST(Utilities, TestAmpDbConversionMacros)
{
    float amps[6] = {1.0, 0.5, 0.25, 0.125, 0.0625, 0.0};
    float dbs[6] = {0.0,-6.0206003,-12.041201,-18.061801,-24.082401, -150.0};
    for (unsigned i = 0; i < 6; ++i)
    {
        ASSERT_NEAR(dbs[i], AMP_TO_DB(amps[i]), EPSILON);
        ASSERT_NEAR(amps[i], DB_TO_AMP(dbs[i]), EPSILON);
        ASSERT_NEAR(dbs[i], AMP_TO_DBD(amps[i]), EPSILON);
        ASSERT_NEAR(amps[i], DB_TO_AMPD(dbs[i]), EPSILON);
    }
}

TEST(Utilities, TestClamp)
{
    float in[10] = {-10.0, -2.0, -1.1, -1.0, -0.5, 0.5, 1.0, 1.1, 2.0, 10.0};
    float ex[10] = {-1.0, -1.0, -1.0, -1.0, -0.5, 0.5, 1.0, 1.0, 1.0, 1.0};
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(ex[i], f_clamp(in[i], -1.0, 1.0));
    }
    
}

TEST(Utilities, TestFastPow2)
{
    float in[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(powf(2.0, in[i]), f_pow2(in[i]));
    }
}

TEST(Utilities, log2)
{
    double in[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_DOUBLE_EQ(log(in[i])/log(2.), log2(in[i]));
    }

}

TEST(Utilities, TestMaxAndMin)
{
    float a[10] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float b[10] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_FLOAT_EQ(a[i], f_min(a[i], b[i]));
        ASSERT_FLOAT_EQ(b[i], f_max(a[i], b[i]));
    }
}

TEST(Utilities, TestFastExp)
{
    float in[21] = {
        -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1,
        0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0
    };
    
    for (unsigned i = 0; i < 21; ++i)
    {
        ASSERT_NEAR(expf(in[i]), F_EXP(in[i]), 0.000001);
    }
}


TEST(Utilities, TestFastExpD)
{
    double in[21] = {
        -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1,
        0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0
    };
    
    for (unsigned i = 0; i < 21; ++i)
    {
        ASSERT_NEAR(exp(in[i]), F_EXP(in[i]), 0.000001);
    }
}

TEST(Utilities, TestNextPow2)
{
    int in[11] = {-2, 2, 3, 6, 12, 30, 33, 127, 129, 500, 1000};
    int expect[11] = {0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_EQ(expect[i], next_pow2(in[i]));
    }
}


TEST(Utilities, TestPolarRectangularConversion)
{
    double mag[10] = {0.0, 0.1, 0.2, 0.3, 0.4, -0.5, -0.6, -0.7, -0.8, -0.9};
    double phase[10] = {0.9, 0.8, 0.7, -0.6, -0.5, -0.4, -0.3, -0.2, 0.1, 0.0};
    float real[10];
    float im[10];
    double realD[10];
    double imD[10];
    
    for (unsigned i = 0; i < 10; ++i)
    {
        PolarToRect((float)mag[i], (float)phase[i], real+i, im+i);
        PolarToRectD(mag[i], phase[i], realD+i, imD + i);
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        
        ASSERT_FLOAT_EQ(mag[i] * cosf(phase[i]), real[i]);
        ASSERT_FLOAT_EQ(mag[i] * sinf(phase[i]), im[i]);
        ASSERT_DOUBLE_EQ(mag[i] * cos(phase[i]), realD[i]);
        ASSERT_DOUBLE_EQ(mag[i] * sin(phase[i]), imD[i]);
    }
}


TEST(Utilities, TestRectangularPolarConversion)
{
    double re[6] = {-1.0, -0.5, -0.25, 0.25, 0.5, 1.0};
    double im[6] = {-1.0, -0.5, -0.25, 0.25, 0.5, 1.0};
    float mag;
    float phase;
    double magD;
    double phaseD;
    
    //Try points in all 4 quadrants
    for (unsigned i = 0; i < 6; ++i)
    {
        for (unsigned j = 0; j < 6; ++j)
        {
            RectToPolar((float)re[i], (float)im[j], &mag, &phase);
            RectToPolarD(re[i], im[j], &magD, &phaseD);
            double exp_mag = sqrt(re[i]*re[i] + im[j]*im[j]);
            double exp_phase = atan(im[j]/re[i]);
            if (exp_phase < 0)
                exp_phase += M_PI;
            ASSERT_FLOAT_EQ((float)exp_mag, mag);
            ASSERT_FLOAT_EQ((float)exp_phase, phase);
            ASSERT_DOUBLE_EQ(exp_mag, magD);
            ASSERT_DOUBLE_EQ(exp_phase, phaseD);
        }
    }
}


TEST(Utilities, TestFastTanh)
{
    float in[10] = {-1.0, -0.5, -0.25, -0.125, -0.0625, 0.0625, 0.125, 0.25, 0.5, 1.0};
    for (unsigned i = 0; i < 10; ++i)
    {
        
        ASSERT_NEAR(tanhf(in[i]), f_tanh(in[i]), 0.0005);
    }
}


TEST(Utilities, TestInt16ToFloat)
{
    ASSERT_FLOAT_EQ(1.0, int16ToFloat(32767));
}

TEST(Utilities, TesFloatToInt16)
{
    ASSERT_EQ(32767, floatToInt16(1.0));
}


TEST(Utilities, TestDbAmpConversion)
{
    ASSERT_FLOAT_EQ(0.0, DbToAmp(-150.0));
    ASSERT_FLOAT_EQ(1.0, DbToAmp(0.0));
}

TEST(Utilities, TestDbAmpConversionDouble)
{
    ASSERT_DOUBLE_EQ(0.0, DbToAmpD(-150.0));
    ASSERT_DOUBLE_EQ(1.0, DbToAmpD(0.0));
}