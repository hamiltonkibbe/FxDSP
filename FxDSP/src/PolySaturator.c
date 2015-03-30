//
//  PolySaturator.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 3/15/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "PolySaturator.h"
#include "Dsp.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct PolySaturator
{
    float a;
    float b;
    float n;
};

PolySaturator*
PolySaturatorInit(float n)
{
    PolySaturator* saturator = (PolySaturator*)malloc(sizeof(PolySaturator));
    if (saturator)
    {
        PolySaturatorSetN(saturator, n);
        return saturator;
    }
    else
    {
        return NULL;
    }
}

Error_t
PolySaturatorFree(PolySaturator* saturator)
{
    if (saturator)
    {
        free(saturator);
    }
    return NOERR;
}

Error_t
PolySaturatorSetN(PolySaturator* saturator, float n)
{
    if (saturator)
    {
        saturator->a = powf(1./n, 1./n);
        saturator->b = (n + 1) / n;
        saturator->n = n;
        return NOERR;
    }
    else
    {
        return ERROR;
    }
}

Error_t
PolySaturatorProcess(PolySaturator*     saturator,
                     float*             out_buffer,
                     const float*       in_buffer,
                     unsigned           n_samples)
{
    float buf[n_samples];
    VectorScalarMultiply(buf, (float*)in_buffer, saturator->a, n_samples);
    VectorAbs(buf, buf, n_samples);
    VectorPower(buf, buf, saturator->n, n_samples);
    VectorScalarAdd(buf, buf, -saturator->b, n_samples);
    VectorNegate(buf, buf, n_samples);
    VectorVectorMultiply(out_buffer, in_buffer, buf, n_samples);
    return NOERR;
}


float
PolySaturatorTick(PolySaturator* saturator, float in_sample)
{
    return -(powf(fabsf(saturator->a * in_sample), saturator->n) - saturator->b) * in_sample;
}