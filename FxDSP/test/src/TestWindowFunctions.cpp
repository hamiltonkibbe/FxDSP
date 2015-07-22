//
//  TestWindowFunctions.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/8/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "TestWindowFunctions.h"
#include "WindowFunction.h"
#include "Dsp.h"
#include "Signals.h"
#include <gtest/gtest.h>

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

TEST(WindowFunctionSingle, TestCosineWindow)
{
    float out[10];
    cosine(9,out);
    ASSERT_NEAR(0.0, out[0], EPSILON);
    ASSERT_NEAR(1.0, out[4], EPSILON);
    ASSERT_NEAR(0.0, out[8], EPSILON);
}


TEST(WindowFunctionSingle, TestLanczosWindowBounds)
{
    float out[20];
    for (unsigned i = 2; i < 20; ++i)
    {
        lanczos(i, out);
        for (unsigned j = 0; j < i; ++j)
        {
            ASSERT_TRUE(out[j] <= 1.0);
        }
    }
}

TEST(WindowFunctionSingle, TestBlackmanNuttallWindowBounds)
{
    float out[20];
    for (unsigned i = 2; i < 20; ++i)
    {
        blackman_nuttall(i, out);
        for (unsigned j = 0; j < i; ++j)
        {
            ASSERT_TRUE(out[j] <= 1.0);
        }
    }
}


TEST(WindowFunctionSingle, TestPoissonWindowBounds)
{
    float out[20];
    for (unsigned i = 2; i < 20; ++i)
    {
        for (unsigned j = 1; j < 100; ++j)
        {
            poisson(i, j, out);
            for (unsigned k = 0; k < i; ++k)
            {
                ASSERT_TRUE(out[k] <= 1.0);
            }
        }
    }
}

TEST(WindowFunctionSingle, TestChebyshevWindow)
{
    float out[10];
    chebyshev(10, 100, out);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(matlabChebyshev[i], out[i], EPSILON);
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
    

    window = WindowFunctionInit(TEST_WINDOW_LENGTH, BOXCAR);
    WindowFunctionProcess(window , output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(ones[i], output[i], EPSILON);
    }
    

    window = WindowFunctionInit(TEST_WINDOW_LENGTH, HANN);
    WindowFunctionProcess(window ,output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabHann[i], output[i], EPSILON);
    }
    

    window = WindowFunctionInit(TEST_WINDOW_LENGTH, BLACKMAN);
    WindowFunctionProcess(window, output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabBlackman[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInit(TEST_WINDOW_LENGTH, TUKEY);
    WindowFunctionProcess(window, output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabTukey[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInit(TEST_WINDOW_LENGTH, BARTLETT);
    WindowFunctionProcess(window, output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabBartlett[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInit(TEST_WINDOW_LENGTH, GAUSSIAN);
    WindowFunctionProcess(window, output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabGaussian[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInit(TEST_WINDOW_LENGTH, BARTLETT_HANN);
    WindowFunctionProcess(window, output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabBartlettHann[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInit(TEST_WINDOW_LENGTH, KAISER);
    WindowFunctionProcess(window, output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabKaiser[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInit(TEST_WINDOW_LENGTH, NUTTALL);
    WindowFunctionProcess(window, output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabNuttall[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInit(TEST_WINDOW_LENGTH, BLACKMAN_HARRIS);
    WindowFunctionProcess(window, output, ones, TEST_WINDOW_LENGTH);
    WindowFunctionFree(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabBlackmanHarris[i], output[i], EPSILON);
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


TEST(WindowFunctionDouble, TestChebyshevWindow)
{
    double out[10];
    chebyshevD(10, 100, out);
    for (unsigned i = 0; i < 10; ++i)
    {
        ASSERT_NEAR(matlabChebyshevD[i], out[i], EPSILON);
    }
}

/* Need a better way to test these... */
TEST(WindowFunctionDouble, TestCosineWindow)
{
    double out[10];
    cosineD(9,out);
    ASSERT_NEAR(0.0, out[0], EPSILON);
    ASSERT_NEAR(1.0, out[4], EPSILON);
    ASSERT_NEAR(0.0, out[8], EPSILON);
}

TEST(WindowFunctionDouble, TestLanczosWindowBounds)
{
    double out[20];
    for (unsigned i = 2; i < 20; ++i)
    {
        lanczosD(i, out);
        for (unsigned j = 0; j < i; ++j)
        {
            ASSERT_TRUE(out[j] <= 1.0);
        }
    }
}


TEST(WindowFunctionDouble, TestBlackmanNuttallWindowBounds)
{
    double out[20];
    for (unsigned i = 2; i < 20; ++i)
    {
        blackman_nuttallD(i, out);
        for (unsigned j = 0; j < i; ++j)
        {
            ASSERT_TRUE(out[j] <= 1.0);
        }
    }
}


TEST(WindowFunctionDouble, TestPoissonWindowBounds)
{
    double out[20];
    for (unsigned i = 2; i < 20; ++i)
    {
        for (unsigned j = 1; j < 100; ++j)
        {
            poissonD(i, j, out);
            for (unsigned k = 0; k < i; ++k)
            {
                ASSERT_TRUE(out[k] <= 1.0);
            }
        }
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
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, BOXCAR);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        ASSERT_NEAR(onesD[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, HANN);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabHannD[i], output[i], EPSILON);
    }

    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, BLACKMAN);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabBlackmanD[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, TUKEY);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabTukeyD[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, BARTLETT);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabBartlettD[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, GAUSSIAN);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabGaussianD[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, BARTLETT_HANN);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabBartlettHannD[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, KAISER);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabKaiserD[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, NUTTALL);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabNuttallD[i], output[i], EPSILON);
    }
    
    window = WindowFunctionInitD(TEST_WINDOW_LENGTH, BLACKMAN_HARRIS);
    WindowFunctionProcessD(window , output, onesD, TEST_WINDOW_LENGTH);
    WindowFunctionFreeD(window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH-1; ++i)
    {
        ASSERT_NEAR(matlabBlackmanHarrisD[i], output[i], EPSILON);
    }
}