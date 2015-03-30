//
//  TestBiquadFilter.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 10/28/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "BiquadFilter.h"
#include "Dsp.h"
#include "testBiquad.h"


@interface FxDSPBiquadFilter_Tests : XCTestCase

@end

@implementation FxDSPBiquadFilter_Tests

- (void)testBiquadAgainstMatlab
{
    printf("\nTesting filter results against matlab...\n");
    
    // Set up
    float output[10];
    FillBuffer(output, 10, 0.0);
    BiquadFilter *filter = BiquadFilterInit(b, a);
    
    // Process
    BiquadFilterProcess(filter, output, input, 10);
    
    // Clean up
    BiquadFilterFree(filter);
    
    // Check results
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], MatlabOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabOutput[i]);
    }

}

- (void)testBiquadOverlaps
{
    printf("\nTesting filter overlaps\n");
    // Set up
    float output[10];
    FillBuffer(output, 10, 0.0);
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
        XCTAssertEqualWithAccuracy(output[i], MatlabOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabOutput[i]);
    }

    
}



@end
