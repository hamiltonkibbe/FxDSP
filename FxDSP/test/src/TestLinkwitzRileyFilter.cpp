//
//  TestLinkwitzRileyFilter.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/6/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#include "LinkwitzRileyFilter.h"
#include "Signals.h"
#include <gtest/gtest.h>


TEST(LinkwitzRileySingle, TestFlush)
{
    float in[100] = {0.};
    float out1[100] = {0.};
    float out2[100] = {0.};
    
    sinewave(in, 100, 1000, 0, 1.0, 48000);
    
    LRFilter* filter = LRFilterInit(LOWPASS, 25, 0.707, 100);
    LRFilterProcess(filter, out1, in, 100);
    LRFilterFlush(filter);
    LRFilterProcess(filter, out2, in, 100);
    
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_FLOAT_EQ(out1[i], out2[i]);
    }
}



TEST(LinkwitzRileySingle, TestSetParams)
{
    float in[100] = {0.};
    float out1[100] = {0.};
    float out2[100] = {0.};
    
    sinewave(in, 100, 1000, 0, 1.0, 48000);
    
    
    LRFilter* filter1 = LRFilterInit(LOWPASS, 25, 0.707, 100);
    LRFilter* filter2 = LRFilterInit(HIGHPASS, 50, 0.5, 100);
    LRFilterSetParams(filter1, HIGHPASS, 50, 0.5);
    LRFilterProcess(filter1, out1, in, 100);
    LRFilterProcess(filter2, out2, in, 100);
    
    LRFilterFree(filter1);
    LRFilterFree(filter2);
    
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_FLOAT_EQ(out2[i], out1[i]);
    }
}



TEST(LinkwitzRileyDouble, TestFlush)
{
    double in[100] = {0.};
    double out1[100] = {0.};
    double out2[100] = {0.};
    
    sinewaveD(in, 100, 1000, 0, 1.0, 48000);
    
    LRFilterD* filter = LRFilterInitD(LOWPASS, 25, 0.707, 100);
    LRFilterProcessD(filter, out1, in, 100);
    LRFilterFlushD(filter);
    LRFilterProcessD(filter, out2, in, 100);
    
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_DOUBLE_EQ(out1[i], out2[i]);
    }
}


TEST(LinkwitzRileyDouble, TestSetParams)
{
    double in[100] = {0.};
    double out1[100] = {0.};
    double out2[100] = {0.};
    
    sinewaveD(in, 100, 1000, 0, 1.0, 48000);
    
    
    LRFilterD* filter1 = LRFilterInitD(LOWPASS, 25, 0.707, 100);
    LRFilterD* filter2 = LRFilterInitD(HIGHPASS, 50, 0.5, 100);
    LRFilterSetParamsD(filter1, HIGHPASS, 50, 0.5);
    LRFilterProcessD(filter1, out1, in, 100);
    LRFilterProcessD(filter2, out2, in, 100);
    
    LRFilterFreeD(filter1);
    LRFilterFreeD(filter2);
    
    for (unsigned i = 0; i < 100; ++i)
    {
        ASSERT_DOUBLE_EQ(out2[i], out1[i]);
    }
}

