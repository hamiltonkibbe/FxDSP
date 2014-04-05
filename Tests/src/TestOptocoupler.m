//
//  TestOptocoupler.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 4/5/14.
//  Copyright (c) 2014 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioOptocoupler.h"

@interface TestOptocoupler : XCTestCase

@end

@implementation TestOptocoupler


- (void)testStepResponse
{
    
    float step_function[100] = {0.0};
    float out[100] = {0.0};
    unsigned i;
    for (i = 25; i < 100; ++i)
    {
        step_function[i] = 1.0;
    }
    
    FTA_Opto* opto = FTA_OptoInit(OPTO_LDR, 0.5, 2000);
    
    /* Process input */
    for (i = 0; i < 100; ++i)
    {
        out[i] = FTA_OptoTick(opto, step_function[i]);
    }
    
}

@end
