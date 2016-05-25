//
//  TestBS1770.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 7/9/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "bs1770.h"
#include "Signals.h"
#include "Dsp.h"
#include <stdio.h>
#include <gtest/gtest.h>

#define N_SAMPLE_RATES (4)
#define N_PHASES (3)


static float sample_rates[N_SAMPLE_RATES] = {22050, 44100, 48000, 96000};
static float phases[N_PHASES] = {0.0, M_PI/10, M_PI};

static double sample_ratesD[N_SAMPLE_RATES] = {22050, 44100, 48000, 96000};
static double phasesD[N_PHASES] = {0.0, M_PI/10, M_PI};

TEST(BS1770Single, BS1770Meter1k)
{
    unsigned siglen = 0;
    float sr = 0.0;
    float loudness = 0.0;
    float lpeak, rpeak, cpeak;
    float* peaks[3] = {&lpeak, &rpeak, &cpeak};
    float* signal[3];
    
    /* Test multiple sample rates */
    for (unsigned i = 0; i < N_SAMPLE_RATES; ++i)
    {
        
        sr = sample_rates[i];
        siglen = (unsigned)(sr / 2.4);
        float test_signal[siglen];
        float zero_signal[siglen];

        
        /* Generate test signals */
        ClearBuffer(zero_signal, siglen);
        
        for (unsigned phase = 0; phase < N_PHASES; ++phase)
        {
            sinewave(test_signal, siglen, 1000.0, phases[phase], 1.0, sr); /* 1kHz @ 0dBFS */

            /* Test Left, Right and Center channels */
            for (unsigned ch = 0; ch < 3; ++ch)
            {
                signal[0] = zero_signal;
                signal[1] = zero_signal;
                signal[2] = zero_signal;
                signal[ch] = test_signal;
            
                BS1770Meter* meter = BS1770MeterInit(3, sr);
                BS1770MeterProcess(meter, &loudness, peaks, (const float**)signal, siglen);
                BS1770MeterFree(meter);
                
                ASSERT_NEAR(-3.01, loudness, 0.05);
                ASSERT_NEAR(0.0, *peaks[ch], 0.03);
            }
        }
    }
}

TEST(BS1770Single, KWeightingFlush)
{
    float signal[1000];
    float run1[1000];
    float run2[1000];
    sinewave(signal, 1000, 1000.0, 0.0, 1.0, 48000);
    KWeightingFilter* filter = KWeightingFilterInit(48000);
    KWeightingFilterProcess(filter, run1, signal, 1000);
    KWeightingFilterFlush(filter);
    KWeightingFilterProcess(filter, run2, signal, 1000);
    KWeightingFilterFree(filter);
    for (unsigned i = 0; i < 1000; ++i)
    {
        ASSERT_FLOAT_EQ(run1[i], run2[i]);
    }
    
}


TEST(BS1770Double, BS1770Meter1k)
{
    unsigned siglen = 0;
    double sr = 0.0;
    double loudness = 0.0;
    double lpeak, rpeak, cpeak;
    double* peaks[3] = {&lpeak, &rpeak, &cpeak};
    double* signal[3];
    
    /* Test multiple sample rates */
    for (unsigned i = 0; i < N_SAMPLE_RATES; ++i)
    {
        sr = sample_ratesD[i];
        siglen = (unsigned)(sr / 2.4);
        double test_signal[siglen];
        double zero_signal[siglen];
        
        /* Generate test signals */
        ClearBufferD(zero_signal, siglen);
        
        for (unsigned phase = 0; phase < N_PHASES; ++phase)
        {
            sinewaveD(test_signal, siglen, 1000.0, phasesD[phase], 1.0, sr); /* 1kHz @ 0dBFS */
            
            /* Test Left, Right and Center channels */
            for (unsigned ch = 0; ch < 3; ++ch)
            {
                signal[0] = zero_signal;
                signal[1] = zero_signal;
                signal[2] = zero_signal;
                signal[ch] = test_signal;
                
                BS1770MeterD* meter = BS1770MeterInitD(3, sr);
                BS1770MeterProcessD(meter, &loudness, peaks, (const double**)signal, siglen);
                BS1770MeterFreeD(meter);
                
                ASSERT_NEAR(-3.01, loudness, 0.05);
                ASSERT_NEAR(0.0, *peaks[ch], 0.03);
            }
        }
    }
}


TEST(BS1770Double, KWeightingFlush)
{
    double signal[1000];
    double run1[1000];
    double run2[1000];
    sinewaveD(signal, 1000, 1000.0, 0.0, 1.0, 48000);
    KWeightingFilterD* filter = KWeightingFilterInitD(48000);
    KWeightingFilterProcessD(filter, run1, signal, 1000);
    KWeightingFilterFlushD(filter);
    KWeightingFilterProcessD(filter, run2, signal, 1000);
    KWeightingFilterFreeD(filter);
    for (unsigned i = 0; i < 1000; ++i)
    {
        ASSERT_DOUBLE_EQ(run1[i], run2[i]);
    }
    
}


