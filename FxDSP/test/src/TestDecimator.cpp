//
//  TestDecimator.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/25/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#include "Decimator.h"
#include <math.h>
#include <gtest/gtest.h>

TEST(DecimatorSingle, TestDecimator)
{
    float in[800];
    float out[400];
    float expected[400];
    for (unsigned i = 0; i < 800; ++i)
    {
        in[i] = sinf(i * M_PI / 80.0);
    }
    
    
    Decimator* ds = DecimatorInit(X2);
    DecimatorProcess(ds, out, in, 800);
    DecimatorFree(ds);
    
    float *residx = out+32;

    for(unsigned i = 0; i < 400; ++i)
    {
        expected[i] = sinf(i*M_PI/40.0);
    }

    for (unsigned i = 0; i < 400- 32; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
        
    }
  
    ds = DecimatorInit(X4);
    DecimatorProcess(ds, out, in, 800);
    DecimatorFree(ds);
    
    
    for(unsigned i = 0; i < 200; ++i)
    {
        expected[i] = sinf(i*M_PI/20.0);
    }
    
    for (unsigned i = 0; i < 200 - 32; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }
}




TEST(DecimatorDouble, TestDecimator)
{
    double in[800];
    double out[400];
    double expected[400];
    
    for (unsigned i = 0; i < 800; ++i)
    {
        in[i] = sin(i * M_PI / 80.0);
    }


    DecimatorD* ds = DecimatorInitD(X2);
    DecimatorProcessD(ds, out, in, 800);
    DecimatorFreeD(ds);

    double *residx = out+32;

    for(unsigned i = 0; i < 400; ++i)
    {
        expected[i] = sinf(i*M_PI/40.0);
    }

    for (unsigned i = 0; i < 400- 32; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }

    ds = DecimatorInitD(X4);
    DecimatorProcessD(ds, out, in, 800);
    DecimatorFreeD(ds);


    for(unsigned i = 0; i < 200; ++i)
    {
        expected[i] = sinf(i*M_PI/20.0);
    }

    for (unsigned i = 0; i < 200 - 32; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }
}
