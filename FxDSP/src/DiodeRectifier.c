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
 Diode */
struct Diode
{
    bias_t  bias;
    float   threshold;
    float   vt;
    float   scale;
};


struct DiodeD
{
    bias_t  bias;
    double  threshold;
    double  vt;
    double  scale;
};


/*******************************************************************************
 DiodeInit */
Diode*
DiodeInit(bias_t bias, float threshold)
{
    // Create opto struct
    Diode* diode = (Diode*)malloc(sizeof(Diode));
    
    // Initialization
    diode->bias = bias;
    diode->threshold = threshold;
    DiodeSetThreshold(diode, threshold);
    return diode;
}

DiodeD*
DiodeInitD(bias_t bias, double threshold)
{
    // Create opto struct
    DiodeD* diode = (DiodeD*)malloc(sizeof(DiodeD));
    
    // Initialization
    diode->bias = bias;
    diode->threshold = threshold;
    DiodeSetThresholdD(diode, threshold);
    return diode;
}


/*******************************************************************************
 DiodeFree */
Error_t
DiodeFree(Diode* diode)
{
    if(diode)
        free(diode);
    diode = NULL;
    return NOERR;
}

Error_t
DiodeFreeD(DiodeD* diode)
{
    if(diode)
        free(diode);
    diode = NULL;
    return NOERR;
}


/*******************************************************************************
 DiodeSetThreshold */
Error_t
DiodeSetThreshold(Diode* diode, float threshold)
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
DiodeSetThresholdD(DiodeD* diode, double threshold)
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
 DiodeProcess */
Error_t
DiodeProcess(Diode*         diode,
            float*          out_buffer,
            const float*    in_buffer,
            unsigned        n_samples)
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
DiodeProcessD(DiodeD*       diode,
              double*       out_buffer,
              const double* in_buffer,
              unsigned      n_samples)
{
    double vt = diode->vt;
    double scale = diode->scale;
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = f_expD((in_buffer[i]/vt)-1) * scale;
    }
    return NOERR;
}

/*******************************************************************************
 DiodeTick */
float
DiodeTick(Diode* diode, float in_sample)
{
    return f_exp((in_sample/diode->vt)-1) * diode->scale;
}

double
DiodeTickD(DiodeD* diode, double in_sample)
{
    return f_expD((in_sample/diode->vt)-1) * diode->scale;
}