//
//  SpectrumAnalyzer.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 6/8/16.
//  Copyright © 2016 Hamilton Kibbe. All rights reserved.
//

#include "SpectrumAnalyzer.h"
#include "Dsp.h"
#include "FFT.h"

#include <stddef.h>
#include <stdlib.h>


/*******************************************************************************
 Static Function Prototypes */

static void
calculate_bin_frequencies(float* dest, unsigned fft_length, float sample_rate);

static void
calculate_bin_frequenciesD(double* dest, unsigned fft_length, double sample_rate);


/*******************************************************************************
 Structs */

struct SpectrumAnalyzer
{
    unsigned        fft_length;
    unsigned        bins;
    float           sample_rate;
    float           mag_sum;
    float*          frequencies;
    float*          real;
    float*          imag;
    float*          mag;
    float*          phase;
    float*          root_moment;
    FFTConfig*      fft;
    Window_t        window_type;
    WindowFunction* window;
};

struct SpectrumAnalyzerD
{
    unsigned            fft_length;
    unsigned            bins;
    double              sample_rate;
    double              mag_sum;
    double*             frequencies;
    double*             real;
    double*             imag;
    double*             mag;
    double*             phase;
    double*             root_moment;
    FFTConfigD*         fft;
    Window_t            window_type;
    WindowFunctionD*    window;
};

SpectrumAnalyzer*
SpectrumAnalyzerInit(unsigned fft_length, float sample_rate)
{
    SpectrumAnalyzer* inst = (SpectrumAnalyzer*) malloc(sizeof(SpectrumAnalyzer));
    if (NULL != inst)
    {
        FFTConfig* fft = FFTInit(fft_length);
        WindowFunction* window = WindowFunctionInit(fft_length, BLACKMAN);
        float* frequencies = (float*)malloc((fft_length / 2) * sizeof(float));
        float* real = (float*)malloc((fft_length / 2) * sizeof(float));
        float* imag = (float*)malloc((fft_length / 2) * sizeof(float));
        float* mag = (float*)malloc((fft_length / 2) * sizeof(float));
        float* phase = (float*)malloc((fft_length / 2) * sizeof(float));
        float* root_moment = (float*)malloc((fft_length / 2) * sizeof(float));
        if ((NULL != window) && (NULL != fft) && (NULL != frequencies) && (NULL != real) \
            && (NULL != imag) && (NULL != mag) && (NULL != phase) && (NULL != root_moment))
        {
            inst->fft_length = fft_length;
            inst->bins = fft_length / 2;
            inst->sample_rate = sample_rate;
            inst->mag_sum = 0.0;
            inst->frequencies = frequencies;
            inst->real = real;
            inst->imag = imag;
            inst->mag = mag;
            inst->phase = phase;
            inst->root_moment = root_moment;
            inst->fft = fft;
            inst->window = window;
            inst->window_type = BLACKMAN;
            calculate_bin_frequencies(inst->frequencies, fft_length, sample_rate);
            *(inst->root_moment) = 0.0;
        }
        else
        {
            WindowFunctionFree(window);
            FFTFree(fft);
            if (NULL != root_moment)
            {
                free(root_moment);
            }
            if (NULL != phase)
            {
                free(phase);
            }
            if (NULL != mag)
            {
                free(mag);
            }
            if (NULL != imag)
            {
                free(imag);
            }
            if (NULL != real)
            {
                free(real);
            }
            if (NULL != frequencies)
            {
                free(real);
            }
            inst = NULL;
        }
    }
    return inst;
}

SpectrumAnalyzerD*
SpectrumAnalyzerInitD(unsigned fft_length, double sample_rate)
{
    SpectrumAnalyzerD* inst = (SpectrumAnalyzerD*) malloc(sizeof(SpectrumAnalyzerD));
    if (NULL != inst)
    {
        FFTConfigD* fft = FFTInitD(fft_length);
        WindowFunctionD* window = WindowFunctionInitD(fft_length, BLACKMAN);
        double* frequencies = (double*)malloc((fft_length / 2) * sizeof(double));
        double* real = (double*)malloc((fft_length / 2) * sizeof(double));
        double* imag = (double*)malloc((fft_length / 2) * sizeof(double));
        double* mag = (double*)malloc((fft_length / 2) * sizeof(double));
        double* phase = (double*)malloc((fft_length / 2) * sizeof(double));
        double* root_moment = (double*)malloc((fft_length / 2) * sizeof(double));
        if ((NULL != window) && (NULL != fft) && (NULL != frequencies) && (NULL != real) \
            && (NULL != imag) && (NULL != mag) && (NULL != phase) && (NULL != root_moment))
        {
            inst->fft_length = fft_length;
            inst->bins = fft_length/2;
            inst->sample_rate = sample_rate;
            inst->mag_sum = 0.0;
            inst->frequencies = frequencies;
            inst->real = real;
            inst->imag = imag;
            inst->mag = mag;
            inst->phase = phase;
            inst->root_moment = root_moment;
            inst->fft = fft;
            inst->window = window;
            inst->window_type = BLACKMAN;
            calculate_bin_frequenciesD(inst->frequencies, fft_length, sample_rate);
            *(inst->root_moment) = 0.0;
        }
        else
        {
            WindowFunctionFreeD(window);
            FFTFreeD(fft);
            if (NULL != root_moment)
            {
                free(root_moment);
            }
            if (NULL != phase)
            {
                free(phase);
            }
            if (NULL != mag)
            {
                free(mag);
            }
            if (NULL != imag)
            {
                free(imag);
            }
            if (NULL != real)
            {
                free(real);
            }
            if (NULL != frequencies)
            {
                free(frequencies);
            }
            inst = NULL;
        }
    }
    return inst;
}



