//
//  TestWindowFunctions.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/8/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "gtest/gtest.h"
#include "TestWindowFunctions.h"
#include "WindowFunction.h"
#include "Dsp.h"
#include "Signals.h"

#define EPSILON (0.000001)

#pragma mark -
#pragma mark Single-precision tests

TEST(WindowFunctionSingle, TestBoxcarWindow)
{
    float window[TEST_WINDOW_LENGTH];
    boxcar(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBoxcar[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestHannWindow)
{
    float window[TEST_WINDOW_LENGTH];
    hann(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabHann[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestHammingWindow)
{
    float window[TEST_WINDOW_LENGTH];
    hamming(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabHamming[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestBlackmanWindow)
{
    float window[TEST_WINDOW_LENGTH];
    blackman(TEST_WINDOW_LENGTH, 0.16, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBlackman[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestTukeyWindow)
{
    float window[TEST_WINDOW_LENGTH];
    tukey(TEST_WINDOW_LENGTH, 0.5, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabTukey[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestBartlettWindow)
{
    float window[TEST_WINDOW_LENGTH];
    bartlett(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBartlett[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestGaussianWindow)
{
    float window[TEST_WINDOW_LENGTH];
    gaussian(TEST_WINDOW_LENGTH, 0.4, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabGaussian[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestBartlettHannWindow)
{
    float window[TEST_WINDOW_LENGTH];
    bartlett_hann(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBartlettHann[i], window[i], EPSILON);
    }
}


TEST(WindowFunctionSingle, TestKaiserWindow)
{
    float window[TEST_WINDOW_LENGTH];
    kaiser(TEST_WINDOW_LENGTH, 0.5, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabKaiser[i], window[i], EPSILON);
    }
}


TEST(WindowFunctionSingle, TestNuttallWindow)
{
    float window[TEST_WINDOW_LENGTH];
    nuttall(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabNuttall[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestBlackmanHarrisWindow)
{
    float window[TEST_WINDOW_LENGTH];
    blackman_harris(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBlackmanHarris[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionSingle, TestFlatTopWindow)
{
    float window[TEST_WINDOW_LENGTH];
    flat_top(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabFlatTop[i], window[i], EPSILON);
    }
}


TEST(WindowFunctionSingle, TestWindowFunctionProcess)
{
    float output[TEST_WINDOW_LENGTH] = {0.0};
    ClearBuffer(output, TEST_WINDOW_LENGTH);
    WindowFunction* window = WindowFunctionInit(TEST_WINDOW_LENGTH, HAMMING);
    WindowFunctionProcess(window , output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabHamming[i], output[i], EPSILON);
    }
}



#pragma mark -
#pragma mark Double-precision Tests

TEST(WindowFunctionDouble, TestBoxcarWindow)
{
    double window[TEST_WINDOW_LENGTH];
    boxcarD(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBoxcarD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestHannWindow)
{
    double window[TEST_WINDOW_LENGTH];
    hannD(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabHannD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestHammingWindow)
{
    double window[TEST_WINDOW_LENGTH];
    hammingD(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabHammingD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestBlackmanWindow)
{
    double window[TEST_WINDOW_LENGTH];
    blackmanD(TEST_WINDOW_LENGTH, 0.16, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBlackmanD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestTukeyWindow)
{
    double window[TEST_WINDOW_LENGTH];
    tukeyD(TEST_WINDOW_LENGTH, 0.5, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabTukeyD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestBartlettWindow)
{
    double window[TEST_WINDOW_LENGTH];
    bartlettD(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBartlettD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestGaussianWindow)
{
    double window[TEST_WINDOW_LENGTH];
    gaussianD(TEST_WINDOW_LENGTH, 0.4, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabGaussianD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestBartlettHannWindow)
{
    double window[TEST_WINDOW_LENGTH];
    bartlett_hannD(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBartlettHannD[i], window[i], EPSILON);
    }
}


TEST(WindowFunctionDouble, TestKaiserWindow)
{
    double window[TEST_WINDOW_LENGTH];
    kaiserD(TEST_WINDOW_LENGTH, 0.5, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabKaiserD[i], window[i], EPSILON);
    }
}


TEST(WindowFunctionDouble, TestNuttallWindow)
{
    double window[TEST_WINDOW_LENGTH];
    nuttallD(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabNuttallD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestBlackmanHarrisWindow)
{
    double window[TEST_WINDOW_LENGTH];
    blackman_harrisD(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabBlackmanHarrisD[i], window[i], EPSILON);
    }
}

TEST(WindowFunctionDouble, TestFlatTopWindow)
{
    double window[TEST_WINDOW_LENGTH];
    flat_topD(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabFlatTopD[i], window[i], EPSILON);
    }
}


TEST(WindowFunctionDouble, TestWindowFunctionProcess)
{
    double output[TEST_WINDOW_LENGTH] = {0.0};
    ClearBufferD(output, TEST_WINDOW_LENGTH);
    WindowFunctionD* window = WindowFunctionInitD(TEST_WINDOW_LENGTH, HAMMING);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(matlabHammingD[i], output[i], EPSILON);
    }
}