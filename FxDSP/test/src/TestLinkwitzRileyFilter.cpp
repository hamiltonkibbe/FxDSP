//
//  TestLinkwitzRileyFilter.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/6/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#include "LinkwitzRileyFilter.h"
#include <gtest/gtest.h>


TEST(LinkwitzRileySingle, TestSetParams)
{
    LRFilter* filter = LRFilterInit(LOWPASS, 25, 0.707, 100);
    ASSERT_TRUE(filter);
    LRFilterSetParams(filter, HIGHPASS, 50, 0.5);
    LRFilterFree(filter);
}