//
//  TestSpectrumAnalyzer.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 6/8/16.
//  Copyright © 2016 Hamilton Kibbe. All rights reserved.
//

#include "SpectrumAnalyzer.h"
#include "Signals.h"

#include <gtest/gtest.h>




TEST(SpectrumAnalyzer, SpectralCentroid)
{
    unsigned length = 2048;
    float sample_rate = 44100.0;
    float signal[length];
    sinewave(signal, length, 1000, 0, 1.0, sample_rate);
    SpectrumAnalyzer* analyzer = SpectrumAnalyzerInit(length, sample_rate);
    SpectrumAnalyzerAnalyze(analyzer, signal);
    float centroid = SpectralCentroid(analyzer);
    ASSERT_NEAR(centroid, 1000 , sample_rate/(2.0*length));
}

TEST(SpectrumAnalyzerD, SpectralCentroid)
{
    unsigned length = 2048;
    double sample_rate = 44100.0;
    double signal[length];
    sinewaveD(signal, length, 1000, 0, 1.0, sample_rate);
    SpectrumAnalyzerD* analyzer = SpectrumAnalyzerInitD(length, sample_rate);
    SpectrumAnalyzerAnalyzeD(analyzer, signal);
    float centroid = SpectralCentroidD(analyzer);
    ASSERT_NEAR(centroid, 1000 , sample_rate/(2.0*length));
}