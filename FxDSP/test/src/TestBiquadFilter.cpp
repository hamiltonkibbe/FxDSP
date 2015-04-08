//
//  TestBiquadFilter.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/7/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "gtest/gtest.h"
#include "BiquadFilter.h"
#include "Dsp.h"

static const float b[3] = {0.1, 0.2, 0.3};
static const float a[2] = {0.1, 0.2};

static const double bD[3] = {0.1, 0.2, 0.3};
static const double aD[2] = {0.1, 0.2};

static const float input[10] =
{
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

static const double inputD[10] =
{
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

static const float MatlabOutput[10] =
{
    0.100000, 0.290000, 0.551000, 0.486900, 0.441110,
    0.458509, 0.465927, 0.461706, 0.460644, 0.461594
};

static const double MatlabOutputD[10] =
{
    0.100000, 0.290000, 0.551000, 0.486900, 0.441110,
    0.458509, 0.465927, 0.461706, 0.460644, 0.461594
};

TEST(BiquadFilterSingle, TestResultsAgainstMatlab)
{
    // Set up
    float output[10];
    ClearBuffer(output, 10);
    BiquadFilter *filter = BiquadFilterInit(b, a);
    
    // Process
    BiquadFilterProcess(filter, output, input, 10);
    
    // Clean up
    BiquadFilterFree(filter);
    
    // Check results
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(output[i], MatlabOutput[i], 0.00001);
    }
}

TEST(BiquadFilterSingle, TestOverlaps)
{
    // Set up
    float output[10];
    ClearBuffer(output, 10);
    BiquadFilter *filter = BiquadFilterInit(b, a);
    
    // Process
    BiquadFilterProcess(filter, output, input, 3);
    BiquadFilterProcess(filter,output + 3, input + 3, 3);
    BiquadFilterProcess(filter, output + 6, input + 6, 4);
    
    // Clean up
    BiquadFilterFree(filter);
    
    // Check results
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(output[i], MatlabOutput[i], 0.00001);
    }

}

TEST(BiquadFilterDouble, TestResultsAgainstMatlab)
{
    // Set up
    double output[10];
    ClearBufferD(output, 10);
    BiquadFilterD *filter = BiquadFilterInitD(bD, aD);
    
    // Process
    BiquadFilterProcessD(filter, output, inputD, 10);
    
    // Clean up
    BiquadFilterFreeD(filter);
    
    // Check results
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(output[i], MatlabOutputD[i], 0.00001);
    }
}

TEST(BiquadFilterDouble, TestOverlaps)
{
    // Set up
    double output[10];
    ClearBufferD(output, 10);
    BiquadFilterD *filter = BiquadFilterInitD(bD, aD);
    
    // Process
    BiquadFilterProcessD(filter, output, inputD, 3);
    BiquadFilterProcessD(filter,output + 3, inputD + 3, 3);
    BiquadFilterProcessD(filter, output + 6, inputD + 6, 4);
    
    // Clean up
    BiquadFilterFreeD(filter);
    
    // Check results
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(output[i], MatlabOutputD[i], 0.00001);
    }
    
}