void
SpectrumAnalyzerAnalyze(SpectrumAnalyzer* analyzer, float* signal)
{
    float scratch[analyzer->fft_length];
    WindowFunctionProcess(analyzer->window, scratch, signal, analyzer->fft_length);
    FFT_R2C(analyzer->fft, scratch, analyzer->real, analyzer->imag);
    VectorRectToPolar(analyzer->mag, analyzer->phase, analyzer->real, analyzer->imag, analyzer->bins);
    analyzer->mag_sum = VectorSum(analyzer->mag, analyzer->bins);
    analyzer->root_moment[0] = 0.0;
}

void
SpectrumAnalyzerAnalyzeD(SpectrumAnalyzerD* analyzer, double* signal)
{
    double scratch[analyzer->fft_length];
    WindowFunctionProcessD(analyzer->window, scratch, signal, analyzer->fft_length);
    FFT_R2CD(analyzer->fft, scratch, analyzer->real, analyzer->imag);
    VectorRectToPolarD(analyzer->mag, analyzer->phase, analyzer->real, analyzer->imag, analyzer->bins);
    analyzer->mag_sum = VectorSumD(analyzer->mag, analyzer->bins);
    analyzer->root_moment[0] = 0.0;
}

float
SpectralCentroid(SpectrumAnalyzer* analyzer)
{
    float num[analyzer->bins];
    VectorVectorMultiply(num, analyzer->mag, analyzer->frequencies, analyzer->bins);
    return VectorSum(num, analyzer->bins) / analyzer->mag_sum;
}

double
SpectralCentroidD(SpectrumAnalyzerD* analyzer)
{
    double num[analyzer->bins];
    VectorVectorMultiplyD(num, analyzer->mag, analyzer->frequencies, analyzer->bins);
    return VectorSumD(num, analyzer->bins) / analyzer->mag_sum;
}

float
SpectralSpread(SpectrumAnalyzer* analyzer)
{
    float mu = SpectralCentroid(analyzer);
    float num[analyzer->bins];
    if (analyzer->root_moment[0] == 0.0)
    {
        VectorScalarAdd(analyzer->root_moment, analyzer->frequencies, -mu, analyzer->bins);
    }
    VectorPower(num, analyzer->root_moment, 2, analyzer->bins);
    return VectorSum(num, analyzer->bins) / analyzer->mag_sum;
}

double
SpectralSpreadD(SpectrumAnalyzerD* analyzer)
{
    double mu = SpectralCentroidD(analyzer);
    double num[analyzer->bins];
    if (analyzer->root_moment[0] == 0.0)
    {
        VectorScalarAddD(analyzer->root_moment, analyzer->frequencies, -mu, analyzer->bins);
    }
    VectorPowerD(num, analyzer->root_moment, 2, analyzer->bins);
    return VectorSumD(num, analyzer->bins) / analyzer->mag_sum;
}

float
SpectralSkewness(SpectrumAnalyzer* analyzer)
{
    float mu = SpectralCentroid(analyzer);
    float num[analyzer->bins];
    if (analyzer->root_moment[0] == 0.0)
    {
        VectorScalarAdd(analyzer->root_moment, analyzer->frequencies, -mu, analyzer->bins);
    }
    VectorPower(num, analyzer->root_moment, 3, analyzer->bins);
    return VectorSum(num, analyzer->bins) / analyzer->mag_sum;
}

double
SpectralSkewnessD(SpectrumAnalyzerD* analyzer)
{
    double mu = SpectralCentroidD(analyzer);
    double num[analyzer->bins];
    if (analyzer->root_moment[0] == 0.0)
    {
        VectorScalarAddD(analyzer->root_moment, analyzer->frequencies, -mu, analyzer->bins);
    }
    VectorPowerD(num, analyzer->root_moment, 3, analyzer->bins);
    return VectorSumD(num, analyzer->bins) / analyzer->mag_sum;
}

float
SpectralKurtosis(SpectrumAnalyzer* analyzer)
{
    float mu = SpectralCentroid(analyzer);
    float num[analyzer->bins];
    if (analyzer->root_moment[0] == 0.0)
    {
        VectorScalarAdd(analyzer->root_moment, analyzer->frequencies, -mu, analyzer->bins);
    }
    VectorPower(num, analyzer->root_moment, 4, analyzer->bins);
    return VectorSum(num, analyzer->bins) / analyzer->mag_sum;
}

double
SpectralKurtosisD(SpectrumAnalyzerD* analyzer)
{
    double mu = SpectralCentroidD(analyzer);
    double num[analyzer->bins];
    if (analyzer->root_moment[0] == 0.0)
    {
        VectorScalarAddD(analyzer->root_moment, analyzer->frequencies, -mu, analyzer->bins);
    }
    VectorPowerD(num, analyzer->root_moment, 4, analyzer->bins);
    return VectorSumD(num, analyzer->bins) / analyzer->mag_sum;
}


/*******************************************************************************
 Static Funcdtion Definitions */
static void
calculate_bin_frequencies(float* dest, unsigned fft_length, float sample_rate)
{
    float freq_step = sample_rate / fft_length;
    for(unsigned bin = 0; bin < fft_length / 2; ++bin)
    {
        dest[bin] = bin * freq_step;
    }
}

static void
calculate_bin_frequenciesD(double* dest, unsigned fft_length, double sample_rate)
{
    double freq_step = sample_rate / fft_length;
    for(unsigned bin = 0; bin < fft_length / 2; ++bin)
    {
        dest[bin] = bin * freq_step;
    }
}

