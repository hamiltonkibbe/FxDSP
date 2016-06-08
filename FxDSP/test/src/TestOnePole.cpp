//
//  TestOnePole.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/8/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "TestOnePole.h"
#include "OnePole.h"
#include "Dsp.h"
#include "Signals.h"
#include <gtest/gtest.h>


TEST(OnePoleSingle, TestRawInit)
{
  float exp_alpha = 0.9;
  float exp_beta = 0.1;
  OnePole *filter = OnePoleRawInit(exp_beta, exp_alpha);
  float alpha = OnePoleAlpha(filter);
  float beta = OnePoleBeta(filter);

  // Clean up
  OnePoleFree(filter);

  // Check results
  ASSERT_FLOAT_EQ(exp_alpha, alpha);
  ASSERT_FLOAT_EQ(exp_beta, beta);
}

TEST(OnePoleSingle, TestLPCoefficientCalculation)
{

    OnePole *filter = OnePoleInit(25, 100, LOWPASS);
    float alpha = OnePoleAlpha(filter);
    float beta = OnePoleBeta(filter);

    // Clean up
    OnePoleFree(filter);

    // Check results
    ASSERT_FLOAT_EQ(MatlabLpAlpha, alpha);
    ASSERT_FLOAT_EQ(MatlabLpBeta, beta);
}

TEST(OnePoleSingle, TestHPCoefficientCalculation)
{

    OnePole *filter = OnePoleInit(25, 100, HIGHPASS);
    float alpha = OnePoleAlpha(filter);
    float beta = OnePoleBeta(filter);

    // Clean up
    OnePoleFree(filter);

    // Check results
    ASSERT_FLOAT_EQ(MatlabHpAlpha, alpha);
    ASSERT_FLOAT_EQ(MatlabHpBeta, beta);
}

TEST(OnePoleSingle, TestLowpassAgainstMatlab)
{
    // Set up
    float output[50];
    ClearBuffer(output, 50);

    OnePole *filter = OnePoleInit(25, 100, LOWPASS);

    // Process
    OnePoleProcess(filter, output, MatlabSignal, 50);

    // Clean up
    OnePoleFree(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabLPOutput[i]);
    }
}

TEST(OnePoleSingle, TestFlush)
{
    // Set up
    float output[50];
    ClearBuffer(output, 50);

    OnePole *filter = OnePoleInit(25, 100, LOWPASS);

    // Process
    OnePoleProcess(filter, output, MatlabSignal, 50);
    OnePoleFlush(filter);
    OnePoleProcess(filter, output, MatlabSignal, 50);

    // Clean up
    OnePoleFree(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabLPOutput[i]);
    }
}

TEST(OnePoleSingle, TestSetSampleRate)
{
    // Set up
    float output[50];
    ClearBuffer(output, 50);

    OnePole *filter = OnePoleInit(25, 500, LOWPASS);
    OnePoleSetSampleRate(filter, 100);

    // Process
    OnePoleProcess(filter, output, MatlabSignal, 50);

    // Clean up
    OnePoleFree(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabLPOutput[i]);
    }
}

TEST(OnePoleSingle, TestSetCoefficients)
{
  float exp_alpha = 0.9;
  float exp_beta = 0.1;
  OnePole *filter = OnePoleInit(25, 500, LOWPASS);
  OnePoleSetCoefficients(filter, &exp_beta, &exp_alpha);
  float alpha = OnePoleAlpha(filter);
  float beta = OnePoleBeta(filter);

  // Clean up
  OnePoleFree(filter);

  // Check results
  ASSERT_FLOAT_EQ(exp_alpha, alpha);
  ASSERT_FLOAT_EQ(exp_beta, beta);

}

TEST(OnePoleSingle, TestLowpassTick)
{
    // Set up
    float output[50];
    ClearBuffer(output, 50);


    OnePole *filter = OnePoleInit(25, 100, LOWPASS);

    // Process
    for (unsigned i = 0; i < 50; ++i)
    {
        output[i] = OnePoleTick(filter, MatlabSignal[i]);
    }

    // Clean up
    OnePoleFree(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabLPOutput[i]);
    }
}

