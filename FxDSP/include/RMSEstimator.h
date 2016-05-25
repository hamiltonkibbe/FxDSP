//
//  RMSEstimator.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#ifndef FxDSP_RMSEstimator_h
#define FxDSP_RMSEstimator_h

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct RMSEstimator RMSEstimator;
typedef struct RMSEstimatorD RMSEstimatorD;


/** Create a new RMSEstimator
 *
 * @details allocates memory and returns an initialized RMS Estimator
 *
 * @param avgTime       Averaging time/ sliding window length
 # @param sampleRate    Sample rate.
 *
 */
RMSEstimator*
RMSEstimatorInit(float avgTime, float sampleRate);

RMSEstimatorD*
RMSEstimatorInitD(double avgTime, double sampleRate);


/** Free memory allocated by RMSEstimatorInit
 *
 */
Error_t
RMSEstimatorFree(RMSEstimator* rms);

Error_t
RMSEstimatorFreeD(RMSEstimatorD* rms);


Error_t
RMSEstimatorFlush(RMSEstimator* rms);

Error_t
RMSEstimatorFlushD(RMSEstimatorD* rms);


/** Set the RMSEstimator Window Time
 *
 *
 * @param rms       RMSEstimator to update
 * @param avgTime   Averaging time.
 */
Error_t
RMSEstimatorSetAvgTime(RMSEstimator* rms, float avgTime);

Error_t
RMSEstimatorSetAvgTimeD(RMSEstimatorD* rms, double avgTime);


/** Calculate sliding RMS of a signal
 *
 * @details Uses an algorithm based on Newton's method for fast square-root
 *          calculation to estimate RMS power of a signal.
 *
 * @param rms       RMSEstimator to use.
 * @param outBuffer Destination to write RMS values to.
 * @param inBuffer  Samples to process
 * @param n_samples Number of samples to process
 */
Error_t
RMSEstimatorProcess(RMSEstimator*   rms,
                    float*          outBuffer,
                    const float*    inBuffer,
                    unsigned        n_samples);


Error_t
RMSEstimatorProcessD(RMSEstimatorD* rms,
                     double*        outBuffer,
                     const double*  inBuffer,
                     unsigned       n_samples);


/** Return sliding RMS at the current sample
 *
 * @details Uses an algorithm based on Newton's method for fast square-root
 *          calculation to estimate RMS power of a signal.
 *
 * @param rms       RMSEstimator to use
 * @param inSample  Sample to process
 * @return          RMS estimate.
 */
float
RMSEstimatorTick(RMSEstimator* rms, float inSample);

double
RMSEstimatorTickD(RMSEstimatorD* rms, double inSample);


#ifdef __cplusplus
}
#endif
#endif
