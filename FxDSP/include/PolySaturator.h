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


typedef struct PolySaturator PolySaturator;


PolySaturator*
PolySaturatorInit(float n);

Error_t
PolySaturatorFree(PolySaturator* Saturator);

Error_t
PolySaturatorSetN(PolySaturator* saturator, float n);

Error_t
PolySaturatorProcess(PolySaturator*     saturator,
                     float*             out_buffer,
                     const float*       in_buffer,
                     unsigned           n_samples);

float
PolySaturatorTick(PolySaturator* saturator, float in_sample);


#endif
