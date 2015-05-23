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

float
phase_correlation(float* left, float* right, unsigned n_samples);

double
phase_correlationD(double* left, double* right, unsigned n_samples);

float
balance(float* left, float* right, unsigned n_samples);

double
balanceD(double* left, double* right, unsigned n_samples);

#ifdef __cplusplus
}
#endif

#endif /* defined(__FxDSP__metering__) */
