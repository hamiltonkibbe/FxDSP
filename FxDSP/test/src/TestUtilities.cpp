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


TEST(Utilities, TestAmpDbConversion)
{
    float amps[5] = {1.0, 0.5, 0.25, 0.125, 0.0625};
    float dbs[5] = {0.0,-6.0206003,-12.041201,-18.061801,-24.082401};
    for (unsigned i = 0; i < 5; ++i)
    {
        ASSERT_NEAR(dbs[i], AmpToDb(amps[i]), EPSILON);
        ASSERT_NEAR(amps[i], DbToAmp(dbs[i]), EPSILON);
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


TEST(Utilities, TestExpD)
{
    double in[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(exp(in[i]), f_expD(in[i]), 0.000001);
    }
}

