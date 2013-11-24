//
//  TestRBJFilter.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/20/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioRBJFilter.h"
#include "FtAudioDsp.h"
#include "testRBJ.h"

@interface TestRBJFilter : XCTestCase

@end

@implementation TestRBJFilter



- (void)testRBJLowpassFilterAgainstMatlab
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(LOWPASS, 3000, 44100);
    float output[10];

    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJHighpassFilterAgainstMatlab
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(HIGHPASS, 3000, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], highpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], highpassOutput[i]);
    }
}

- (void)testRBJBandpassFilterAgainstMatlab
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(BANDPASS, 3000, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], bandpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], bandpassOutput[i]);
    }
}

- (void)testRBJAllpassFilterAgainstMatlab
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(ALLPASS, 3000, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], allpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], allpassOutput[i]);
    }
}


- (void)testRBJNotchFilterAgainstMatlab
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(NOTCH, 3000, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], notchOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], notchOutput[i]);
    }
}


- (void)testRBJPeakFilterAgainstMatlab
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(PEAK, 3000, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterSetQ(filter, 2);
    FTA_RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], peakOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], peakOutput[i]);
    }
}


- (void)testRBJLowShelfFilterAgainstMatlab
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(LOW_SHELF, 300, 44100);
    float output[10];

    FTA_FillBuffer(output, 10, 0.0);

    FTA_RBJFilterSetQ(filter, 0.5);
    FTA_RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowShelfOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowShelfOutput[i]);
    }
}


- (void)testRBJHighShelfFilterAgainstMatlab
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(LOW_SHELF, 300, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterSetQ(filter, 0.5);
    FTA_RBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], highShelfOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], highShelfOutput[i]);
    }
}


- (void)testRBJFilterSetCutoff
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(LOWPASS, 2000, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterSetCutoff(filter, 3000);
    FTA_RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJFilterSetType
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(HIGHPASS, 3000, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterSetType(filter, LOWPASS);
    FTA_RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.0000011, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJFilterSetQ
{
    
    FTA_RBJFilter *filter = FTA_RBJFilterInit(LOWPASS, 3000, 44100);
    float output[10];
    
    FTA_FillBuffer(output, 10, 0.0);
    
    FTA_RBJFilterSetQ(filter, 0.5);
    FTA_RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertNotEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
    
    FTA_RBJFilterFlush(filter);  // Flush overlap buffers...
    FTA_RBJFilterSetQ(filter, 1);
    FTA_RBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

@end
