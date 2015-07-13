//
//  TestBiquadFilter.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/7/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "TestBiquadFilter.h"
#include "BiquadFilter.h"
#include "Dsp.h"
#include <gtest/gtest.h>


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


TEST(BiquadFilterSingle, TestTick)
{
    // Set up
    float output[10];
    ClearBuffer(output, 10);
    BiquadFilter *filter = BiquadFilterInit(b, a);
    
    for (unsigned i = 0; i < 10; ++i)
    {;
        output[i] = BiquadFilterTick(filter, input[i]);
    }
    
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


TEST(BiquadFilterDouble, TestTick)
{
    // Set up
    double output[10];
    ClearBufferD(output, 10);
    BiquadFilterD *filter = BiquadFilterInitD(bD, aD);
    
    for (unsigned i = 0; i < 10; ++i)
    {;
        output[i] = BiquadFilterTickD(filter, input[i]);
    }
    
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