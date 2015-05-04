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


TEST(Utilities, AmpDbConversion)
{
    float amps[5] = {1.0, 0.5, 0.25, 0.125, 0.0625};
    float dbs[5] = {0.0,-6.0206003,-12.041201,-18.061801,-24.082401};
    for (unsigned i = 0; i < 5; ++i)
    {
        ASSERT_NEAR(dbs[i], AmpToDb(amps[i]), EPSILON);
        ASSERT_NEAR(amps[i], DbToAmp(dbs[i]), EPSILON);
    }
}