//
//  TestRMSEstimator.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/26/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioRMSEstimator.h"
#include <math.h>

@interface TestRMSEstimator : XCTestCase

@end

@implementation TestRMSEstimator

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

- (void)testRMSEstimatorProcess
{
    float sinewave[10000];
    float out[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sinf((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    FTA_RMSEstimator * rms = FTA_RMSEstimatorInit(0.01, 10000);
    FTA_RMSEstimatorProcess(rms, out, sinewave, 10000);
    FTA_RMSEstimatorFree(rms);
    for (unsigned i = 250; i < 10000; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], 0.7071, 0.1, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], 0.7071);
    }
}


- (void)testRMSEstimatorTick
{
    float sinewave[10000];
    float out[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sinf((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    FTA_RMSEstimator * rms = FTA_RMSEstimatorInit(0.01, 10000);
    
    for (unsigned i = 0; i < 10000; ++i)
    {
        out[i] = FTA_RMSEstimatorTick(rms, sinewave[i]);
    }
    
    FTA_RMSEstimatorFree(rms);
    
    for (unsigned i = 250; i < 10000; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], 0.7071, 0.1, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], 0.7071);
    }
    
}
@end