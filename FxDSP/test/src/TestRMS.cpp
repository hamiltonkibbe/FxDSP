//
//  TestRMS.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/3/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "RMSEstimator.h"
#include <math.h>
#include <gtest/gtest.h>


TEST(RMSEstimatorSingle, TestRMSEstimator)
{
    float sinewave[10000];
    float out[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sinf((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    RMSEstimator * rms = RMSEstimatorInit(0.01, 10000);
    RMSEstimatorProcess(rms, out, sinewave, 10000);
    RMSEstimatorFree(rms);
    for (unsigned i = 250; i < 10000; ++i)
    {
        ASSERT_NEAR(0.7071, out[i], 0.1);
    }
    
}

TEST(RMSEstimatorSingle, TestRMSEstimatorSetTime)
{
    float sinewave[10000];
    float out[10000];
    float out2[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sinf((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    RMSEstimator * rms = RMSEstimatorInit(0.01, 10000);
    RMSEstimator * rms2 = RMSEstimatorInit(0.05, 10000);
    RMSEstimatorSetAvgTime(rms2, 0.01);
    RMSEstimatorProcess(rms, out, sinewave, 10000);
    RMSEstimatorProcess(rms2, out2, sinewave, 10000);
    RMSEstimatorFree(rms);
    RMSEstimatorFree(rms2);
    for (unsigned i = 250; i < 10000; ++i)
    {
        ASSERT_FLOAT_EQ(out[i], out2[i]);
    }
}

TEST(RMSEstimatorSingle, TestRMSEstimatorTick)
{
    float sinewave[10000];
    float out[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sinf((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    RMSEstimator * rms = RMSEstimatorInit(0.01, 10000);

    for (unsigned i = 0; i < 10000; ++i)
    {
        out[i] = RMSEstimatorTick(rms, sinewave[i]);
    }

    RMSEstimatorFree(rms);

    for (unsigned i = 250; i < 10000; ++i)
    {
        ASSERT_NEAR(0.7071, out[i], 0.1);
    }
}

TEST(RMSEstimatorDouble, TestRMSEstimator)
{
    double sinewave[10000];
    double out[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sin((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    RMSEstimatorD * rms = RMSEstimatorInitD(0.01, 10000);
    RMSEstimatorProcessD(rms, out, sinewave, 10000);
    RMSEstimatorFreeD(rms);
    for (unsigned i = 250; i < 10000; ++i)
    {
        ASSERT_NEAR(0.7071, out[i], 0.1);
    }
    
}

TEST(RMSEstimatorDouble, TestRMSEstimatorSetAvgTime)
{
    double sinewave[10000];
    double out[10000];
    double out2[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sin((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    RMSEstimatorD * rms = RMSEstimatorInitD(0.01, 10000);
    RMSEstimatorD * rms2 = RMSEstimatorInitD(0.05, 10000);
    RMSEstimatorSetAvgTimeD(rms2, 0.01);
    RMSEstimatorProcessD(rms, out, sinewave, 10000);
    RMSEstimatorProcessD(rms2, out2, sinewave, 10000);
    RMSEstimatorFreeD(rms);
    RMSEstimatorFreeD(rms2);
    for (unsigned i = 250; i < 10000; ++i)
    {
        ASSERT_FLOAT_EQ(out[i], out2[i]);
    }
    
}

TEST(RMSEstimatorDouble, TestRMSEstimatorTick)
{
    double sinewave[10000];
    double out[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sin((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    RMSEstimatorD * rms = RMSEstimatorInitD(0.01, 10000);
    
    for (unsigned i = 0; i < 10000; ++i)
    {
        out[i] = RMSEstimatorTickD(rms, sinewave[i]);
    }
    
    RMSEstimatorFreeD(rms);
    
    for (unsigned i = 250; i < 10000; ++i)
    {
        ASSERT_NEAR(0.7071, out[i], 0.1);
    }
}
