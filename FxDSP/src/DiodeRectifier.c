//
//  DiodeRectifier.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 12/9/14.
//  Copyright (c) 2014 Hamilton Kibbe. All rights reserved.
//

#include "DiodeRectifier.h"
#include "Utilities.h"
#include <stdlib.h>
#include <math.h>


/*******************************************************************************
 DiodeRectifier */
struct DiodeRectifier
{
    bias_t  bias;
    float   threshold;
    float   vt;
    float   scale;
};

struct DiodeRectifierD
{
    bias_t  bias;
    double  threshold;
    double  vt;
    double  scale;
};


/*******************************************************************************
 DiodeRectifierInit */
DiodeRectifier*
DiodeRectifierInit(bias_t bias, float threshold)
{
    // Create opto struct
    DiodeRectifier* diode = (DiodeRectifier*)malloc(sizeof(DiodeRectifier));
    
    // Initialization
    diode->bias = bias;
    diode->threshold = threshold;
    DiodeRectifierSetThreshold(diode, threshold);
    return diode;
}

DiodeRectifierD*
DiodeRectifierInitD(bias_t bias, double threshold)
{
    // Create opto struct
    DiodeRectifierD* diode = (DiodeRectifierD*)malloc(sizeof(DiodeRectifierD));
    
    // Initialization
    diode->bias = bias;
    diode->threshold = threshold;
    DiodeRectifierSetThresholdD(diode, threshold);
    return diode;
}


/*******************************************************************************
 DiodeRectifierFree */
Error_t
DiodeRectifierFree(DiodeRectifier* diode)
{
    if(diode)
        free(diode);
    diode = NULL;
    return NOERR;
}

Error_t
DiodeRectifierFreeD(DiodeRectifierD* diode)
{
    if(diode)
        free(diode);
    diode = NULL;
    return NOERR;
}


/*******************************************************************************
 DiodeRectifierSetThreshold */
Error_t
DiodeRectifierSetThreshold(DiodeRectifier* diode, float threshold)
{
    float scale = 1.0;
    if (diode->bias== REVERSE_BIAS)
    {
        scale = -1.0;
    }
    diode->threshold = threshold;
    diode->vt = scale * -0.1738 * threshold + 0.1735;
    diode->scale = scale/(expf((1.0/diode->vt) - 1.));
    return NOERR;
}

Error_t
DiodeRectifierSetThresholdD(DiodeRectifierD* diode, double threshold)
{
    double scale = 1.0;
    if (diode->bias == REVERSE_BIAS)
    {
        scale = -1.0;
    }
    diode->threshold = threshold;
    diode->vt = scale * -0.1738 * threshold + 0.1735;
    diode->scale = scale/(expf((1.0/diode->vt) - 1.));
    return NOERR;
}


/*******************************************************************************
 DiodeRectifierProcess */
Error_t
DiodeRectifierProcess(DiodeRectifier*   diode,
                      float*            out_buffer,
                      const float*      in_buffer,
                      unsigned          n_samples)
{
    float vt = diode->vt;
    float scale = diode->scale;
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = f_exp((in_buffer[i]/vt)-1) * scale;
    }
    return NOERR;
}

Error_t
DiodeRectifierProcessD(DiodeRectifierD* diode,
                       double*          out_buffer,
                       const double*    in_buffer,
                       unsigned         n_samples)
{
    double vt = diode->vt;
    double scale = diode->scale;
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = exp((in_buffer[i]/vt)-1) * scale;
    }
    return NOERR;
}

/*******************************************************************************
 DiodeRectifierTick */
float
DiodeRectifierTick(DiodeRectifier* diode, float in_sample)
{
    return f_exp((in_sample/diode->vt)-1) * diode->scale;
}

double
DiodeRectifierTickD(DiodeRectifierD* diode, double in_sample)
{
    return exp((in_sample/diode->vt)-1) * diode->scale;
}