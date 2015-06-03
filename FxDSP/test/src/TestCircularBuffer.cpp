//
//  TestCircularBuffer.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/7/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "CircularBuffer.h"
#include <gtest/gtest.h>


#pragma mark Single-Precision Tests
TEST(CircularBufferSingle, TestWriteRead)
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0,0,0,0};
    CircularBuffer *buffer = CircularBufferInit(7);
    CircularBufferWrite(buffer, data, 4);
    CircularBufferRead(buffer, out, 4);
    CircularBufferFree(buffer);
    
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
    CircularBufferFree(buffer);
    
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
    CircularBufferFree(buffer);
    
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
    CircularBufferFree(buffer);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_FLOAT_EQ(out[i], 0.0);
    }
}

TEST(CircularBufferSingle, TestRewind)
{
    const float data[4] = {1,2,3,4};
    float out[4] = {0, 0, 0, 0};
    float expected[4] = {3, 4, 3, 4};
    CircularBuffer *buffer = CircularBufferInit(7);
    CircularBufferWrite(buffer, data, 4);
    CircularBufferRead(buffer, out, 4);
    CircularBufferRewind(buffer, 2);
    CircularBufferRead(buffer, out, 2);
    CircularBufferFlush(buffer);
    CircularBufferFree(buffer);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_FLOAT_EQ(out[i], expected[i]);
    }
}

TEST(CircularBufferSingle, TestCount)
{
    const float data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    float out[4];
    CircularBuffer* buffer = CircularBufferInit(8);
    CircularBufferWrite(buffer, data, 4);
    ASSERT_EQ(4, CircularBufferCount(buffer));
    CircularBufferRead(buffer, out, 2);
    ASSERT_EQ(2, CircularBufferCount(buffer));
    CircularBufferFlush(buffer);
    ASSERT_EQ(0, CircularBufferCount(buffer));
}


TEST(CircularBufferSingle, TestError)
{
    const float data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    float out[4];
    CircularBuffer* buffer = CircularBufferInit(8);
    CircularBufferWrite(buffer, data, 4);
    ASSERT_EQ(VALUE_ERROR, CircularBufferRead(buffer, out, 5));
    CircularBufferFlush(buffer);
    CircularBufferWrite(buffer, data, 8);
    ASSERT_EQ(VALUE_ERROR, CircularBufferWrite(buffer, data, 1));
}

#pragma mark Double-Precision Tests
TEST(CircularBufferDouble, TestWriteRead)
{
    const double data[4] = {1,2,3,4};
    double out[4] = {0,0,0,0};
    CircularBufferD *buffer = CircularBufferInitD(7);
    CircularBufferWriteD(buffer, data, 4);
    CircularBufferReadD(buffer, out, 4);
    CircularBufferFreeD(buffer);
    
    
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
    CircularBufferFreeD(buffer);
    
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
    CircularBufferFreeD(buffer);
    
    
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
    CircularBufferFreeD(buffer);
    
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_DOUBLE_EQ(out[i], 0.0);
    }
}

TEST(CircularBufferDouble, TestRewind)
{
    const double data[4] = {1,2,3,4};
    double out[4] = {0, 0, 0, 0};
    double expected[4] = {3, 4, 3, 4};
    CircularBufferD *buffer = CircularBufferInitD(7);
    CircularBufferWriteD(buffer, data, 4);
    CircularBufferReadD(buffer, out, 4);
    CircularBufferRewindD(buffer, 2);
    CircularBufferReadD(buffer, out, 2);
    CircularBufferFlushD(buffer);
    CircularBufferFreeD(buffer);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        ASSERT_DOUBLE_EQ(out[i], expected[i]);
    }
}


TEST(CircularBufferDouble, TestCount)
{
    const double data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    double out[4];
    CircularBufferD* buffer = CircularBufferInitD(8);
    CircularBufferWriteD(buffer, data, 4);
    ASSERT_EQ(4, CircularBufferCountD(buffer));
    CircularBufferReadD(buffer, out, 2);
    ASSERT_EQ(2, CircularBufferCountD(buffer));
    CircularBufferFlushD(buffer);
    ASSERT_EQ(0, CircularBufferCountD(buffer));
}

TEST(CircularBufferDouble, TestError)
{
    const double data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    double out[4];
    CircularBufferD* buffer = CircularBufferInitD(8);
    CircularBufferWriteD(buffer, data, 4);
    ASSERT_EQ(VALUE_ERROR, CircularBufferReadD(buffer, out, 5));
    CircularBufferFlushD(buffer);
    CircularBufferWriteD(buffer, data, 8);
    ASSERT_EQ(VALUE_ERROR, CircularBufferWriteD(buffer, data, 1));
}