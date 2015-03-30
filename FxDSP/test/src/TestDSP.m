//
//  TestDSP.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/28/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "Dsp.h"
#include "signals.h"
#include "Utilities.h"

@interface TestDSP : XCTestCase

@end

@implementation TestDSP


-(void)testFillBuffer
{
    float out[10];
    FillBuffer(out, 10, 1.0);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ones[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ones[i]);
    }
}

-(void)testClearBuffer
{
    float out[10];
    for (unsigned i = 0; i < 10; ++i)
    {
        out[i] = i;
    }
    ClearBuffer(out, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], zeros[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ones[i]);
    }

}

-(void)testCopyBuffer
{
    float out[10];
    CopyBuffer(out, ones, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ones[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ones[i]);
    }
}

-(void)testVectorVectorAdd
{
    float out[10];
    VectorVectorAdd(out, zeros, ramp, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ramp[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ramp[i]);
    }
}


-(void) testVectorScalarAdd
{
    float out[10];
    VectorScalarAdd(out, (float*)zeros, 1, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ones[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ones[i]);
    }
    
}

-(void)testVectorVectorMultiply
{
    float out[10];
    VectorVectorMultiply(out, ones, ramp, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], ramp[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], ramp[i]);
    }
}

-(void)testVectorScalarMultiply
{
    float out[10];
    VectorScalarMultiply(out, ramp, 2.0, 10);
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
    
    Convolve(in1, 5, in2, 6, out);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], res[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], res[i]);
    }
}










@end
