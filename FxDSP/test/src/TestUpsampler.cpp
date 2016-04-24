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
    float out[1600];
    float expected[1600];
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
    
    us = UpsamplerInit(X8);
    UpsamplerProcess(us, out, in, 200);
    UpsamplerFree(us);
    
    residx = out + 256;
    
    for(unsigned i = 0; i < 1600; ++i)
    {
        expected[i] = sinf(i*M_PI/160.0);
    }
    
    for (unsigned i = 0; i < 1600 - 256; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }

    /* Test invalid argument handling */
    us = UpsamplerInit((ResampleFactor_t)10000);
    ASSERT_EQ((void*)us, (void*)NULL);
}

TEST(UpsamplerSingle, TestUpsamplerFlush)
{
    float in[200];
    float out1[400];
    float out2[400];
    for (unsigned i = 0; i < 200; ++i)
    {
        in[i] = sinf(i * M_PI / 20.0);
    }
    
    
    Upsampler* us = UpsamplerInit(X2);
    UpsamplerProcess(us, out1, in, 200);
    UpsamplerFlush(us);
    UpsamplerProcess(us, out2, in, 200);
    UpsamplerFree(us);
    
    
    for(unsigned i = 0; i < 400; ++i)
    {
        ASSERT_DOUBLE_EQ(out1[i], out2[i]);
    }
}


TEST(UpsamplerDouble, TestUpsampler)
{
    double in[200];
    double out[1600];
    double expected[1600];
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
    
    us = UpsamplerInitD(X8);
    UpsamplerProcessD(us, out, in, 200);
    UpsamplerFreeD(us);
    
    residx = out + 256;
    
    for(unsigned i = 0; i < 1600; ++i)
    {
        expected[i] = sin(i*M_PI/160.0);
    }
    
    for (unsigned i = 0; i < 1600 - 256; ++i)
    {
        ASSERT_NEAR(residx[i], expected[i], 0.1);
    }

    /* Test invalid argument handling */
    us = UpsamplerInitD((ResampleFactor_t)10000);
    ASSERT_EQ((void*)us, (void*)NULL);
}


TEST(UpsamplerDouble, TestUpsamplerFlush)
{
    double in[200];
    double out1[400];
    double out2[400];
    for (unsigned i = 0; i < 200; ++i)
    {
        in[i] = sin(i * M_PI / 20.0);
    }
    
    
    UpsamplerD* us = UpsamplerInitD(X2);
    UpsamplerProcessD(us, out1, in, 200);
    UpsamplerFlushD(us);
    UpsamplerProcessD(us, out2, in, 200);
    UpsamplerFreeD(us);

    
    for(unsigned i = 0; i < 400; ++i)
    {
        ASSERT_DOUBLE_EQ(out1[i], out2[i]);
    }
}