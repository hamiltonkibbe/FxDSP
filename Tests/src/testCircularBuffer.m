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
    FtAudioCircularBuffer *buffer = FtAudioCircularBufferInit(7);
    FtAudioCircularBufferWrite(buffer, data, 4);
    FtAudioCircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], data[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], data[i]);
    }
}

- (void)testMultiWriteRead
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    FtAudioCircularBuffer *buffer = FtAudioCircularBufferInit(7);
    FtAudioCircularBufferWrite(buffer, data, 2);
    FtAudioCircularBufferWrite(buffer, data + 2, 2);
    FtAudioCircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], data[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], data[i]);
    }
}

- (void)testWrap
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    FtAudioCircularBuffer *buffer = FtAudioCircularBufferInit(7);
    FtAudioCircularBufferWrite(buffer, data, 4);
    FtAudioCircularBufferWrite(buffer, data, 4);
    FtAudioCircularBufferWrite(buffer, data, 4);
    FtAudioCircularBufferRead(buffer, out, 4);
    FtAudioCircularBufferRead(buffer, out, 4);
    FtAudioCircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        XCTAssertEqualWithAccuracy(out[i], data[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, out[i], data[i]);
    }
}


@end
