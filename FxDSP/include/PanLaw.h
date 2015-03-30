//
//  PanLaw.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 3/30/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#ifndef __FxDSP__PanLaw__
#define __FxDSP__PanLaw__

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif

Error_t
linear_pan(float control, float *l_gain, float *r_gain);

Error_t
linear_panD(double control, double *l_gain, double *r_gain);

Error_t
equal_power_3dB_pan(float control, float *l_gain, float *r_gain);

Error_t
equal_power_3dB_panD(double control, double *l_gain, double *r_gain);

Error_t
equal_power_6dB_pan(float control, float *l_gain, float *r_gain);

Error_t
equal_power_6dB_panD(double control, double *l_gain, double *r_gain);
    
    
#ifdef __cplusplus
}
#endif

#endif /* defined(__FxDSP__PanLaw__) */
