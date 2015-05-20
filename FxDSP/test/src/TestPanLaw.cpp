//
//  TestPanLaw.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/3/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "PanLaw.h"
#include <math.h>
#include <gtest/gtest.h>


TEST(PanLawSingle, TestLinearPan)
{
    float left = 0.0;
    float right = 0.0;
    linear_pan(0, &left, &right);
    ASSERT_FLOAT_EQ(1.0, left);
    ASSERT_FLOAT_EQ(0.0, right);
    
    linear_pan(0.5, &left, &right);
    ASSERT_FLOAT_EQ(0.5, left);
    ASSERT_FLOAT_EQ(0.5, right);
    
    linear_pan(1.0, &left, &right);
    ASSERT_FLOAT_EQ(0.0, left);
    ASSERT_FLOAT_EQ(1.0, right);
}


TEST(PanLawSingle, Test3dBPan)
{
    float left = 0.0;
    float right = 0.0;
    float sqrt2 = sqrtf(2.0);
    
    
    equal_power_3dB_pan(0.0, &left, &right);
    ASSERT_FLOAT_EQ(1.0, left);
    ASSERT_FLOAT_EQ(0.0, right);
    
    equal_power_3dB_pan(0.5, &left, &right);
    ASSERT_FLOAT_EQ(sqrt2/2., left);
    ASSERT_FLOAT_EQ(sqrt2/2., right);
    
    equal_power_3dB_pan(1.0, &left, &right);
    ASSERT_FLOAT_EQ(0.0, left);
    ASSERT_FLOAT_EQ(1.0, right);
}


TEST(PanLawSingle, Test6dBPan)
{
    float left = 0.0;
    float right = 0.0;
    
    equal_power_6dB_pan(0.0, &left, &right);
    ASSERT_FLOAT_EQ(1.0, left);
    ASSERT_FLOAT_EQ(0.0, right);
    
    equal_power_6dB_pan(0.5, &left, &right);
    ASSERT_FLOAT_EQ(0.5, left);
    ASSERT_FLOAT_EQ(0.5, right);
    
    equal_power_6dB_pan(1.0, &left, &right);
    ASSERT_FLOAT_EQ(0.0, left);
    ASSERT_FLOAT_EQ(1.0, right);
}


TEST(PanLawDouble, TestLinearPan)
{
    double left = 0.0;
    double right = 0.0;
    linear_panD(0, &left, &right);
    ASSERT_DOUBLE_EQ(1.0, left);
    ASSERT_DOUBLE_EQ(0.0, right);
    
    linear_panD(0.5, &left, &right);
    ASSERT_DOUBLE_EQ(0.5, left);
    ASSERT_DOUBLE_EQ(0.5, right);
    
    linear_panD(1.0, &left, &right);
    ASSERT_DOUBLE_EQ(0.0, left);
    ASSERT_DOUBLE_EQ(1.0, right);
}


TEST(PanLawDouble, Test3dBPan)
{
    double left = 0.0;
    double right = 0.0;
    double sqrt2 = sqrt(2.0);
    
    
    equal_power_3dB_panD(0.0, &left, &right);
    ASSERT_DOUBLE_EQ(1.0, left);
    ASSERT_DOUBLE_EQ(0.0, right);
    
    equal_power_3dB_panD(0.5, &left, &right);
    ASSERT_DOUBLE_EQ(sqrt2/2., left);
    ASSERT_DOUBLE_EQ(sqrt2/2., right);
    
    equal_power_3dB_panD(1.0, &left, &right);
    ASSERT_DOUBLE_EQ(0.0, left);
    ASSERT_DOUBLE_EQ(1.0, right);
}


TEST(PanLawDouble, Test6dBPan)
{
    double left = 0.0;
    double right = 0.0;
    
    equal_power_6dB_panD(0.0, &left, &right);
    ASSERT_DOUBLE_EQ(1.0, left);
    ASSERT_DOUBLE_EQ(0.0, right);
    
    equal_power_6dB_panD(0.5, &left, &right);
    ASSERT_DOUBLE_EQ(0.5, left);
    ASSERT_DOUBLE_EQ(0.5, right);
    
    equal_power_6dB_panD(1.0, &left, &right);
    ASSERT_DOUBLE_EQ(0.0, left);
    ASSERT_DOUBLE_EQ(1.0, right);
}

