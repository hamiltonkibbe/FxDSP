//
//  TestFIR.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/8/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "TestFIRFilter.h"
#include "FIRFilter.h"
#include "Signals.h"
#include "Dsp.h"
#include <gtest/gtest.h>


#define EPSILON (0.000001)


TEST(FIRFilterSingle, TestDirectAgainstMatlab)
{
    float output[100];
    
    // Set up
    FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 22, DIRECT);
    
    // Process
    FIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
    // Tear down
    FIRFilterFree(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutput[i], output[i], EPSILON);
    }
}

TEST(FIRFilterSingle, TestFFTAgainstMatlab)
{
    float output[100];
    
    // Set up
    FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 22, FFT);
    
    // Process
    FIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
    // Tear down
    FIRFilterFree(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutput[i], output[i],  EPSILON);
    }
    
}

TEST(FIRFilterSingle, TestBestModeAgainstMatlab)
{
    float output[100];
    
    // Set up
    FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 22, BEST);
    
    // Process
    FIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
    // Tear down
    FIRFilterFree(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutput[i], output[i], EPSILON);
    }
    
}


TEST(FIRFilterSingle, TestOverlapAdd)
{
    
    float output[100];
    unsigned block_size[6] = {4, 5, 10, 20, 25, 50};
    
    // Set up
    FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 22, DIRECT);
    
    for (unsigned i = 0; i < 6; ++i)
    {
        unsigned bs = block_size[i];
        unsigned bc = 100 / bs;
        for (unsigned block = 0; block < bc; ++block)
        {
            // Process
            FIRFilterProcess(theFilter, output + (block * bs), MatlabSignal + (block * bs), bs);;
        }
        // Check results
        for (unsigned i = 0; i < 100; ++i)
        {
            ASSERT_NEAR(MatlabLowpassOutput[i], output[i], EPSILON);
        }
        FIRFilterFlush(theFilter);
    }
        
    // Tear down
    FIRFilterFree(theFilter);
}


TEST(FIRFilterSingle, TestUpdateKernel)
{
    float kernel[22];
    float output[100];
    ClearBuffer(kernel, 22);
    
    // Set up
    FIRFilter *theFilter = FIRFilterInit(kernel, 22, DIRECT);
    
    // Process
    FIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(0, output[i], EPSILON);
    }
    
    FIRFilterUpdateKernel(theFilter, MatlabFilter);
    FIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutput[i], output[i], EPSILON);
    }

    
    // Tear down
    FIRFilterFree(theFilter);
}




TEST(FIRFilterDouble, TestDirectAgainstMatlab)
{
    double output[100];
    
    // Set up
    FIRFilterD *theFilter = FIRFilterInitD(MatlabFilterD, 22, DIRECT);
    
    // Process
    FIRFilterProcessD(theFilter, output, MatlabSignalD,100);
    
    // Tear down
    FIRFilterFreeD(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutputD[i], output[i], EPSILON);
    }
}

TEST(FIRFilterDouble, TestFFTAgainstMatlab)
{
    double output[100];
    
    // Set up
    FIRFilterD *theFilter = FIRFilterInitD(MatlabFilterD, 22, FFT);
    
    // Process
    FIRFilterProcessD(theFilter, output, MatlabSignalD, 100);
    
    // Tear down
    FIRFilterFreeD(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutputD[i], output[i], EPSILON);
    }
    
}

TEST(FIRFilterDouble, TestBestModeAgainstMatlab)
{
    double output[100];
    
    // Set up
    FIRFilterD *theFilter = FIRFilterInitD(MatlabFilterD, 22, BEST);
    
    // Process
    FIRFilterProcessD(theFilter, output, MatlabSignalD, 100);
    
    // Tear down
    FIRFilterFreeD(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutputD[i], output[i], EPSILON);
    }
    
}


TEST(FIRFilterDouble, TestOverlapAdd)
{
    
    double output[100];
    unsigned block_size[6] = {4, 5, 10, 20, 25, 50};
    
    // Set up
    FIRFilterD *theFilter = FIRFilterInitD(MatlabFilterD, 22, DIRECT);
    
    for (unsigned i = 0; i < 6; ++i)
    {
        unsigned bs = block_size[i];
        unsigned bc = 100 / bs;
        for (unsigned block = 0; block < bc; ++block)
        {
            // Process
            FIRFilterProcessD(theFilter, output + (block * bs), MatlabSignalD + (block * bs), bs);;
        }
        // Check results
        for (unsigned i = 0; i < 100; ++i)
        {
            ASSERT_NEAR(MatlabLowpassOutputD[i], output[i], EPSILON);
        }
        FIRFilterFlushD(theFilter);
    }
    
    // Tear down
    FIRFilterFreeD(theFilter);
}


TEST(FIRFilterDouble, TestUpdateKernel)
{
    double kernel[22];
    double output[100];
    
    ClearBufferD(kernel, 22);
    
    // Set up
    FIRFilterD *theFilter = FIRFilterInitD(kernel, 22, DIRECT);
    
    // Process
    FIRFilterProcessD(theFilter, output, MatlabSignalD,100);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(0, output[i], EPSILON);
    }
    
    
    FIRFilterUpdateKernelD(theFilter, MatlabFilterD);
    // Process
    FIRFilterProcessD(theFilter, output, MatlabSignalD,100);
    
    // Check results
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutputD[i], output[i], EPSILON);
    }
    
    // Tear down
    FIRFilterFreeD(theFilter);
}