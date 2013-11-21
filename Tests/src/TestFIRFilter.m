//
//  FutureTechAudioFIR_Tests.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 10/25/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioFIRFilter.h"
#include "testFIR.h"
#include "signals.h"

@interface FutureTechAudioFIRFilter_Tests : XCTestCase

@end

@implementation FutureTechAudioFIRFilter_Tests

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testFIRFilterDirectAgainstMatlab
{
    float output[100];
    
	// Set up
    FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit(MatlabFilter, 21, DIRECT);
    
    // Process
    FtAudioFIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
	// Tear down
    FtAudioFIRFilterFree(theFilter);
    
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
    FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit(MatlabFilter, 21, FFT);
    
    // Process
    FtAudioFIRFilterProcess(theFilter, output, MatlabSignal, 100);
    
	// Tear down
    FtAudioFIRFilterFree(theFilter);
    
	// Check results
    for (unsigned i = 0; i < 32; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], MatlabLowpassOutput[i], 0.01, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabLowpassOutput[i]);
    }
}




- (void)testFIRFilterBlockSize
{
    printf("Testing variable block size...");
    unsigned inputLength = 100;
    float outfull[inputLength];
    float outchunk[inputLength];
    const float *input = MatlabSignal;
    
	// Set up
	FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit(MatlabFilter, 21, FFT);//BEST);
	
	// Process in one block
    FtAudioFIRFilterProcess(theFilter, outfull, input, inputLength);
    
	// Process in two blocks
    FtAudioFIRFilterFlush(theFilter);
    FtAudioFIRFilterProcess(theFilter, outchunk, input, (inputLength / 2));
    FtAudioFIRFilterProcess(theFilter, outchunk+(inputLength / 2), input+(inputLength / 2), (inputLength / 2));
    
	// Tear down
	FtAudioFIRFilterFree(theFilter);
    
	// Check results
    for (unsigned i = 0; i < 32; ++i)
    {
        XCTAssertEqualWithAccuracy(outfull[i], outchunk[i], 0.001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, outfull[i], outchunk[i]);
    }
};


@end
