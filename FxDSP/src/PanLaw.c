//
//  PanLaw.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 3/30/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "PanLaw.h"
#include <math.h>


Error_t
linear_pan(float control, float *l_gain, float *r_gain)
{
    *l_gain = 1.0f - control;
    *r_gain = control;
    return NOERR;
}

Error_t
linear_panD(double control, double *l_gain, double *r_gain)
{
    *l_gain = 1.0 - control;
    *r_gain = control;
    return NOERR;
}

Error_t
equal_power_3dB_pan(float control, float *l_gain, float *r_gain)
{
    *l_gain = sinf((1.0 - control) * M_PI_2);
    *r_gain = sinf(control * M_PI_2);
    return NOERR;
}

Error_t
equal_power_3dB_panD(double control, double *l_gain, double *r_gain)
{
    *l_gain = sin((1.0 - control) * M_PI_2);
    *r_gain = sin(control * M_PI_2);
    return NOERR;
}

Error_t
equal_power_6dB_pan(float control, float *l_gain, float *r_gain)
{
    *l_gain = powf(sinf((1.0 - control) * M_PI_2), 2.0);
    *r_gain = powf(sinf(control * M_PI_2), 2.0);
    return NOERR;
}

Error_t
equal_power_6dB_panD(double control, double *l_gain, double *r_gain)
{
    *l_gain = pow(sin((1.0 - control) * M_PI_2),2.0);
    *r_gain = pow(sin(control * M_PI_2), 2.0);
    return NOERR;
}
