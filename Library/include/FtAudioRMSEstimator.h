//
//  FtAudioRMSEstimator.h
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#ifndef FutureTechAudio_FtAudioRMSEstimator_h
#define FutureTechAudio_FtAudioRMSEstimator_h

#include "FtAudioError.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct FTA_RMSEstimator FTA_RMSEstimator;

    
/** Create a new FTA_RMSEstimator
 *
 * @details allocates memory and returns an initialized RMS Estimator
 *
 * @param avgTime       averaging time/ sliding window length
 # @param sampleRate    Sample rate.
 *
 */
FTA_RMSEstimator*
FTA_RMSEstimatorInit(float      avgTime,
                     long long  sampleRate);

/** Free memory allocated by FTA_RMSEstimatorInit
 *
 */
FTA_Error_t
FTA_RMSEstimatorFree(FTA_RMSEstimator* rms);
    
    
/** Calculate sliding RMS of a signal
 *
 * @details Uses an algorithm based on Newton's method for fast square-root
 *          calculation to estimate RMS power of a signal.
 */
FTA_Error_t 
FTA_RMSEstimatorProcess(FTA_RMSEstimator*   rms,
                        float*              outBuffer,
                        const float*        inBuffer,
                        unsigned            n_samples);


    
/** Return sliding RMS at the current sample
 *
 * @details Uses an algorithm based on Newton's method for fast square-root 
 *          calculation to estimate RMS power of a signal.
 */
float
FTA_RMSEstimatorTick(FTA_RMSEstimator*  rms,
                     float              inSample);
    
    
    
    
#ifdef __cplusplus
}
#endif
#endif
