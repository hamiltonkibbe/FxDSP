//
//  testOnePoleLP.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 3/24/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>

#include "signals.h"
#include "testOnePole.h"
#include "OnePole.h"
#include "Dsp.h"

@interface testOnePole : XCTestCase

@end

@implementation testOnePole

- (void) testLowpassAgainstMatlab
{
    // Set up
    float output[50];
    FillBuffer(output, 50, 0.0);


    OnePole *filter = OnePoleInit(25, 100, LOWPASS);

    // Process
    OnePoleProcess(filter, output, MatlabSignal, 50);

    // Clean up
    OnePoleFree(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], MatlabLPOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabLPOutput[i]);
    }

}

- (void) testLowpassTick
{
    // Set up
    float output[50];
    FillBuffer(output, 50, 0.0);
    
    
    OnePole *filter = OnePoleInit(25, 100, LOWPASS);
    
    // Process
    for (unsigned i = 0; i < 50; ++i)
    {
        output[i] = OnePoleTick(filter, MatlabSignal[i]);
    }
    
    // Clean up
    OnePoleFree(filter);
    
    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], MatlabLPOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabLPOutput[i]);
    }
    
}

- (void) testHighpassAgainstMatlab
{
    // Set up
    float output[50];
    FillBuffer(output, 50, 0.0);
    
    
    OnePole *filter = OnePoleInit(25, 100, HIGHPASS);
    
    // Process
    OnePoleProcess(filter, output, MatlabSignal, 50);
    
    // Clean up
    OnePoleFree(filter);
    
    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], MatlabHPOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabHPOutput[i]);
    }
    
}

- (void) testHighpassTick
{
    // Set up
    float output[50];
    FillBuffer(output, 50, 0.0);
    
    
    OnePole *filter = OnePoleInit(25, 100, HIGHPASS);
    
    // Process
    for (unsigned i = 0; i < 50; ++i)
    {
        output[i] = OnePoleTick(filter, MatlabSignal[i]);
    }
    
    // Clean up
    OnePoleFree(filter);
    
    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], MatlabHPOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabHPOutput[i]);
    }
}

- (void) testOnePoleTypeValidation
{
    OnePole *filter = OnePoleInit(25, 100, LOWPASS);
    
    // Process
    Error_t val = OnePoleSetType(filter, LOW_SHELF);
    
    // Clean up
    OnePoleFree(filter);
    
    XCTAssertEqual(val, VALUE_ERROR);
}


@end
