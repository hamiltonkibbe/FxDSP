//
//  Decimator.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/10/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#ifndef DECIMATOR_H
#define DECIMATOR_H


#include "Error.h"
#include "PolyphaseCoeffs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Opaque Decimator object */
typedef struct Decimator Decimator;
typedef struct DecimatorD DecimatorD;



/** Create a new Decimator
 *
 * @details Allocates memory and returns an initialized Decimator with
 *          a given decimation factor.
 *
 * @param factor    Decimation factor
 * @return          An initialized Decimator
 */
Decimator*
DecimatorInit(ResampleFactor_t factor);

DecimatorD*
DecimatorInitD(ResampleFactor_t factor);

/** Free memory associated with a Upsampler
 *
 * @details release all memory allocated by DecimatorInit for the
 *          supplied filter.
 * @param decimator Decimator to free.
 * @return          Error code, 0 on success
 */
Error_t
DecimatorFree(Decimator* decimator);

Error_t
DecimatorFreeD(DecimatorD* decimator);

/** Flush decimator state buffers
 *
 * @param decimator Upsampler to flush.
 * @return          Error code, 0 on success
 */
Error_t
DecimatorFlush(Decimator* decimator);

Error_t
DecimatorFlushD(DecimatorD* decimator);

/** Decimate a buffer of samples
 *
 * @details Decimates given buffer using a polyphase decimator
 *
 * @param decimator The Decimator to use
 * @param outBuffer The buffer to write the output to
 * @param inBuffer  The buffer to filter
 * @param n_samples The number of samples to upsample
 * @return          Error code, 0 on success
 */
Error_t
DecimatorProcess(Decimator*     decimator,
                 float*         outBuffer,
                 const float*   inBuffer,
                 unsigned       n_samples);

Error_t
DecimatorProcessD(DecimatorD*   decimator,
                 double*        outBuffer,
                 const double*  inBuffer,
                 unsigned       n_samples);

#ifdef __cplusplus
}
#endif

#endif
