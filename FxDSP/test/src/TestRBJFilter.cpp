//
//  TestRBJFilter.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/8/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "RBJFilter.h"
#include "TestRBJFilter.h"
#include "Signals.h"
#include "Dsp.h"
#include <gtest/gtest.h>

#define EPSILON (0.00001)


#pragma mark -
#pragma mark Single-Precision Filter Calculation
TEST(RBJFilterSingle, TestLowpassAgainstMatlab)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    
    RBJFilter *filter = RBJFilterInit(LOWPASS, 3000, 44100);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutput[i], output[i], EPSILON);
    }
}

TEST(RBJFilterSingle, TestHighpassAgainstMatlab)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(HIGHPASS, 3000, 44100);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(highpassOutput[i], output[i], EPSILON);
    }
}


TEST(RBJFilterSingle, TestBandpassAgainstMatlab)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(BANDPASS, 3000, 44100);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(bandpassOutput[i], output[i], EPSILON);
    }
}


TEST(RBJFilterSingle, TestAllpassAgainstMatlab)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(ALLPASS, 3000, 44100);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(allpassOutput[i], output[i], EPSILON);
    }
}


TEST(RBJFilterSingle, TestNotchAgainstMatlab)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(NOTCH, 3000, 44100);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(notchOutput[i], output[i], EPSILON);
    }
}


TEST(RBJFilterSingle, TestPeakAgainstMatlab)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(PEAK, 3000, 44100);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(peakOutput[i], output[i], EPSILON);
    }
}



TEST(RBJFilterSingle, TestLowShelfAgainstMatlab)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(LOW_SHELF, 3000, 44100);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowShelfOutput[i], output[i], EPSILON);
    }
}

TEST(RBJFilterSingle, TestHighShelfAgainstMatlab)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(HIGH_SHELF, 3000, 44100);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(highShelfOutput[i], output[i], EPSILON);
    }
}



#pragma mark Single-Precision Filter Update/Modify

TEST(RBJFilterSingle, TestSetCutoff)
{
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(LOWPASS, 2000, 44100);
    RBJFilterSetCutoff(filter, 3000.);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutput[i], output[i], EPSILON);
    }
}

TEST(RBJFilterSingle, TestSetType)
{
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(HIGHPASS, 3000, 44100);
    RBJFilterSetType(filter, LOWPASS);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutput[i], output[i], EPSILON);
    }
}


TEST(RBJFilterSingle, TestSetQ)
{
    float output[10];
    ClearBuffer(output, 10);
    
    RBJFilter *filter = RBJFilterInit(LOWPASS, 3000, 44100);
    RBJFilterSetQ(filter, 0.5);
    RBJFilterProcess(filter, output, ones, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NE(lowpassOutput[i], output[i]);
    }
    
    RBJFilterFlush(filter);  // Flush overlap buffers...
    RBJFilterSetQ(filter, 1);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutput[i], output[i], EPSILON);
    }
   
}



TEST(RBJFilterSingle, TestSetParams)
{
    
    float output[10];
    ClearBuffer(output, 10);
    
    
    RBJFilter *filter = RBJFilterInit(HIGHPASS, 0, 44100);
    RBJFilterSetParams(filter, LOWPASS, 3000, 1);
    RBJFilterProcess(filter, output, ones, 10);
    RBJFilterFree(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutput[i], output[i], EPSILON);
    }
}




#pragma mark -
#pragma mark Double-Precision Filter Calculation
TEST(RBJFilterDouble, TestLowpassAgainstMatlab)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(LOWPASS, 3000, 44100);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutputD[i], output[i], EPSILON);
    }
}

TEST(RBJFilterDouble, TestHighpassAgainstMatlab)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(HIGHPASS, 3000, 44100);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(highpassOutputD[i], output[i], EPSILON);
    }
}



TEST(RBJFilterDouble, TestBandpassAgainstMatlab)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(BANDPASS, 3000, 44100);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(bandpassOutputD[i], output[i], EPSILON);
    }
}

TEST(RBJFilterDouble, TestAllpassAgainstMatlab)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(ALLPASS, 3000, 44100);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(allpassOutputD[i], output[i], EPSILON);
    }
}

TEST(RBJFilterDouble, TestNotchAgainstMatlab)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(NOTCH, 3000, 44100);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(notchOutputD[i], output[i], EPSILON);
    }
}

TEST(RBJFilterDouble, TestPeakAgainstMatlab)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(PEAK, 3000, 44100);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(peakOutputD[i], output[i], EPSILON);
    }
}



TEST(RBJFilterDouble, TestLowShelfAgainstMatlab)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(LOW_SHELF, 3000, 44100);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowShelfOutputD[i], output[i], EPSILON);
    }
}


TEST(RBJFilterDouble, TestHighShelfAgainstMatlab)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(HIGH_SHELF, 3000, 44100);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(highShelfOutputD[i], output[i], EPSILON);
    }
}



TEST(RBJFilterDouble, TestSetCutoff)
{
    double output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(LOWPASS, 2000, 44100);
    RBJFilterSetCutoffD(filter, 3000.);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutputD[i], output[i], EPSILON);
    }
}

TEST(RBJFilterDouble, TestSetType)
{
    double  output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(HIGHPASS, 3000, 44100);
    RBJFilterSetTypeD(filter, LOWPASS);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutputD[i], output[i], EPSILON);
    }
}


TEST(RBJFilterDouble, TestSetQ)
{
    double  output[10];
    ClearBufferD(output, 10);
    
    RBJFilterD *filter = RBJFilterInitD(LOWPASS, 3000, 44100);
    RBJFilterSetQD(filter, 0.5);
    RBJFilterProcessD(filter, output, onesD, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NE(lowpassOutputD[i], output[i]);
    }
    
    RBJFilterFlushD(filter);  // Flush overlap buffers...
    RBJFilterSetQD(filter, 1);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutput[i], output[i], EPSILON);
    }
    
}


TEST(RBJFilterDouble, TestSetParams)
{
    
    double output[10];
    ClearBufferD(output, 10);
    
    
    RBJFilterD *filter = RBJFilterInitD(HIGHPASS, 0, 44100);
    RBJFilterSetParamsD(filter, LOWPASS, 3000, 1);
    RBJFilterProcessD(filter, output, onesD, 10);
    RBJFilterFreeD(filter);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(lowpassOutputD[i], output[i], EPSILON);
    }
}
