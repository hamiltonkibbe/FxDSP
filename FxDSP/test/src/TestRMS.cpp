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