TEST(OnePoleSingle, TestHighpassAgainstMatlab)
{
    // Set up
    float output[50];
    ClearBuffer(output, 50);

    OnePole *filter = OnePoleInit(25, 100, HIGHPASS);

    // Process
    OnePoleProcess(filter, output, MatlabSignal, 50);

    // Clean up
    OnePoleFree(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabHPOutput[i]);
    }
}

TEST(OnePoleSingle, TestHighpassTick)
{
    // Set up
    float output[50];
    ClearBuffer(output, 50);


    OnePole *filter = OnePoleInit(25, 100, HIGHPASS);

    // Process
    for (unsigned i = 0; i < 50; ++i)
    {
        output[i] = OnePoleTick(filter, MatlabSignal[i]);
    }

    // Clean up
    OnePoleFree(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabHPOutput[i]);
    }
}


TEST(OnePoleSingle, TestTypeValidation)
{
    OnePole *filter = OnePoleInit(25, 100, LOWPASS);

    // Process
    Error_t val = OnePoleSetType(filter, LOW_SHELF);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetType(filter, HIGH_SHELF);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetType(filter, BANDPASS);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetType(filter, PEAK);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetType(filter, NOTCH);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetType(filter, ALLPASS);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetType(filter, LOWPASS);
    ASSERT_EQ(val, NOERR);

    val = OnePoleSetType(filter, HIGHPASS);
    ASSERT_EQ(val, NOERR);


    // Clean up
    OnePoleFree(filter);
    filter = 0;

    val = OnePoleSetType(filter, HIGHPASS);
    ASSERT_EQ(val, NULL_PTR_ERROR);
}


TEST(OnePoleDouble, TestRawInit)
{
  double exp_alpha = 0.9;
  double exp_beta = 0.1;
  OnePoleD *filter = OnePoleRawInitD(exp_beta, exp_alpha);
  double alpha = OnePoleAlphaD(filter);
  double beta = OnePoleBetaD(filter);

  // Clean up
  OnePoleFreeD(filter);

  // Check results
  ASSERT_DOUBLE_EQ(exp_alpha, alpha);
  ASSERT_DOUBLE_EQ(exp_beta, beta);
}

TEST(OnePoleDouble, TestLPCoefficientCalculation)
{

    OnePoleD *filter = OnePoleInitD(25, 100, LOWPASS);
    double alpha = OnePoleAlphaD(filter);
    double beta = OnePoleBetaD(filter);

    // Clean up
    OnePoleFreeD(filter);

    // Check results
    ASSERT_DOUBLE_EQ(MatlabLpAlphaD, alpha);
    ASSERT_DOUBLE_EQ(MatlabLpBetaD, beta);
}

TEST(OnePoleDouble, TestHPCoefficientCalculation)
{

    OnePoleD *filter = OnePoleInitD(25, 100, HIGHPASS);
    double alpha = OnePoleAlphaD(filter);
    double beta = OnePoleBetaD(filter);

    // Clean up
    OnePoleFreeD(filter);

    // Check results
    ASSERT_DOUBLE_EQ(MatlabHpAlphaD, alpha);
    ASSERT_DOUBLE_EQ(MatlabHpBetaD, beta);
}

TEST(OnePoleDouble, TestLowpassAgainstMatlab)
{
    // Set up
    double output[50];
    ClearBufferD(output, 50);

    OnePoleD *filter = OnePoleInitD(25, 100, LOWPASS);

    // Process
    OnePoleProcessD(filter, output, MatlabSignalD, 50);

    // Clean up
    OnePoleFreeD(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(MatlabLPOutputD[i], output[i]);
    }
}

