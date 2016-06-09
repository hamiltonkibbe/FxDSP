//
//  SpectrumAnalyzer.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 6/8/16.
//  Copyright © 2016 Hamilton Kibbe. All rights reserved.
//

#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include "Error.h"
#include "WindowFunction.h"

#ifdef __cplusplus
extern "C" {
#endif

/** FFTConfig type */
typedef struct SpectrumAnalyzer SpectrumAnalyzer;
typedef struct SpectrumAnalyzerD SpectrumAnalyzerD;


SpectrumAnalyzer*
SpectrumAnalyzerInit(unsigned fft_length, float sample_rate);

SpectrumAnalyzerD*
SpectrumAnalyzerInitD(unsigned fft_length, double sample_rate);

void
SpectrumAnalyzerAnalyze(SpectrumAnalyzer* analyzer, float* signal);

void
SpectrumAnalyzerAnalyzeD(SpectrumAnalyzerD* analyzer, double* signal);

float
SpectralCentroid(SpectrumAnalyzer* analyzer);

double
SpectralCentroidD(SpectrumAnalyzerD* analyzer);

float
SpectralSpread(SpectrumAnalyzer* analyzer);

double
SpectralSpreadD(SpectrumAnalyzerD* analyzer);

float
SpectralSkewness(SpectrumAnalyzer* analyzer);

double
SpectralSkewnessD(SpectrumAnalyzerD* analyzer);

float
SpectralKurtosis(SpectrumAnalyzer* analyzer);

double
SpectralKurtosisD(SpectrumAnalyzerD* analyzer);

#ifdef __cplusplus
}
#endif

#endif /* Spectrum_h */
