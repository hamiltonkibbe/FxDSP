//
//  TestDiodeRectifier.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 8/2/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "DiodeRectifier.h"
#include "Signals.h"

#include <stdio.h>
#include <gtest/gtest.h>

TEST(DiodeRectifierSingle, TestProcessForward)
{
    float in[1000];
    float out[1000];
    sinewave(in, 1000, 1000.0, 0, 1.0, 44100);
    DiodeRectifier * diode = DiodeRectifierInit(FORWARD_BIAS, 0.7);
    DiodeRectifierProcess(diode, out, in, 1000);
    DiodeRectifierFree(diode);
    for (unsigned i = 0; i < 1000; ++i)
    {
        ASSERT_GE(out[i], 0);
    }
}


TEST(DiodeRectifierSingle, TestProcessReverse)
{
    float in[1000];
    float out[1000];
    sinewave(in, 1000, 1000.0, 0, 1.0, 44100);
    DiodeRectifier * diode = DiodeRectifierInit(REVERSE_BIAS, 0.7);
    DiodeRectifierProcess(diode, out, in, 1000);
    DiodeRectifierFree(diode);
    for (unsigned i = 0; i < 1000; ++i)
    {
        ASSERT_LE(out[i], 0);
    }
}

TEST(DiodeRectifierSingle, TestTick)
{
    float in[1000];
    sinewave(in, 1000, 1000.0, 0, 1.0, 44100);
    DiodeRectifier * diode = DiodeRectifierInit(FORWARD_BIAS, 0.7);
    for (unsigned i = 0; i < 1000; ++i)
    {
        ASSERT_GE(DiodeRectifierTick(diode, in[i]), 0);
    }
    DiodeRectifierFree(diode);
}



TEST(DiodeRectifierDoube, TestProcessForward)
{
    double in[1000];
    double out[1000];
    sinewaveD(in, 1000, 1000.0, 0, 1.0, 44100);
    DiodeRectifierD * diode = DiodeRectifierInitD(FORWARD_BIAS, 0.7);
    DiodeRectifierProcessD(diode, out, in, 1000);
    DiodeRectifierFreeD(diode);
    for (unsigned i = 0; i < 1000; ++i)
    {
        ASSERT_GE(out[i], 0);
    }
}

TEST(DiodeRectifierDouble, TestProcessReverse)
{
    double in[1000];
    double out[1000];
    sinewaveD(in, 1000, 1000.0, 0, 1.0, 44100);
    DiodeRectifierD * diode = DiodeRectifierInitD(REVERSE_BIAS, 0.7);
    DiodeRectifierProcessD(diode, out, in, 1000);
    DiodeRectifierFreeD(diode);
    for (unsigned i = 0; i < 1000; ++i)
    {
        ASSERT_LE(out[i], 0);
    }
}


TEST(DiodeRectifierDouble, TestTick)
{
    double in[1000];
    sinewaveD(in, 1000, 1000.0, 0, 1.0, 44100);
    DiodeRectifierD * diode = DiodeRectifierInitD(FORWARD_BIAS, 0.7);
    for (unsigned i = 0; i < 1000; ++i)
    {
        ASSERT_GE(DiodeRectifierTickD(diode, in[i]), 0);
    }
    DiodeRectifierFreeD(diode);
}