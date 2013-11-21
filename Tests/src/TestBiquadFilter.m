//
//  TestBiquadFilter.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 10/28/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioBiquadFilter.h"
#include "FtAudioDsp.h"
#include "testBiquad.h"


@interface FutureTechAudioBiquadFilter_Tests : XCTestCase

@end

@implementation FutureTechAudioBiquadFilter_Tests

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

- (void)testBiquadAgainstMatlab
{
    printf("Testing filter results against matlab...");
    
    // Set up
    float output[10];
    FtAudioFillBuffer(output, 10, 0.0);
    FtAudioBiquadFilter *filter = FtAudioBiquadFilterInit(b, a);
    
    // Process
    FtAudioBiquadFilterProcess(filter, output, input, 10);
    
    // Clean up
    FtAudioBiquadFilterFree(filter);
    
    // Check results
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], MatlabOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabOutput[i]);
    }

}

- (void)testBiquadOverlaps
{
    // Set up
    float output[10];
    FtAudioFillBuffer(output, 10, 0.0);
    FtAudioBiquadFilter *filter = FtAudioBiquadFilterInit(b, a);
    
    // Process
    FtAudioBiquadFilterProcess(filter, output, input, 3);
    FtAudioBiquadFilterProcess(filter,output + 3, input + 3, 3);
    FtAudioBiquadFilterProcess(filter, output + 6, input + 6, 4);
    
    // Clean up
    FtAudioBiquadFilterFree(filter);
    
    // Check results
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], MatlabOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], MatlabOutput[i]);
    }

    
}



@end
