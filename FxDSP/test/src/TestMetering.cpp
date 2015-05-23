//
//  TestMetering.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/22/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#include "Metering.h"
#include <math.h>
#include <gtest/gtest.h>

#define EPSILON (0.0000001)

TEST(MeteringSingle, TestPhaseCorrelation)
{
    float leftsignal[30];
    float rightsignal[30];
    for (unsigned i = 0; i < 30; ++i)
    {
        leftsignal[i] = sinf(i * M_PI / 10.);
        rightsignal[i] = sinf(i * M_PI / 10.);
    }
    
    ASSERT_NEAR(1.0, phase_correlation(leftsignal, rightsignal, 10), EPSILON);
    ASSERT_NEAR(0.0, phase_correlation(leftsignal, rightsignal+5, 10), EPSILON);
    ASSERT_NEAR(-1.0, phase_correlation(leftsignal, rightsignal+10, 10), EPSILON);
    ASSERT_NEAR(0.0, phase_correlation(leftsignal, rightsignal+15, 10), EPSILON);
    ASSERT_NEAR(1.0, phase_correlation(leftsignal, rightsignal+20, 10), EPSILON);
}






TEST(MeteringDouble, TestPhaseCorrelation)
{
    double leftsignal[30];
    double rightsignal[30];
    for (unsigned i = 0; i < 30; ++i)
    {
        leftsignal[i] = sinf(i * M_PI / 10.);
        rightsignal[i] = sinf(i * M_PI / 10.);
    }
    
    ASSERT_NEAR(1.0, phase_correlationD(leftsignal, rightsignal, 10), EPSILON);
    ASSERT_NEAR(0.0, phase_correlationD(leftsignal, rightsignal+5, 10), EPSILON);
    ASSERT_NEAR(-1.0, phase_correlationD(leftsignal, rightsignal+10, 10), EPSILON);
    ASSERT_NEAR(0.0, phase_correlationD(leftsignal, rightsignal+15, 10), EPSILON);
    ASSERT_NEAR(1.0, phase_correlationD(leftsignal, rightsignal+20, 10), EPSILON);
}