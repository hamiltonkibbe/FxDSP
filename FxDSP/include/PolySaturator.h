/*
 *  PolySaturator.h
 *  FxDSP
 *
 *  Created by Hamilton Kibbe on 3/15/15.
 *  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
 *
 *  Polynomial Saturator
 */

#ifndef FxDSP_saturation_h
#define FxDSP_saturation_h

#include "Error.h"
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct PolySaturator PolySaturator;
typedef struct PolySaturatorD PolySaturatorD;


PolySaturator*
PolySaturatorInit(float n);

PolySaturatorD*
PolySaturatorInitD(double n);

Error_t
PolySaturatorFree(PolySaturator* Saturator);

Error_t
PolySaturatorFreeD(PolySaturatorD* Saturator);

Error_t
PolySaturatorSetN(PolySaturator* saturator, float n);

Error_t
PolySaturatorSetND(PolySaturatorD* saturator, double n);

Error_t
PolySaturatorProcess(PolySaturator* saturator,
                     float*         out_buffer,
                     const float*   in_buffer,
                     unsigned       n_samples);

Error_t
PolySaturatorProcessD(PolySaturatorD* saturator,
                      double*         out_buffer,
                      const double*   in_buffer,
                      unsigned        n_samples);


float
PolySaturatorTick(PolySaturator* saturator, float in_sample);

double
PolySaturatorTickD(PolySaturatorD* saturator, double in_sample);


#ifdef __cplusplus
}
#endif

#endif
