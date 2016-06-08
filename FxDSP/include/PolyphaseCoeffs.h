/**
 * @file PolyphaseCoeffs.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef POLYPHASECOEFFS_H
#define POLYPHASECOEFFS_H

#ifdef __cplusplus
extern "C" {
#endif


/** Resampling Factor constants */
typedef enum factor
{
    /** 2x resampling */
    X2 = 0,

    /** 4x resampling */
    X4,

    /** 8x resampling */
    X8,

    /** 16x resampling */
    /*X16,*/

    /** number of resampling factors */
    N_FACTORS
} ResampleFactor_t;

extern const float** PolyphaseCoeffs[N_FACTORS];
extern const double** PolyphaseCoeffsD[N_FACTORS];

#ifdef __cplusplus
}
#endif


#endif //POLYPHASECOEFFS_H