TEST(OnePoleDouble, TestFlush)
{
    // Set up
    double output[50];
    ClearBufferD(output, 50);

    OnePoleD *filter = OnePoleInitD(25, 100, LOWPASS);

    // Process
    OnePoleProcessD(filter, output, MatlabSignalD, 50);
    OnePoleFlushD(filter);
    OnePoleProcessD(filter, output, MatlabSignalD, 50);

    // Clean up
    OnePoleFreeD(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(MatlabLPOutputD[i], output[i]);
    }
}

TEST(OnePoleDouble, TestSetSampleRate)
{
    // Set up
    double output[50];
    ClearBufferD(output, 50);

    OnePoleD *filter = OnePoleInitD(25, 500, LOWPASS);
    OnePoleSetSampleRateD(filter, 100);

    // Process
    OnePoleProcessD(filter, output, MatlabSignalD, 50);

    // Clean up
    OnePoleFreeD(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(MatlabLPOutputD[i], output[i]);
    }
}

TEST(OnePoleDouble, TestSetCoefficients)
{
  double exp_alpha = 0.9;
  double exp_beta = 0.1;
  OnePoleD *filter = OnePoleInitD(25, 500, LOWPASS);
  OnePoleSetCoefficientsD(filter, &exp_beta, &exp_alpha);
  double alpha = OnePoleAlphaD(filter);
  double beta = OnePoleBetaD(filter);

  // Clean up
  OnePoleFreeD(filter);

  // Check results
  ASSERT_DOUBLE_EQ(exp_alpha, alpha);
  ASSERT_DOUBLE_EQ(exp_beta, beta);

}

TEST(OnePoleDouble, TestLowpassTick)
{
    // Set up
    double output[50];
    ClearBufferD(output, 50);


    OnePoleD *filter = OnePoleInitD(25, 100, LOWPASS);

    // Process
    for (unsigned i = 0; i < 50; ++i)
    {
        output[i] = OnePoleTickD(filter, MatlabSignalD[i]);
    }

    // Clean up
    OnePoleFreeD(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabLPOutputD[i]);
    }
}

TEST(OnePoleDouble, TestHighpassAgainstMatlab)
{
    // Set up
    double output[50];
    ClearBufferD(output, 50);

    OnePoleD *filter = OnePoleInitD(25, 100, HIGHPASS);

    // Process
    OnePoleProcessD(filter, output, MatlabSignalD, 50);

    // Clean up
    OnePoleFreeD(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabHPOutputD[i]);
    }
}

TEST(OnePoleDouble, TestHighpassTick)
{
    // Set up
    double output[50];
    ClearBufferD(output, 50);


    OnePoleD *filter = OnePoleInitD(25, 100, HIGHPASS);

    // Process
    for (unsigned i = 0; i < 50; ++i)
    {
        output[i] = OnePoleTickD(filter, MatlabSignalD[i]);
    }

    // Clean up
    OnePoleFreeD(filter);

    // Check results
    for (unsigned i = 0; i < 50; ++i)
    {
        ASSERT_FLOAT_EQ(output[i], MatlabHPOutput[i]);
    }
}

TEST(OnePoleDouble, TestTypeValidation)
{
    OnePoleD *filter = OnePoleInitD(25, 100, LOWPASS);

    // Process
    Error_t val = OnePoleSetTypeD(filter, LOW_SHELF);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetTypeD(filter, HIGH_SHELF);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetTypeD(filter, BANDPASS);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetTypeD(filter, PEAK);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetTypeD(filter, NOTCH);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetTypeD(filter, ALLPASS);
    ASSERT_EQ(val, VALUE_ERROR);

    val = OnePoleSetTypeD(filter, LOWPASS);
    ASSERT_EQ(val, NOERR);

    val = OnePoleSetTypeD(filter, HIGHPASS);
    ASSERT_EQ(val, NOERR);

    // Clean up
    OnePoleFreeD(filter);

    filter = 0;

    val = OnePoleSetTypeD(filter, HIGHPASS);
    ASSERT_EQ(val, NULL_PTR_ERROR);

}