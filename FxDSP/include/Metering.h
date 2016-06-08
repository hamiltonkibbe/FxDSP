//
//  metering.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/22/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#ifndef __FxDSP__metering__
#define __FxDSP__metering__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    FULL_SCALE,
    K_12,
    K_14,
    K_20
} MeterScale;


float
phase_correlation(float* left, float* right, unsigned n_samples);

double
phase_correlationD(double* left, double* right, unsigned n_samples);

float
balance(float* left, float* right, unsigned n_samples);

double
balanceD(double* left, double* right, unsigned n_samples);


float
vu_peak(float* signal, unsigned n_samples, MeterScale scale);

double
vu_peakD(double* signal, unsigned n_samples, MeterScale scale);


#ifdef __cplusplus
}
#endif

#endif /* defined(__FxDSP__metering__) */
