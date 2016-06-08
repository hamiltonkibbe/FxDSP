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

/** Calculate linear pan channel gains
 *
 * Use a linear curve to mix between two sources. Results in a peak at the
 * center.
 *
 * @param control   Pan amount. Must be between 0 (hard left) and 1 (hard right)
 * @param l_gain    Left channel gain.
 * @param r_gain    Right channel gain.
 * @return          Error code.
 */
Error_t
linear_pan(float control, float *l_gain, float *r_gain);

Error_t
linear_panD(double control, double *l_gain, double *r_gain);

/** Calculate 3dB Equal-Power pan channel gains
 *
 * Use an equal-power curve to mix between two sources. Results in both
 * channels being down 3dB at the center.
 *
 * @param control   Pan amount. Must be between 0 (hard left) and 1 (hard right)
 * @param l_gain    Left channel gain.
 * @param r_gain    Right channel gain.
 * @return          Error code.
 */
Error_t
equal_power_3dB_pan(float control, float *l_gain, float *r_gain);

Error_t
equal_power_3dB_panD(double control, double *l_gain, double *r_gain);


/** Calculate 6dB Equal-Power pan channel gains
 *
 * Use an equal-power curve to mix between two sources. Results in both
 * channels being down 6dB at the center.
 *
 * @param control   Pan amount. Must be between 0 (hard left) and 1 (hard right)
 * @param l_gain    Left channel gain.
 * @param r_gain    Right channel gain.
 * @return          Error code.
 */
Error_t
equal_power_6dB_pan(float control, float *l_gain, float *r_gain);

Error_t
equal_power_6dB_panD(double control, double *l_gain, double *r_gain);


#ifdef __cplusplus
}
#endif

#endif /* defined(__FxDSP__PanLaw__) */
