//
//  testUpsampler.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 3/23/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//
#include "Upsampler.h"
#include "signals.h"
#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>

@interface testUpsampler : XCTestCase

@end

@implementation testUpsampler

-(void) testUpsamplerAgainstMatlab
{
    float out[400];
    Upsampler* resample = UpsamplerInit(UP_4X);
    UpsamplerProcess(resample, out, MatlabSignal, 100);
    printf("\n\n\n\nsig = [\n");
    for (unsigned i = 0; i < 400; ++i)
    {
        printf("%1.10f,\n", out[i]);
    }
    printf("];\n\n\n");
    UpsamplerFree(resample);
}

@end
