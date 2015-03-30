//
//  TestOptocoupler.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/5/14.
//  Copyright (c) 2014 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "Optocoupler.h"

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
    
    Opto* opto;
    opto = OptoInit(OPTO_LDR, 0.5, 2000);
    
    /* Process input */
    for (i = 0; i < 100; ++i)
    {
        out[i] = OptoTick(opto, step_function[i]);
    }
    OptoFree(opto);
}


- (void) testPulseResponse
{
    float pulse[200] = {0.0};
    float out[200] = {0.0};
    
    for (unsigned i = 50; i < 100; ++i)
    {
        pulse[i] = 1.0;
    }
    Opto* opto;
    opto = OptoInit(OPTO_PHOTOTRANSISTOR, 0.5, 1000);
    
    printf("\n\n\n\n\nPulseResp = [\n");
    /* Process input */
    for (unsigned i = 0; i < 200; ++i)
    {
        out[i] = OptoTick(opto, pulse[i]);
        printf("    %1.20f,\n", out[i]);
    }
    printf("];\n\n\n\n\n");
    OptoFree(opto);
}
@end
