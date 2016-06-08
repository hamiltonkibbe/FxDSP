//
//  metering.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/22/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "Metering.h"
#include "Dsp.h"

#include <math.h>
#include <float.h>

/* pow(10, (-12./20.)) */
#define K12_REF (0.25118864315095801309)

/* pow(10, (-12./20.)); */
#define K14_REF (0.19952623149688797355)

/* pow(10, (-20./20.)); */
#define K20_REF (0.1)


static const float ref[] = {1.0, (float)K12_REF, (float)K14_REF, (float)K20_REF};
static const double refD[] = {1.0, K12_REF, K14_REF, K20_REF};

float
phase_correlation(float* left, float* right, unsigned n_samples)
{
    float product = 0.0;
    float lsq = 0.0;
    float rsq = 0.0;
    float denom = 0.0;

    for (unsigned i = 0; i < n_samples; ++i)
    {
        float left_sample = left[i];
        float right_sample = right[i];

        product += left_sample * right_sample;
        lsq += left_sample * left_sample;
        rsq += right_sample * right_sample;
    }

    denom = lsq * rsq;

    if (denom > 0.0)
    {
        return product / sqrtf(denom);
    }
    else
    {
        return 1.0;
    }
}


double
phase_correlationD(double* left, double* right, unsigned n_samples)
{
    double product = 0.0;
    double lsq = 0.0;
    double rsq = 0.0;
    double denom = 0.0;

    for (unsigned i = 0; i < n_samples; ++i)
    {
        double left_sample = left[i];
        double right_sample = right[i];

        product += left_sample * right_sample;
        lsq += left_sample * left_sample;
        rsq += right_sample * right_sample;
    }

    denom = lsq * rsq;

    if (denom > 0.0)
    {
        return product / sqrt(denom);
    }
    else
    {
        return 1.0;
    }
}


float
balance(float* left, float* right, unsigned n_samples)
{
    float r = 0.0;
    float l = 0.0;
    float rbuf[n_samples];
    float lbuf[n_samples];
    VectorPower(rbuf, right, 2.0, n_samples);
    VectorPower(lbuf, left, 2.0, n_samples);
    r = VectorSum(rbuf, n_samples);
    l = VectorSum(lbuf, n_samples);
    return  (r - l) / ((r + l) + FLT_MIN);
}


double
balanceD(double* left, double* right, unsigned n_samples)
{
    double r = 0.0;
    double l = 0.0;
    double rbuf[n_samples];
    double lbuf[n_samples];
    VectorPowerD(rbuf, right, 2.0, n_samples);
    VectorPowerD(lbuf, left, 2.0, n_samples);
    r = VectorSumD(rbuf, n_samples);
    l = VectorSumD(lbuf, n_samples);
    return  (r - l) / ((r + l) + DBL_MIN);
}


float
vu_peak(float* signal, unsigned n_samples, MeterScale scale)
{
    float peak = VectorMax(signal, n_samples);
    return 20.0 * log10f(peak / ref[scale]);
}

double
vu_peakD(double* signal, unsigned n_samples, MeterScale scale)
{
    {
        double peak = VectorMaxD(signal, n_samples);
        return 20 * log10(peak / refD[scale]);
    }
}

