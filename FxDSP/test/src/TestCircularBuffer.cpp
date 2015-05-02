//
//  TestCircularBuffer.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/7/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "gtest/gtest.h"
#include "CircularBuffer.h"

#pragma mark Single-Precision Tests
TEST(CircularBufferSingle, TestWriteRead)
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    CircularBuffer *buffer = CircularBufferInit(7);
    CircularBufferWrite(buffer, data, 4);
    CircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_FLOAT_EQ(out[i], data[i]);
    }
}

TEST(CircularBufferSingle, TestMultiWriteRead)
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    CircularBuffer *buffer = CircularBufferInit(7);
    CircularBufferWrite(buffer, data, 2);
    CircularBufferWrite(buffer, data + 2, 2);
    CircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_FLOAT_EQ(out[i], data[i]);
    }
}

TEST(CircularBufferSingle, TestWrap)
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    CircularBuffer *buffer = CircularBufferInit(7);
    CircularBufferWrite(buffer, data, 4);
    CircularBufferWrite(buffer, data, 4);
    CircularBufferWrite(buffer, data, 4);
    CircularBufferRead(buffer, out, 4);
    CircularBufferRead(buffer, out, 4);
    CircularBufferRead(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_FLOAT_EQ(out[i], data[i]);
    }
}

TEST(CircularBufferSingle, TestFlush)
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    CircularBuffer *buffer = CircularBufferInit(7);
    CircularBufferWrite(buffer, data, 4);
    CircularBufferFlush(buffer);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_FLOAT_EQ(out[i], 0.0);
    }
}

#pragma mark Double-Precision Tests
TEST(CircularBufferDouble, TestWriteRead)
{
    const double data[4] = {1,2,3,4};
    double out[4] = {0,0,0,0};
    CircularBufferD *buffer = CircularBufferInitD(7);
    CircularBufferWriteD(buffer, data, 4);
    CircularBufferReadD(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_DOUBLE_EQ(out[i], data[i]);
    }
}

TEST(CircularBufferDouble, TestMultiWriteRead)
{
    const double data[4] = {1,2,3,4};
    double out[4] = {0,0,0,0};
    CircularBufferD *buffer = CircularBufferInitD(7);
    CircularBufferWriteD(buffer, data, 2);
    CircularBufferWriteD(buffer, data + 2, 2);
    CircularBufferReadD(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_DOUBLE_EQ(out[i], data[i]);
    }
}

TEST(CircularBufferDouble, TestWrap)
{
    const double data[4] = {1,2,3,4};
    double out[4] = {0,0,0,0};
    CircularBufferD *buffer = CircularBufferInitD(7);
    CircularBufferWriteD(buffer, data, 4);
    CircularBufferWriteD(buffer, data, 4);
    CircularBufferWriteD(buffer, data, 4);
    CircularBufferReadD(buffer, out, 4);
    CircularBufferReadD(buffer, out, 4);
    CircularBufferReadD(buffer, out, 4);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_DOUBLE_EQ(out[i], data[i]);
    }
}

TEST(CircularBufferDouble, TestFlush)
{
    const double data[4] = {1,2,3,4};
    double out[4] = {0,0,0,0};
    CircularBufferD *buffer = CircularBufferInitD(7);
    CircularBufferWriteD(buffer, data, 4);
    CircularBufferFlushD(buffer);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_DOUBLE_EQ(out[i], 0.0);
    }
}