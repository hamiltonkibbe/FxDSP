//
//  TestMetering.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/22/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#include "Metering.h"
#include "PanLaw.h"
#include "Signals.h"
#include <math.h>
#include <gtest/gtest.h>

#define EPSILON (0.0000001)

TEST(MeteringSingle, TestPhaseCorrelation)
{
    float left[30];
    float right[30];
    for (unsigned i = 0; i < 30; ++i)
    {
        left[i] = sinf(i * M_PI / 10.);
        right[i] = sinf(i * M_PI / 10.);
    }
    
    ASSERT_NEAR(1.0, phase_correlation(left, right, 10), EPSILON);
    ASSERT_NEAR(0.0, phase_correlation(left, right+5, 10), EPSILON);
    ASSERT_NEAR(-1.0, phase_correlation(left, right+10, 10), EPSILON);
    ASSERT_NEAR(0.0, phase_correlation(left, right+15, 10), EPSILON);
    ASSERT_NEAR(1.0, phase_correlation(left, right+20, 10), EPSILON);
}


TEST(MeteringSingle, TestBalance)
{
    float left[20];
    float right[20];
    float pan[5] = {0.0, 0.25, 0.5, 0.75, 1.0};
    float expect[5] = {-1.0, -sqrtf(2)/2, 0.0, sqrtf(2)/2, 1.0};
    float lg;
    float rg;
    for (unsigned ctrl = 0; ctrl < 5; ++ctrl)
    {
        equal_power_3dB_pan(pan[ctrl], &lg, &rg);
        for (unsigned i = 0; i < 20; ++i)
        {
            left[i] = lg *sinf(i * M_PI / 5.);
            right[i] = rg * sinf(i * M_PI / 5.);
        }
        ASSERT_FLOAT_EQ(expect[ctrl], balance(left, right, 20));
    }
}

TEST(MeteringSingle, TestVuPeak)
{
    float signal[1000];
    sinewave(signal, 1000, 1000.0, 0.0, 1.0, 48000);
    
    ASSERT_FLOAT_EQ(12.0, vu_peak(signal, 1000, K_12));
    ASSERT_FLOAT_EQ(14.0, vu_peak(signal, 1000, K_14));
    ASSERT_FLOAT_EQ(20.0, vu_peak(signal, 1000, K_20));
}

TEST(MeteringDouble, TestPhaseCorrelation)
{
    double left[30];
    double right[30];
    for (unsigned i = 0; i < 30; ++i)
    {
        left[i] = sinf(i * M_PI / 10.);
        right[i] = sinf(i * M_PI / 10.);
    }
    
    ASSERT_NEAR(1.0, phase_correlationD(left, right, 10), EPSILON);
    ASSERT_NEAR(0.0, phase_correlationD(left, right+5, 10), EPSILON);
    ASSERT_NEAR(-1.0, phase_correlationD(left, right+10, 10), EPSILON);
    ASSERT_NEAR(0.0, phase_correlationD(left, right+15, 10), EPSILON);
    ASSERT_NEAR(1.0, phase_correlationD(left, right+20, 10), EPSILON);
}

TEST(MeteringDouble, TestBalance)
{
    double left[20];
    double right[20];
    double pan[5] = {0.0, 0.25, 0.5, 0.75, 1.0};
    double expect[5] = {-1.0, -sqrt(2)/2, 0.0, sqrt(2)/2, 1.0};
    double lg;
    double rg;
    for (unsigned ctrl = 0; ctrl < 5; ++ctrl)
    {
        equal_power_3dB_panD(pan[ctrl], &lg, &rg);
        for (unsigned i = 0; i < 20; ++i)
        {
            left[i] = lg *sin(i * M_PI / 5.);
            right[i] = rg * sin(i * M_PI / 5.);
        }
        ASSERT_DOUBLE_EQ(expect[ctrl], balanceD(left, right, 20));
    }
}


TEST(MeteringDouble, TestVuPeak)
{
    double signal[1000];
    sinewaveD(signal, 1000, 1000.0, 0.0, 1.0, 48000);
    
    ASSERT_DOUBLE_EQ(12.0, vu_peakD(signal, 1000, K_12));
    ASSERT_DOUBLE_EQ(14.0, vu_peakD(signal, 1000, K_14));
    ASSERT_DOUBLE_EQ(20.0, vu_peakD(signal, 1000, K_20));
}
