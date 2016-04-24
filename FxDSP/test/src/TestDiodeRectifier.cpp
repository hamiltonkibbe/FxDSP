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


const float thresholds[11] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

TEST(DiodeRectifierSingle, TestProcessForward)
{
    float in[1000];
    float out[1000];
    sinewave(in, 1000, 1000.0, 0, 1.0, 44100);
    for (unsigned th_idx = 0; th_idx < 11; ++th_idx)
    {
        DiodeRectifier * diode = DiodeRectifierInit(FORWARD_BIAS, thresholds[th_idx]);
        DiodeRectifierProcess(diode, out, in, 1000);
        DiodeRectifierFree(diode);

        for (unsigned i = 0; i < 1000; ++i)
        {
            ASSERT_GE(out[i], 0.0);
        }
    }
}


TEST(DiodeRectifierSingle, TestProcessReverse)
{
    float in[1000];
    float out[1000];
    sinewave(in, 1000, 1000.0, 0, 1.0, 44100);
    for (unsigned th_idx = 0; th_idx < 11; ++th_idx)
    {
        DiodeRectifier * diode = DiodeRectifierInit(REVERSE_BIAS, thresholds[th_idx]);
        DiodeRectifierProcess(diode, out, in, 1000);
        DiodeRectifierFree(diode);
        for (unsigned i = 0; i < 1000; ++i)
        {
            ASSERT_LE(out[i], 0.0);
        }
    }
}

TEST(DiodeRectifierSingle, TestTick)
{
    float in[1000];
    sinewave(in, 1000, 1000.0, 0, 1.0, 44100);
    for (unsigned th_idx = 0; th_idx < 11; ++th_idx)
    {
        DiodeRectifier * diode = DiodeRectifierInit(FORWARD_BIAS, thresholds[th_idx]);
        for (unsigned i = 0; i < 1000; ++i)
        {
            ASSERT_GE(DiodeRectifierTick(diode, in[i]), 0.0);
        }
        DiodeRectifierFree(diode);
    }
}



TEST(DiodeRectifierDoube, TestProcessForward)
{
    double in[1000];
    double out[1000];
    sinewaveD(in, 1000, 1000.0, 0, 1.0, 44100);
    for (unsigned th_idx = 0; th_idx < 11; ++th_idx)
    {
        DiodeRectifierD * diode = DiodeRectifierInitD(FORWARD_BIAS, (double)thresholds[th_idx]);
        DiodeRectifierProcessD(diode, out, in, 1000);
        DiodeRectifierFreeD(diode);
        for (unsigned i = 0; i < 1000; ++i)
        {
            ASSERT_GE(out[i], 0.0);
        }
    }
}

TEST(DiodeRectifierDouble, TestProcessReverse)
{
    double in[1000];
    double out[1000];
    sinewaveD(in, 1000, 1000.0, 0, 1.0, 44100);
    for (unsigned th_idx = 0; th_idx < 11; ++th_idx)
    {
        DiodeRectifierD * diode = DiodeRectifierInitD(REVERSE_BIAS, (double)thresholds[th_idx]);
        DiodeRectifierProcessD(diode, out, in, 1000);
        DiodeRectifierFreeD(diode);
        for (unsigned i = 0; i < 1000; ++i)
        {
            ASSERT_LE(out[i], 0.0);
        }
    }
}


TEST(DiodeRectifierDouble, TestTick)
{
    double in[1000];
    sinewaveD(in, 1000, 1000.0, 0, 1.0, 44100);
    for (unsigned th_idx = 0; th_idx < 11; ++th_idx)
    {
        DiodeRectifierD * diode = DiodeRectifierInitD(FORWARD_BIAS, (double)thresholds[th_idx]);
        for (unsigned i = 0; i < 1000; ++i)
        {
            ASSERT_GE(DiodeRectifierTickD(diode, in[i]), 0.0);
        }
        DiodeRectifierFreeD(diode);
    }
}