//
//  TestRBJFilter.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/20/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "RBJFilter.h"
#include "Dsp.h"
#include "testRBJ.h"

@interface TestRBJFilter : XCTestCase

@end

@implementation TestRBJFilter



- (void)testRBJLowpassFilterAgainstMatlab
{
    
    RBJFilter *filter = RBJFilterInit(LOWPASS, 3000, 44100);
    float output[10];

    FillBuffer(output, 10, 0.0);
    
    RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJHighpassFilterAgainstMatlab
{
    
    RBJFilter *filter = RBJFilterInit(HIGHPASS, 3000, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], highpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], highpassOutput[i]);
    }
}

- (void)testRBJBandpassFilterAgainstMatlab
{
    
    RBJFilter *filter = RBJFilterInit(BANDPASS, 3000, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], bandpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], bandpassOutput[i]);
    }
}

- (void)testRBJAllpassFilterAgainstMatlab
{
    
    RBJFilter *filter = RBJFilterInit(ALLPASS, 3000, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], allpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], allpassOutput[i]);
    }
}


- (void)testRBJNotchFilterAgainstMatlab
{
    
    RBJFilter *filter = RBJFilterInit(NOTCH, 3000, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], notchOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], notchOutput[i]);
    }
}


- (void)testRBJPeakFilterAgainstMatlab
{
    
    RBJFilter *filter = RBJFilterInit(PEAK, 3000, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterSetQ(filter, 2);
    RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], peakOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], peakOutput[i]);
    }
}


- (void)testRBJLowShelfFilterAgainstMatlab
{
    
    RBJFilter *filter = RBJFilterInit(LOW_SHELF, 300, 44100);
    float output[10];

    FillBuffer(output, 10, 0.0);

    RBJFilterSetQ(filter, 0.5);
    RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowShelfOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowShelfOutput[i]);
    }
}


- (void)testRBJHighShelfFilterAgainstMatlab
{
    
    RBJFilter *filter = RBJFilterInit(LOW_SHELF, 300, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterSetQ(filter, 0.5);
    RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], highShelfOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], highShelfOutput[i]);
    }
}


- (void)testRBJFilterSetCutoff
{
    
    RBJFilter *filter = RBJFilterInit(LOWPASS, 2000, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterSetCutoff(filter, 3000);
    RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJFilterSetType
{
    
    RBJFilter *filter = RBJFilterInit(HIGHPASS, 3000, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterSetType(filter, LOWPASS);
    RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.0000011, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJFilterSetQ
{
    
    RBJFilter *filter = RBJFilterInit(LOWPASS, 3000, 44100);
    float output[10];
    
    FillBuffer(output, 10, 0.0);
    
    RBJFilterSetQ(filter, 0.5);
    RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertNotEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
    
    RBJFilterFlush(filter);  // Flush overlap buffers...
    RBJFilterSetQ(filter, 1);
    RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

@end
