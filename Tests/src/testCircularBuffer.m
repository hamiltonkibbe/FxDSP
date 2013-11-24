//
//  testCircularBuffer.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/22/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioCircularBuffer.h"
@interface testCircularBuffer : XCTestCase

@end

@implementation testCircularBuffer

- (void)testWriteRead
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    FTA_CircularBuffer *buffer = FTA_CircularBufferInit(7);
    FTA_CircularBufferWrite(buffer, data, 4);
    FTA_CircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], data[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], data[i]);
    }
}

- (void)testMultiWriteRead
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    FTA_CircularBuffer *buffer = FTA_CircularBufferInit(7);
    FTA_CircularBufferWrite(buffer, data, 2);
    FTA_CircularBufferWrite(buffer, data + 2, 2);
    FTA_CircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], data[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], data[i]);
    }
}

- (void)testWrap
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    FTA_CircularBuffer *buffer = FTA_CircularBufferInit(7);
    FTA_CircularBufferWrite(buffer, data, 4);
    FTA_CircularBufferWrite(buffer, data, 4);
    FTA_CircularBufferWrite(buffer, data, 4);
    FTA_CircularBufferRead(buffer, out, 4);
    FTA_CircularBufferRead(buffer, out, 4);
    FTA_CircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], data[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], data[i]);
    }
}


@end
