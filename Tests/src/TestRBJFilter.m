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

- (void)testRBJLowpassFilterAgainstMatlab
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_LPF, 3000, 44100);
    float output[10];

    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJHighpassFilterAgainstMatlab
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_HPF, 3000, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], highpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], highpassOutput[i]);
    }
}

- (void)testRBJBandpassFilterAgainstMatlab
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_BPF, 3000, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], bandpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], bandpassOutput[i]);
    }
}

- (void)testRBJAllpassFilterAgainstMatlab
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_APF, 3000, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], allpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], allpassOutput[i]);
    }
}


- (void)testRBJNotchFilterAgainstMatlab
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_NOTCH, 3000, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], notchOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], notchOutput[i]);
    }
}


- (void)testRBJPeakFilterAgainstMatlab
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_PEAK, 3000, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterSetQ(filter, 2);
    FtAudioRBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], peakOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], peakOutput[i]);
    }
}


- (void)testRBJLowShelfFilterAgainstMatlab
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_LSHELF, 300, 44100);
    float output[10];

    FtAudioFillBuffer(output, 10, 0.0);

    FtAudioRBJFilterSetQ(filter, 0.5);
    FtAudioRBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowShelfOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowShelfOutput[i]);
    }
}


- (void)testRBJHighShelfFilterAgainstMatlab
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_LSHELF, 300, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterSetQ(filter, 0.5);
    FtAudioRBJFilterProcess(filter, output, input, 10);
    
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], highShelfOutput[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], highShelfOutput[i]);
    }
}


- (void)testRBJFilterSetCutoff
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_LPF, 2000, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterSetCutoff(filter, 3000);
    FtAudioRBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJFilterSetType
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_HPF, 3000, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterSetType(filter, RBJ_LPF);
    FtAudioRBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.0000011, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

- (void)testRBJFilterSetQ
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_LPF, 3000, 44100);
    float output[10];
    
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterSetQ(filter, 0.5);
    FtAudioRBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertNotEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
    
    FtAudioRBJFilterFlush(filter);  // Flush overlap buffers...
    FtAudioRBJFilterSetQ(filter, 1);
    FtAudioRBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], lowpassOutput[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], lowpassOutput[i]);
    }
}

@end
