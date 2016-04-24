//
//  DiodeRectifier.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 12/9/14.
//  Copyright (c) 2014 Hamilton Kibbe. All rights reserved.
//

#include "DiodeSaturator.h"
#include "Utilities.h"
#include <stdlib.h>
#include <math.h>



#define E_INV (0.36787944117144233)

/*******************************************************************************
 Diode */
struct DiodeSaturator
{
    bias_t  bias;
    float   amount;
};

struct DiodeSaturatorD
{
    bias_t  bias;
    double  amount;
};

/*******************************************************************************
 DiodeInit */
DiodeSaturator*
DiodeSaturatorInit(bias_t bias, float amount)
{
    // Create saturator struct
    DiodeSaturator* saturator = (DiodeSaturator*)malloc(sizeof(DiodeSaturator));
    
    // Initialization
    saturator->bias = bias;
    saturator->amount = amount;
    return saturator;
}

DiodeSaturatorD*
DiodeSaturatorInitD(bias_t bias, double amount)
{
    // Create saturator struct
    DiodeSaturatorD* saturator = (DiodeSaturatorD*)malloc(sizeof(DiodeSaturatorD));
    
    // Initialization
    saturator->bias = bias;
    saturator->amount = amount;
    return saturator;
}


/*******************************************************************************
 DiodeFree */
Error_t
DiodeSaturatorFree(DiodeSaturator* saturator)
{
    if(saturator)
        free(saturator);
    saturator = NULL;
    return NOERR;
}

Error_t
DiodeSaturatorFreeD(DiodeSaturatorD* saturator)
{
    if(saturator)
        free(saturator);
    saturator = NULL;
    return NOERR;
}


/*******************************************************************************
 DiodeSetThreshold */
Error_t
DiodeSaturatorSetAmount(DiodeSaturator* saturator, float amount)
{
    saturator->amount = 0.5 * powf(amount, 0.5);
    return NOERR;
}

Error_t
DiodeSaturatorSetThresholdD(DiodeSaturatorD* saturator, double amount)
{
    saturator->amount = 0.5 * pow(amount, 0.5);
    return NOERR;
}


/*******************************************************************************
 DiodeProcess */
Error_t
DiodeSaturatorProcess(DiodeSaturator*   saturator,
                      float*            out_buffer,
                      const float*      in_buffer,
                      unsigned          n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = in_buffer[i] - (saturator->amount * (F_EXP((in_buffer[i]/0.7) - 1.0) + E_INV));
    }
    return NOERR;
}

Error_t
DiodeSaturatorProcessD(DiodeSaturatorD* saturator,
                       double*          out_buffer,
                       const double*    in_buffer,
                       unsigned         n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = in_buffer[i] - (saturator->amount * (F_EXP((in_buffer[i]/0.7) - 1.0) + E_INV));
    }
    return NOERR;
}


/*******************************************************************************
 DiodeTick */
float
DiodeSaturatorTick(DiodeSaturator* saturator, float in_sample)
{
    return in_sample - (saturator->amount * (F_EXP((in_sample/0.7) - 1.0) + E_INV));
}

double
DiodeSaturatorTickD(DiodeSaturatorD* saturator, double in_sample)
{
    return in_sample - (saturator->amount * (F_EXP((in_sample/0.7) - 1.0) + E_INV));
}

