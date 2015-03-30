
//
//  FxDSPFIR_Tests.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 10/25/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FIRFilter.h"
#include "testFIR.h"
#include "signals.h"

@interface FxDSPFIRFilter_Tests : XCTestCase

@end

@implementation FxDSPFIRFilter_Tests


- (void)testFIRFilterDirectAgainstMatlab
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
        XCTAssertEqualWithAccuracy(output[i], MatlabLowpassOutput[i], 0.01, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabLowpassOutput[i]);
    }
}


- (void)testFIRFilterFFTAgainstMatlab
{
    float output[100];
    
	// Set up
    FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 21, FFT);
    if (theFilter)
    {
        // Process
        FIRFilterProcess(theFilter, output, MatlabSignal, 100);
        
        // Tear down
        FIRFilterFree(theFilter);
        
        // Check results
        for (unsigned i = 0; i < 32; ++i)
        {
            XCTAssertEqualWithAccuracy(output[i], MatlabLowpassOutput[i], 0.01,
                                       @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabLowpassOutput[i]);
        }
    }
}




- (void)testFIRFilterBlockSize
{
    unsigned inputLength = 100;
    float outfull[inputLength];
    float outchunk[inputLength];
    const float *input = MatlabSignal;
    
	// Set up
	FIRFilter *theFilter = FIRFilterInit(MatlabFilter, 21, FFT);//BEST);
	if (theFilter)
    {
        // Process in one block
        FIRFilterProcess(theFilter, outfull, input, inputLength);
        
        // Process in two blocks
        FIRFilterFlush(theFilter);
        FIRFilterProcess(theFilter, outchunk, input, (inputLength / 2));
        FIRFilterProcess(theFilter, outchunk+(inputLength / 2), input+(inputLength / 2), (inputLength / 2));
        
        // Tear down
        FIRFilterFree(theFilter);
        
        // Check results
        for (unsigned i = 0; i < 32; ++i)
        {
            XCTAssertEqualWithAccuracy(outfull[i], outchunk[i], 0.001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, outfull[i], outchunk[i]);
        }
    }
}


@end
