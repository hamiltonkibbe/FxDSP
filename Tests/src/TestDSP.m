//
//  TestDSP.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/28/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioDsp.h"
#include "signals.h"
#include "FtAudioUtilities.h"

@interface TestDSP : XCTestCase

@end

@implementation TestDSP

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

-(void)testFillBuffer
{
    float out[10];
    FTA_FillBuffer(out, 10, 1.0);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ones[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ones[i]);
    }
}

-(void)testCopyBuffer
{
    float out[10];
    FTA_CopyBuffer(out, ones, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ones[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ones[i]);
    }
}

-(void)testBufferAdd
{
    float out[10];
    FTA_BufferAdd(out, (float*)zeros, (float*)ramp, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ramp[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ramp[i]);
    }
}

-(void)testVectorVectorMultiply
{
    float out[10];
    FTA_VectorVectorMultiply(out, (float*)ones, (float*)ramp, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ramp[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ramp[i]);
    }
}

-(void)testVectorScalarMultiply
{
    float out[10];
    FTA_VectorScalarMultiply(out, (float*)ramp, 2.0, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ramp[i]*2.0, 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ramp[i]*2.0);
    }
}

-(void)testConvolve
{
    float out[10];
    float in1[5] =
    {
        0.0,
        0.5,
        1.0,
        0.5,
        0.0
    };
    
    float in2[6] =
    {
        0.5,
        0.5,
        0.5,
        1.0,
        1.0,
        1.0
    };
    
    // >> conv([0 0.5 1 0.5 0], [0.5 0.5 0.5 1.0 1.0 1.0])
    float res[10] =
    {
        0.0,
        0.25,
        0.75,
        1.0,
        1.25,
        1.75,
        2.0,
        1.5,
        0.5,
        0.0
    };
    
    FTA_Convolve(in1, 5, in2, 6, out);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], res[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], res[i]);
    }
}










@end
