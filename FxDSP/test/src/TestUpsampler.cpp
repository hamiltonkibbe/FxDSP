//
//  TestUpsampler.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/25/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#include "Upsampler.h"
#include <math.h>
#include <gtest/gtest.h>

TEST(UpsamplerSingle, TestUpsampler)
{
    float in[200];
    float out[800];
    float expected[800];
    for (unsigned i = 0; i < 200; ++i)
    {
        in[i] = sinf(i * M_PI / 20.0);
    }
    

    Upsampler* us = UpsamplerInit(X2);
    UpsamplerProcess(us, out, in, 200);
    UpsamplerFree(us);
    
    float *residx = out+64;
    
    for(unsigned i = 0; i < 400; ++i)
    {
        expected[i] = sinf(i*M_PI/40.0);
    }

    for (unsigned i = 0; i < 400 - 64; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }

    us = UpsamplerInit(X4);
    UpsamplerProcess(us, out, in, 200);
    UpsamplerFree(us);
    
    residx = out + 128;
    
    for(unsigned i = 0; i < 800; ++i)
    {
        expected[i] = sinf(i*M_PI/80.0);
    }
    
    for (unsigned i = 0; i < 800 - 128; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }
}




TEST(UpsamplerDouble, TestUpsampler)
{
    double in[200];
    double out[800];
    float expected[800];
    for (unsigned i = 0; i < 200; ++i)
    {
        in[i] = sin(i * M_PI / 20.0);
    }
    
    
    UpsamplerD* us = UpsamplerInitD(X2);
    UpsamplerProcessD(us, out, in, 200);
    UpsamplerFreeD(us);
    
    double *residx = out+64;
    
    for(unsigned i = 0; i < 400; ++i)
    {
        expected[i] = sin(i*M_PI/40.0);
    }
    
    for (unsigned i = 0; i < 400 - 64; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }
    
    us = UpsamplerInitD(X4);
    UpsamplerProcessD(us, out, in, 200);
    UpsamplerFreeD(us);
    
    residx = out + 128;
    
    for(unsigned i = 0; i < 800; ++i)
    {
        expected[i] = sinf(i*M_PI/80.0);
    }
    
    for (unsigned i = 0; i < 800 - 128; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }
}
