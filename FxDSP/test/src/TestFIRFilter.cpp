//
//  TestFIR.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/8/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "gtest/gtest.h"
#include "TestFIRFilter.h"
#include "FIRFilter.h"
#include "Signals.h"


TEST(FIRFilterSingle, TestDirectAgainstMatlab)
{
    float output[100];
    
    // Set up
    FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 21, DIRECT);
    
    // Process
    FIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
    // Tear down
    FIRFilterFree(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 32; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutput[i], output[i], 0.01);
    }
}

TEST(FIRFilterSingle, TestFFTAgainstMatlab)
{
    float output[100];
    
    // Set up
    FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 21, FFT);
    
    // Process
    FIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
    // Tear down
    FIRFilterFree(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 32; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutput[i], output[i], 0.01);
    }
    
}

TEST(FIRFilterSingle, TestBestModeAgainstMatlab)
{
    float output[100];
    
    // Set up
    FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 21, BEST);
    
    // Process
    FIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
    // Tear down
    FIRFilterFree(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 32; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutput[i], output[i], 0.01);
    }
    
}



TEST(FIRFilterDouble, TestDirectAgainstMatlab)
{
    double output[100];
    
    // Set up
    FIRFilterD *theFilter = FIRFilterInitD(MatlabFilterD, 21, DIRECT);
    
    // Process
    FIRFilterProcessD(theFilter, output, MatlabSignalD,100);
    
    // Tear down
    FIRFilterFreeD(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 32; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutputD[i], output[i], 0.01);
    }
}

TEST(FIRFilterDouble, TestFFTAgainstMatlab)
{
    double output[100];
    
    // Set up
    FIRFilterD *theFilter = FIRFilterInitD(MatlabFilterD, 21, FFT);
    
    // Process
    FIRFilterProcessD(theFilter, output, MatlabSignalD, 100);
    
    // Tear down
    FIRFilterFreeD(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 32; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutputD[i], output[i], 0.01);
    }
    
}

TEST(FIRFilterDouble, TestBestModeAgainstMatlab)
{
    double output[100];
    
    // Set up
    FIRFilterD *theFilter = FIRFilterInitD(MatlabFilterD, 21, BEST);
    
    // Process
    FIRFilterProcessD(theFilter, output, MatlabSignalD, 100);
    
    // Tear down
    FIRFilterFreeD(theFilter);
    
    // Check results
    for (unsigned i = 0; i < 32; ++i)
    {
        ASSERT_NEAR(MatlabLowpassOutputD[i], output[i], 0.01);
    }
    
}